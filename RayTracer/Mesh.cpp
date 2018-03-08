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
#include "Mesh.hpp"
#include "tiny_obj_loader.h"

#define EPSILON 0.001
//Comment to use naive intersectin computation
#define MOLLER_TRUMBORE
//Comment to use face normals instead of smooth ones
//#define SMOOTH_NORMALS

#define USE_BVH

Mesh::Mesh(){}

Mesh::Mesh(const std::vector<Vec3>& sommets, const std::vector<Vec3>& normales, const std::vector<int>& faces, const std::vector<int>& normalesFace, Material* m) :Object(m), sommets(sommets), normales(normales), faces(faces), normalesFace(normalesFace), m_bvh(faces, sommets, this){
	
	smoothNormales.resize(sommets.size());
	for (int i = 0; i < normalesFace.size(); ++i){
		smoothNormales[faces[i]] += normales[normalesFace[i]];
	}
	for (auto& n:smoothNormales){
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
	std::ifstream in(filename);
	std::string s;
	
	double  minx = DBL_MAX,
			miny = DBL_MAX,
			minz = DBL_MAX;
	double  maxx = -DBL_MAX,
			maxy = -DBL_MAX,
			maxz = -DBL_MAX;
	while(getline(in, s)){
		std::istringstream line(s);
		std::string first;
		line >> first;
		if (first == "v"){
			double x,y,z;
			line >> x >> y >> z;
			sommets.emplace_back(x, y, z);
			if (x < minx)
				minx = x;
			else if (x > maxx)
				maxx = x;
			
			if (y < miny)
				miny = y;
			else if (y > miny)
				maxy = y;
			
			if (z <	minz)
				minz = z;
			else if (z > minz)
				maxz = z;
			
		} else if (first == "vn"){
			
			double x,y,z;
			line >> x >> y >> z;
			normales.emplace_back(x, y, z);
			
		} else if (first == "vt"){
			
			double u, v;
			line >> u >> v;
			UVs.emplace_back(u, v);
			
		} else if (first == "f"){
			int centre, ncentre, uvCentre;
			std::string v, ind;
			line >> v;
			std::istringstream buff(v);
			
			centre  = readIndex(buff, '/');
			uvCentre = readIndex(buff, '/');
			ncentre = readIndex(buff, '/');
			
			int pred, uvPred, npred;
			line >> v;
			buff = std::istringstream(v);
			
			pred = readIndex(buff, '/');
			uvPred = readIndex(buff, '/');
			npred = readIndex(buff, '/');
			
			while (line >> v){
				int act, uvAct, nact;
				buff = std::istringstream(v);
				
				act = readIndex(buff, '/');
				uvAct = readIndex(buff, '/');
				nact = readIndex(buff, '/');
				
				faces.push_back(centre);
				faces.push_back(pred);
				faces.push_back(act);
				normalesFace.push_back(ncentre);
				normalesFace.push_back(npred);
				normalesFace.push_back(nact);
				pred = act;
				npred = nact;
			}
		}
	}
	double lx,ly,lz;
	lx = maxx - minx;
	ly = maxy - miny;
	lz = maxz - minz;
	double resize = 2. / std::max(std::max(lx, ly), lz);
	for (auto& p : sommets){
		p *= resize;
	}
	
}

Vec3 toWorldBase(const Vec3& pos, const Vec3& X, const Vec3& Y, const Vec3& Z){
	return pos.x * X + pos.y * Y + pos.z * Z;
}

void Mesh::recomputeNormals(){
	normales.clear();
	normalesFace.clear();
	
	for (int i = 0; i < faces.size() / 3; ++i){
		normalesFace.push_back(i);
		normalesFace.push_back(i);
		normalesFace.push_back(i);
		Vec3& a = sommets[faces[3*i]];
		Vec3& b = sommets[faces[3*i+1]];
		Vec3& c = sommets[faces[3*i+2]];
		
		normales.push_back(normalize((b - a) ^ (c - a)));
	}
	
}

Mesh::Mesh(const std::string& filename, double scale, const Vec3& loc, const Vec3& newX, const Vec3& newY, const Vec3& newZ, Material* m): Object(m){
	
	readFromObj(filename);
	
	for (auto& p : sommets){
		p = toWorldBase(p, newX, newY, newZ);
		p *= scale;
		p += loc;
	}
	for (auto& n : normales){
		n = toWorldBase(n, newX, newY, newZ);
		n.normalize();
	}
	
	if (normales.size() == 0)
		recomputeNormals();
	
	smoothNormales.resize(sommets.size());
	for (int i = 0; i < normalesFace.size(); ++i){
		smoothNormales[faces[i]] += normales[normalesFace[i]];
	}
	for (auto& n:smoothNormales){
		n.normalize();
	}
	
	m_bvh = BVH(faces, sommets, this);
	
}


Vec3 normalInterpol(const float u, const float v, const Vec3& na, const Vec3& nb, const Vec3& nc){
	float w= 1.f - u - v;
	return na * w + nb * u + nc * v;
}

//Moller Trumbore Algorithm code adapted from Jean-Claude Iehl code
//Alternative code derived from intersection equation solving
bool Mesh::intersectTriangle(const Ray &ray, int i, Intersection& inter, double tmax) const {
	
	const Vec3& a = sommets[faces[i*3]];
	const Vec3& b = sommets[faces[i*3+1]];
	const Vec3& c = sommets[faces[i*3+2]];

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
	Vec3 na = smoothNormales[faces[3*i]];
	Vec3 nb = smoothNormales[faces[3*i + 1]];
	Vec3 nc = smoothNormales[faces[3*i + 2]];
#else
	Vec3 na = normales[normalesFace[3*i]];
	Vec3 nb = normales[normalesFace[3*i + 1]];
	Vec3 nc = normales[normalesFace[3*i + 2]];
#endif
	
	inter.norm = normalInterpol(u, v, na, nb, nc);
	
	// ne renvoie vrai que si l'intersection est valide (comprise entre tmin et tmax du rayon)
	return true;
#else
	
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	
	//Normale au plan du triangle
	Vec3 N = normalize(ab ^ ac);
	
	//Composante du rayon selon la normale
	double compDir = N * ray.direction;
	
	if (compDir == 0.){
		return false;
	}
	
	//Composante de l'origine du rayon selon la normale
	double compPos = N * ray.origine;
	
	//Distance entre le plan et (0,0,0)
	// N est la normale au plan donc tous les points du plan ont la même composante selon N
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
		
		Vec3 na = smoothNormales[faces[3*i]]; //normales[normalesFace[3*i]]; //
		Vec3 nb = smoothNormales[faces[3*i + 1]]; //normales[normalesFace[3*i + 1]]; //
		Vec3 nc = smoothNormales[faces[3*i + 2]]; //normales[normalesFace[3*i + 2]]; //
		
		inter.norm = normalInterpol(coefB, coefC, na, nb, nc);
		return true;
		
	}
	
	return false;
	
#endif
	
}

//Moller Trumbore Algorithm code adapted from Jean-Claude Iehl code
//Alternative code derived from intersection equation solving
bool Mesh::intersectTriangle(const Ray &ray, int i, double tmax) const {
	
	const Vec3& a = sommets[faces[i*3]];
	const Vec3& b = sommets[faces[i*3+1]];
	const Vec3& c = sommets[faces[i*3+2]];
	
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
	
	//Normale au plan du triangle
	Vec3 N = normalize(ab ^ ac);
	
	//Composante du rayon selon la normale
	double compDir = N * ray.direction;
	
	if (compDir == 0.){
		return false;
	}
	
	//Composante de l'origine du rayon selon la normale
	double compPos = N * ray.origine;
	
	//Distance entre le plan et (0,0,0)
	// N est la normale au plan donc tous les points du plan ont la même composante selon N
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
