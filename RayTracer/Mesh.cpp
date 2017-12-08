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

Mesh::Mesh(const std::vector<Vec3>& sommets, const std::vector<Vec3>& normales, const std::vector<int>& faces, Material* m) :Object(m), sommets(sommets), normales(normales), faces(faces){}

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
		} else if (first == "f"){
			double x,y,z;
			line >> x >> y >> z;
			normales.emplace_back(x, y, z);
		} else if (first == "vn"){
			for (int i =0; i < 3; ++i){
				std::string v, ind;
				line >> v;
				std::istringstream buff(v);
				getline(buff, ind, '/');
				faces.emplace_back(atoi(ind.c_str()));
				getline(buff, ind, '/');
				getline(buff, ind, '/');
				normalesFace.emplace_back(atoi(ind.c_str()));
			}
		}
		
	}
}

Mesh::Mesh(const std::string& filename, Material* m): Object(m){
	
	readFromObj(filename);
	
}




bool Mesh::intersectTriangle(const Ray &ray, int i, Intersection& inter) const {
	
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
	inter.t = (ac * qvec) * inv_det;
	inter.pos = ray.origine + inter.t * ray.direction;
	inter.fromDir = -ray.direction;
	inter.obj = this;
	inter.norm = normalize(ab ^ ac);
	
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
