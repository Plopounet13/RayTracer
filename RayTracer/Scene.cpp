//
//  Scene.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 21/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cfloat>
#include <cmath>
#include <random>
#include <iostream>
#include "Image.hpp"
#include "Scene.hpp"

//Comment to use random directions to sample AO
#define FIBO_AO
//Comment to stop randomly rotating sampling base
#define TILTED_BASE
//#define DIFF_AO

#define MYEPS 0.001
#define PHI 1.61803398875

extern std::default_random_engine engine;
std::uniform_real_distribution<double> unifScene(0,1);

Scene::Scene(){}

Scene::Scene(const Scene& s){
	lights = s.lights;
	objects = s.objects;
	c = s.c;
	skyColor = s.skyColor;
}

Scene::Scene(const std::vector<Light>& l, const std::vector<Object*>& o, const Camera& cam, const Vec3& sky){
	lights = l;
	objects = o;
	c = cam;
	skyColor = sky;
}

Scene::~Scene(){
	for (auto o : objects){
		delete o;
	}
}

bool Scene::intersect(const Ray& r, double tmax) const{
	for (auto& o : objects){
		if (o->intersect(r, tmax)){
			return true;
		}
	}
	return false;
}

bool Scene::intersect(const Ray& r, Intersection& inter, double tmax) const{
	Intersection tmpInter;
	bool res = false;
	inter.t = tmax;
	for (auto& o : objects){
		if (o->intersect(r, tmpInter, inter.t)){
			inter = tmpInter;
			res = true;
		}
	}
	return res;
}

Vec3 Scene::getCol(const Ray& r, bool indirect, int nbRecur) const{
	Vec3 col;
	Intersection inter;
	if (intersect(r, inter)){
		col = inter.obj->getCol(*this, inter, indirect, nbRecur);
	} else {
		col = skyColor;
	}
	return col;
}

void Scene::render(std::string filename, int nbRay, int nbRecur) const{
	Image im(c.width, c.height);
	
	bool indirect = (nbRay!=0);
	
#pragma omp parallel for schedule(dynamic, 16)
	for (int j = 0; j < c.height; ++j){
		for (int i = 0; i < c.width; ++i){
			Vec3 col;
			int k = 0;
			do {
				Ray r = c.cast_Ray(i, j);
				col += getCol(r, indirect, nbRecur);
			} while (++k < nbRay);
			if (nbRay)
				col /= (double)nbRay;
			im.store(i, j, col);
		}
	}
	
	im.save(filename);
	
}

// b1, b2, n sont 3 axes orthonormes.
void branchlessONB(const Vec3 &n, Vec3 &b1, Vec3 &b2)
{
	float sign = std::copysign(1.0f, n.z);
	const float a = -1.0f / (sign + n.z);
	const float b = n.x * n.y * a;
	b1 = Vec3(1.0f + sign * n.x * n.x * a, sign * b, -sign * n.x);
	b2 = Vec3(b, sign + n.y * n.y * a, -n.y);
}

Vec3 Scene::getAO(const Ray& r, int nbRay) const{
	
	Intersection inter;
	if (!intersect(r, inter))
		return skyColor;
	
	float total = 0;
	Vec3 x, y, z;
	y = inter.norm;
#ifdef TILTED_BASE
	Vec3 randAxe(unifScene(engine)-0.5, unifScene(engine)-0.5, unifScene(engine)-0.5);
	x = y ^ randAxe;
	x.normalize();
	
	z = x ^ y;
#else
	branchlessONB(y, x, z);
#endif
	
	for (int i = 0; i < nbRay; ++i){
		Vec3 dir;
		float weight;
#ifdef FIBO_AO
		float i_PHI = i / PHI;
		float phi = 2.*M_PI * (i_PHI - floor(i_PHI));
		float cosTheta = 1 - (2. * i + 1.) / (2. * nbRay);
		float sinTheta = sqrt(1 - cosTheta*cosTheta);
		float cosPhi = cos(phi);
		float sinPhi = sin(phi);
		
		dir = Vec3(cosPhi*sinTheta, cosTheta, sinPhi*sinTheta);
		
		weight = dir.y * 2.;
		
		dir = x * dir.x + y * dir.y + z * dir.z;
		
		
#else
		double r1, r2;
		r1 = unifScene(engine);
		r2 = unifScene(engine);
#ifdef DIFF_AO
		double msqrt = sqrt(1. - r2);
		dir.x = cos(2. * M_PI * r1) * msqrt;
		dir.z = sin(2. * M_PI * r1) * msqrt;
		dir.y = sqrt(r2);
		dir = x * dir.x + y * dir.y + z * dir.z;
		
		//M_PI ? Non
		weight = 1.;
#else
		double msqrt = sqrt(1. - r2*r2);
		dir.x = cos(2. * M_PI * r1) * msqrt;
		dir.z = sin(2. * M_PI * r1) * msqrt;
		dir.y = r2;
		//M_PI ? Non
		weight = dir.y * 2.;
		
		dir = x * dir.x + y * dir.y + z * dir.z;
#endif
#endif
		
		//TODO: test integration of cos(theta)
		//Resultats: Fibo intègre à 0.5, Random sampling intègre à 1
		if (!intersect(Ray(inter.pos + MYEPS * y, dir)))
			total += weight;
		
	}
	total /= nbRay;
	
	return Vec3(total, total, total);
}

void Scene::renderAO(std::string filename, int nbRay) const{
	Image im(c.width, c.height);
	
#pragma omp parallel for schedule(dynamic, 16)
	for (int j = 0; j < c.height; ++j){
		for (int i = 0; i < c.width; ++i){
			Vec3 col;
			
			Ray r = c.cast_Ray(i, j);
			
			col = getAO(r, nbRay);
			//col = Vec3(float(i) / c.width, float(j) / c.height, 0);
			
			im.store(i, j, col);
		}
	}
	
	im.save(filename);
	
}
