//
//  Scene.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 21/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cfloat>
#include "Image.hpp"
#include "Scene.hpp"


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

bool Scene::intersect(const Ray& r) const{
	for (auto& o : objects){
		if (o->intersect(r)){
			return true;
		}
	}
	return false;
}

bool Scene::intersect(const Ray& r, Intersection& inter) const{
	Intersection tmpInter;
	bool res = false;
	inter.t = DBL_MAX;
	for (auto& o : objects){
		if (o->intersect(r, tmpInter) && tmpInter.t < inter.t){
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
			Ray r = c.cast_Ray(i, j);
			int k = 0;
			do {
				col += getCol(r, indirect, nbRecur);
			} while (++k < nbRay);
			if (nbRay)
				col /= (double)nbRay;
			im.store(i, j, col);
		}
	}
	
	im.save(filename);//"/Users/lois/Documents/M2/MMSI/test.png");
	
}
