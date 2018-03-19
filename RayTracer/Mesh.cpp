//
//  Mesh.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 30/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cfloat>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Mesh.hpp"
#define TINYOBJLOADER_USE_DOUBLE
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define EPSILON 0.00000001
//Comment to use naive intersectin computation
#define MOLLER_TRUMBORE
//Comment to use face normals instead of smooth ones
//#define SMOOTH_NORMALS

#define USE_BVH

Mesh::Mesh(){}

Mesh::Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec3>& normals, const std::vector<indices>& faces, Material* m) :Object(m), vertices(vertices), normals(normals), faces(faces), m_bvh(faces, vertices, this){
	
	smoothnormals.resize(vertices.size());
	for (int i = 0; i < faces.size(); ++i){
		smoothnormals[faces[i].vertex] += normals[faces[i].normal];
	}
	for (auto& n:smoothnormals){
		n.normalize();
	}
}

int readIndex(std::istream& buff, const char sep){
	
	int res;
	std::string ind;
	getline(buff, ind, sep);
	if (!buff.fail()){
		res = atoi(ind.c_str()) - 1;
	} else {
		res = -1;
	}
	return res;
	
}

void Mesh::readFromObj(const std::string& filename){
	
	tinyobj::attrib_t att;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> mats;
	std::string err;
	
	if ( !tinyobj::LoadObj(&att, &shapes, &mats, &err, filename.c_str()) )
		std::cerr << err << std::endl;
	
	vertices.resize(att.vertices.size() / 3);
	for (int i = 0; i < vertices.size(); ++i){
		vertices[i].x = att.vertices[i*3 + 0];
		vertices[i].y = att.vertices[i*3 + 1];
		vertices[i].z = att.vertices[i*3 + 2];
	}
	
	normals.resize(att.normals.size() / 3);
	for (int i = 0; i < normals.size(); ++i){
		normals[i].x = att.normals[i*3 + 0];
		normals[i].y = att.normals[i*3 + 1];
		normals[i].z = att.normals[i*3 + 2];
	}
	texCoords.resize(att.texcoords.size() / 2);
	for (int i = 0; i < texCoords.size(); ++i){
		texCoords[i].first = att.texcoords[i*2 + 0];
		texCoords[i].second = att.texcoords[i*2 + 1];
	}
	
	int totalSize = 0;
	for (const tinyobj::shape_t& s : shapes)
		totalSize += s.mesh.indices.size();
	
	faces.resize(totalSize);
	int i = 0;
	for (const tinyobj::shape_t& s : shapes){
		for (const tinyobj::index_t& ind : s.mesh.indices){
			faces[i].vertex = ind.vertex_index;
			faces[i].normal = ind.normal_index;
			faces[i++].texCoords = ind.texcoord_index;
		}
	}
	
	
	double maxx = -DBL_MAX, minx = DBL_MAX;
	double maxy = -DBL_MAX, miny = DBL_MAX;
	double maxz = -DBL_MAX, minz = DBL_MAX;
	for (auto& p : vertices){
		maxx = std::max(maxx, p.x);
		maxy = std::max(maxy, p.y);
		maxz = std::max(maxz, p.z);
		minx = std::min(minx, p.x);
		miny = std::min(miny, p.y);
		minz = std::min(minz, p.z);
	}
	double lx,ly,lz;
	lx = maxx - minx;
	ly = maxy - miny;
	lz = maxz - minz;
	double resize = 2. / std::max(std::max(lx, ly), lz);
	for (auto& p : vertices)
		p *= resize;
	
	
	
}

Vec3 toWorldBase(const Vec3& pos, const Vec3& X, const Vec3& Y, const Vec3& Z){
	return pos.x * X + pos.y * Y + pos.z * Z;
}

void Mesh::recomputeNormals(){
	normals.clear();
	
	for (int i = 0; i < faces.size() / 3; ++i){
		faces[i * 3 + 0].normal = i;
		faces[i * 3 + 1].normal = i;
		faces[i * 3 + 2].normal = i;
		Vec3& a = vertices[faces[3*i].vertex];
		Vec3& b = vertices[faces[3*i+1].vertex];
		Vec3& c = vertices[faces[3*i+2].vertex];
		
		normals.push_back(normalize((b - a) ^ (c - a)));
	}
	
}

Mesh::Mesh(const std::string& filename, double scale, const Vec3& loc, const Vec3& newX, const Vec3& newY, const Vec3& newZ, Material* m): Object(m){
	
	readFromObj(filename);
	
	for (auto& p : vertices){
		p = toWorldBase(p, newX, newY, newZ);
		p *= scale;
		p += loc;
	}
	for (auto& n : normals){
		n = toWorldBase(n, newX, newY, newZ);
		n.normalize();
	}
	
	if (normals.size() == 0)
		recomputeNormals();
	
	smoothnormals.resize(vertices.size());
	for (int i = 0; i < faces.size(); ++i){
		smoothnormals[faces[i].vertex] += normals[faces[i].normal];
	}
	for (auto& n:smoothnormals){
		n.normalize();
	}
	
	m_bvh = BVH(faces, vertices, this);
	
}


Vec3 normalInterpol(const float u, const float v, const Vec3& na, const Vec3& nb, const Vec3& nc){
	float w= 1.f - u - v;
	return na * w + nb * u + nc * v;
}

//Moller Trumbore Algorithm code adapted from Jean-Claude Iehl code
//Alternative code derived from intersection equation solving
bool Mesh::intersectTriangle(const Ray &ray, int i, Intersection& inter, double tmax) const {
	
	const Vec3& a = vertices[faces[i*3].vertex];
	const Vec3& b = vertices[faces[i*3+1].vertex];
	const Vec3& c = vertices[faces[i*3+2].vertex];

#ifdef MOLLER_TRUMBORE
	/* begin calculating determinant - also used to calculate U parameter */
	Vec3 ac = c - a;
	Vec3 pvec = ray.direction ^ ac;
	
	/* if determinant is near zero, ray lies in plane of triangle */
	Vec3 ab = b - a;
	float det = ab * pvec;
	if(det > -EPSILON && det < EPSILON)
		return false;
	
	float inv_det= 1.0 / det;
	
	/* calculate distance from vert0 to ray origin */
	Vec3 tvec = ray.origine - a;
	
	/* calculate U parameter and test bounds */
	float u  = (tvec * pvec) * inv_det;
	if(u < 0.0 || u > 1.0)
		return false;
	
	/* prepare to test V parameter */
	Vec3 qvec = tvec ^ ab;
	
	/* calculate V parameter and test bounds */
	float v = (ray.direction * qvec) * inv_det;
	if(v < 0.0 || u + v > 1.0)
		return false;
	
	/* calculate t, ray intersects triangle */
	double t = (ac * qvec) * inv_det;
	
	if (t < 0 || t >= tmax)
		return false;
	
	inter.t = t;
	inter.pos = ray.origine + inter.t * ray.direction;
	inter.fromDir = -ray.direction;
	inter.obj = this;
	
#ifdef SMOOTH_NORMALS
	Vec3 na = smoothnormals[faces[3*i].vertex];
	Vec3 nb = smoothnormals[faces[3*i + 1].vertex];
	Vec3 nc = smoothnormals[faces[3*i + 2].vertex];
#else
	Vec3 na = normals[faces[3*i].normal];
	Vec3 nb = normals[faces[3*i + 1].normal];
	Vec3 nc = normals[faces[3*i + 2].normal];
#endif
	
	inter.norm = normalInterpol(u, v, na, nb, nc);
	
	// ne renvoie vrai que si l'intersection est valide (comprise entre tmin et tmax du rayon)
	return true;
#else
	
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	
	//normal au plan du triangle
	Vec3 N = normalize(ab ^ ac);
	
	//Composante du rayon selon la normal
	double compDir = N * ray.direction;
	
	if (compDir == 0.){
		return false;
	}
	
	//Composante de l'origine du rayon selon la normal
	double compPos = N * ray.origine;
	
	//Distance entre le plan et (0,0,0)
	// N est la normal au plan donc tous les points du plan ont la même composante selon N
	double D = - N * a;
	
	//compPos + D = distance du plan à l'origine du point
	double t = - (compPos + D) / compDir;
	
	//Si l'objet est derrière le rayon
	if (t < 0 || t >= tmax){
		return false;
	}
	
	Vec3 p = ray.origine + t * ray.direction;
	
	Vec3 ap = p - a;
	
	//Aire du triangle sans recalculer sqrt pour la norme du produit vectoriel
	double aireTriangle = N * ((b - a) ^ (c - a));
	
	double aireABP = (ab ^ ap) * N;
	double aireAPC = (ap ^ ac) * N;
	
	double coefC = aireABP / aireTriangle;
	double coefB = aireAPC / aireTriangle;
	
	if (coefB + coefC <= 1.
		&&
		(coefB <= 1. && coefB >= 0.)
		&&
		(coefC >= 0. && coefC <= 1.)){
		
		inter.t = t;
		inter.pos = p;
		inter.fromDir = -ray.direction;
		inter.obj = this;
		
		Vec3 na = smoothnormals[faces[3*i]]; //normals[normalsFace[3*i]]; //
		Vec3 nb = smoothnormals[faces[3*i + 1]]; //normals[normalsFace[3*i + 1]]; //
		Vec3 nc = smoothnormals[faces[3*i + 2]]; //normals[normalsFace[3*i + 2]]; //
		
		inter.norm = normalInterpol(coefB, coefC, na, nb, nc);
		return true;
		
	}
	
	return false;
	
#endif
	
}

//Moller Trumbore Algorithm code adapted from Jean-Claude Iehl code
//Alternative code derived from intersection equation solving
bool Mesh::intersectTriangle(const Ray &ray, int i, double tmax) const {
	
	const Vec3& a = vertices[faces[i*3].vertex];
	const Vec3& b = vertices[faces[i*3+1].vertex];
	const Vec3& c = vertices[faces[i*3+2].vertex];
	
#ifdef MOLLER_TRUMBORE
	/* begin calculating determinant - also used to calculate U parameter */
	Vec3 ac = c - a;
	Vec3 pvec = ray.direction ^ ac;
	
	/* if determinant is near zero, ray lies in plane of triangle */
	Vec3 ab = b - a;
	float det = ab * pvec;
	if(det > -EPSILON && det < EPSILON)
		return false;
	
	float inv_det= 1.0 / det;
	
	/* calculate distance from vert0 to ray origin */
	Vec3 tvec = ray.origine - a;
	
	/* calculate U parameter and test bounds */
	float u  = (tvec * pvec) * inv_det;
	if(u < 0.0 || u > 1.0)
		return false;
	
	/* prepare to test V parameter */
	Vec3 qvec = tvec ^ ab;
	
	/* calculate V parameter and test bounds */
	float v = (ray.direction * qvec) * inv_det;
	if(v < 0.0 || u + v > 1.0)
		return false;
	
	/* calculate t, ray intersects triangle */
	double t = (ac * qvec) * inv_det;
	
	if (t < 0 || t >= tmax)
		return false;
	
	// ne renvoie vrai que si l'intersection est valide (comprise entre tmin et tmax du rayon)
	return true;
#else
	
	
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	
	//normal au plan du triangle
	Vec3 N = normalize(ab ^ ac);
	
	//Composante du rayon selon la normal
	double compDir = N * ray.direction;
	
	if (compDir == 0.){
		return false;
	}
	
	//Composante de l'origine du rayon selon la normal
	double compPos = N * ray.origine;
	
	//Distance entre le plan et (0,0,0)
	// N est la normal au plan donc tous les points du plan ont la même composante selon N
	double D = - N * a;
	
	//compPos + D = distance du plan à l'origine du point
	double t = - (compPos + D) / compDir;
	
	//Si l'objet est derrière le rayon
	if (t < 0 || t >= tmax){
		return false;
	}
	
	Vec3 p = ray.origine + t * ray.direction;
	
	Vec3 ap = p - a;
	
	//Aire du triangle sans recalculer sqrt pour la norme du produit vectoriel
	double aireTriangle = N * ((b - a) ^ (c - a));
	
	double aireABP = (ab ^ ap) * N;
	double aireAPC = (ap ^ ac) * N;
	
	double coefC = aireABP / aireTriangle;
	double coefB = aireAPC / aireTriangle;
	
	if (coefB + coefC <= 1.
		&&
		(coefB <= 1. && coefB >= 0.)
		&&
		(coefC >= 0. && coefC <= 1.)){
		
		return true;
		
	}
	
	return false;
	
#endif
}


bool Mesh::intersect(const Ray& r, double tmax) const{
	
#ifdef USE_BVH
	return m_bvh.intersect(r, tmax);
	
#else
	
	for (int i = 0; i < faces.size() / 3; ++i){
		if (intersectTriangle(r, i, tmax)){
			return true;
		}
	}
	return false;
	
#endif
}

bool Mesh::intersect(const Ray& r, Intersection& inter, double tmax) const{
	
#ifdef USE_BVH
	return m_bvh.intersect(r, inter, tmax);
	
#else
	
	Intersection tmpInter;
	inter.t = tmax;
	
	for (int i = 0; i < faces.size() / 3; ++i){
		if (intersectTriangle(r, i, tmpInter, inter.t)){
			inter = tmpInter;
		}
	}
	
	return (inter.t != DBL_MAX);
	
#endif
}
