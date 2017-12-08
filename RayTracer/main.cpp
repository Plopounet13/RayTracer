//
//  main.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/20.2.
//  Copyright © 20.2 Loïs Paulin. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <ctime>

#include "Sphere.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "Vec3.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Scene.hpp"

#include "utilities.hpp"

std::default_random_engine engine;

int main(int argc, const char * argv[]) {
	
	engine.seed((unsigned int)clock());
	
	Camera c(Vec3(0, 0, 55), Vec3(0, 0, -1), Vec3(0, 1, 0), 400, 320);
	
	std::vector<Object*> obj;
	obj.push_back(new Sphere(Vec3(0, 0, -1060), 1000, new DiffuseMat(Vec3(0., 0.2, 0.))));
	;
	obj.push_back(new Sphere(Vec3(0, -1010, 0), 1000, new DiffuseMat(Vec3(0.07, 0.07, 0.5))));
	;
	obj.push_back(new Sphere(Vec3(0, 1060, 0), 1000, new DiffuseMat(Vec3(0.2, 0., 0.))));
	;
	obj.push_back(new Sphere(Vec3(0, 0, 1060), 1000, new DiffuseMat(Vec3(.8, .8, .8))));
	;
	obj.push_back(new Sphere(Vec3(1060, 0, 0), 1000, new DiffuseMat(Vec3(0.2, 0.2, 0.))));
	;
	obj.push_back(new Sphere(Vec3(-1060, 0, 0), 1000, new DiffuseMat(Vec3(0.2, 0., 0.2))));
	;
	obj.push_back(new Sphere(Vec3(0, 0, 20), 10, new DiffuseMat(Vec3(.9, .9, .9))));
	;
	
	std::vector<Light> l;
	l.emplace_back(Vec3(-10, 20, 40), Vec3(1.5, 1.5, 1.5));
/*	l.emplace_back(Vec3(-20, 20, 35), Vec3(0., 1., 0.));
	l.emplace_back(Vec3(0, -20, 35), Vec3(0., 0., 1.)); */
	
	Vec3 sky(0.2, 0.2, 0.2);
	
	Scene s(l, obj, c, sky);
	
	clock_t t;
	t = clock();
	s.render("/Users/lois/Documents/M2/MMSI/test.png", 10, 5);
	t = clock() - t;
	float time = (float)t / CLOCKS_PER_SEC;
	std::cout << "Rendu calculé en " << time << " secondes." << std::endl;
	/*
	for (int j = 0; j < c.height; ++j){
		for (int i = 0; i < c.width; ++i){
			Ray r = c.cast_Ray(i, j);
			Vec3 point, norm;
			double t;
			if (s.intersect(r, t, point, norm)){
				Vec3 lightDir = normalize(l.pos - point);
				Ray versLight(point, lightDir);
				col = Vec3(0, 0, 0);
				if (!s.intersect(versLight)){
					double getCol = std::abs(lightDir * norm);
					col = l.col * getCol;
				}
			} else {
				col = sky;
			}
			im.store(i, j, col);
		}
	}
	
	im.save("/Users/lois/Documents/M2/MMSI/test.png");*/
	
	
}
