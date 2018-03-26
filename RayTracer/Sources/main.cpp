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
#include "Mesh.hpp"
#include "utilities.hpp"

std::default_random_engine engine;

int main(int argc, const char * argv[]) {
	
	if (argc != 3){
		std::cerr << "Usage: " << argv[0] << " SceneFile OutputFile" << std::endl;
		exit(1);
	}
	
	engine.seed((unsigned int)clock());
	
	Camera c(Vec3(0, 0, 3), Vec3(0, 0, -1), Vec3(0, 1, 0), 800, 600);
	c.antiAliazing = true;
	
	std::vector<Object*> obj;
	
	obj.push_back(new Mesh(argv[1], 1., Vec3(0, -1, 0), Vec3(1., 0., 0.), Vec3(0., 1., 0.), Vec3(0., 0., 1.), new DiffuseMat(Vec3(.8, .8, .8))));
	
	std::vector<Light> l;
	l.emplace_back(Vec3(0, 0, 3), Vec3(1.5, 1.5, 1.5));
/*	l.emplace_back(Vec3(-20, 20, 35), Vec3(0., 1., 0.));
	l.emplace_back(Vec3(0, -20, 35), Vec3(0., 0., 1.)); */
	
	Vec3 sky(0.2, 0.2, 0.2);
	
	Scene s(l, obj, c, sky);
	
	s.render("../test.png", 32, 1);
	//s.renderAO(argv[2], 1, 1);
	
	
}
