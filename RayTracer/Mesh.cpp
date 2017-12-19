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

#define EPSILON 0.001

Mesh::Mesh(){}

Mesh::Mesh(const std::vector<Vec3>& sommets, const std::vector<Vec3>& normales, const std::vector<int>& faces, const std::vector<int>& normalesFace, Material* m) :Object(m), sommets(sommets), normales(normales), faces(faces), normalesFace(normalesFace){}

void Mesh::readFromObj(const std::string& filename){
	std::ifstream in(filename);
	std::string s;
	while(getline(in, s)){
		std::istringstream line(s);
		std::string first;
		line >> first;
		if (first == "v"){
			double x,y,z;
			line >> x >> y >> z;
			sommets.emplace_back(x, y, z);
		} else if (first == "vn"){
			double x,y,z;
			line >> x >> y >> z;
			normales.emplace_back(x, y, z);
		} else if (first == "f"){
			for (int i =0; i < 3; ++i){
				std::string v, ind;
				line >> v;
				std::istringstream buff(v);
				getline(buff, ind, '/');
				faces.emplace_back(atoi(ind.c_str()) - 1);
				getline(buff, ind, '/');
				getline(buff, ind);
				normalesFace.emplace_back(atoi(ind.c_str()) - 1);
			}
		}
		
	}
}

Vec3 toWorldBase(const Vec3& pos, const Vec3& X, const Vec3& Y, const Vec3& Z){
	return pos.x * X + pos.y * Y + pos.z * Z;
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
	
	smoothNormales.resize(sommets.size());
	for (int i = 0; i < normalesFace.size(); ++i){
		smoothNormales[faces[i]] += normales[normalesFace[i]];
	}
	for (auto& n:smoothNormales){
		n.normalize();
	}
	
}


Vec3 normalInterpol(const float u, const float v, const Vec3& na, const Vec3& nb, const Vec3& nc){
	float w= 1.f - u - v;
	return na * w + nb * u + nc * v;
}

bool Mesh::intersectTriangle(const Ray &ray, int i, Intersection& inter) const {
	
	const Vec3& a = sommets[faces[i*3]];
	const Vec3& b = sommets[faces[i*3+1]];
	const Vec3& c = sommets[faces[i*3+2]];
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
	inter.t = (ac * qvec) * inv_det;
	
	if (inter.t < 0)
		return false;
	
	inter.pos = ray.origine + inter.t * ray.direction;
	inter.fromDir = -ray.direction;
	inter.obj = this;
	
	Vec3 na = smoothNormales[faces[3*i]];
	Vec3 nb = smoothNormales[faces[3*i + 1]];
	Vec3 nc = smoothNormales[faces[3*i + 2]];
	
	inter.norm = normalInterpol(u, v, na, nb, nc);
	
	// ne renvoie vrai que si l'intersection est valide (comprise entre tmin et tmax du rayon)
	return true;
}


bool Mesh::intersectTriangle(const Ray &ray, int i) const {
	
	const Vec3& a = sommets[i*3];
	const Vec3& b = sommets[i*3+1];
	const Vec3& c = sommets[i*3+2];
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
	
	if (t < 0)
		return false;
	
	// ne renvoie vrai que si l'intersection est valide (comprise entre tmin et tmax du rayon)
	return true;
}


bool Mesh::intersect(const Ray& r) const{
		
	for (int i = 0; i < faces.size() / 3; ++i){
		if (intersectTriangle(r, i)){
			return true;
		}
	}
	return false;
	
}


bool Mesh::intersect(const Ray& r, Intersection& inter) const{
	
	Intersection tmpInter;
	inter.t = DBL_MAX;
	
	for (int i = 0; i < faces.size() / 3; ++i){
		if (intersectTriangle(r, i, tmpInter) && tmpInter.t < inter.t){
			inter = tmpInter;
		}
	}
	
	return (inter.t != DBL_MAX);
	
}
