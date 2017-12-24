//
//  Camera.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <iostream>

#include <cmath>
#include <random>
#include "Camera.hpp"

extern std::default_random_engine engine;
extern std::uniform_real_distribution<double> unif;
std::uniform_real_distribution<double> myRand(-1., 1.);

Camera::Camera(const Vec3& p, const Vec3& d, const Vec3& u, int w, int h, double f){
	pos = p;
	dir = normalize(d);
	up = normalize(u);
	width = w;
	height = h;
	fov = f;
	right = dir ^ up;
	dirCentre = Vec3( -width * 0.5 + 0.5,  -height * 0.5 + 0.5, -height / (2. * tan((fov * (M_PI / 180.)) / 2.)));
}

Camera::Camera(const Camera& c){
	pos = c.pos;
	dir = c.dir;
	up = c.up;
	width = c.width;
	height = c.height;
	fov = c.fov;
	right = c.right;
}

Ray Camera::cast_Ray(int i, int j) const{
	
	Vec3 dDecalPixel(i, j, 0.);
	
	//Compute Gaussian randomization for Anti Aliazing
	if (antiAliazing){
		double x = unif(engine);
		double y = unif(engine);
		
		double frontFactor = std::sqrt(-2. * log(x));
		
		dDecalPixel.x += frontFactor * std::cos(2. * M_PI * y) * 0.5;
		dDecalPixel.y += frontFactor * std::sin(2. * M_PI * y) * 0.5;
		/* Rejective method Supposed faster but mean result same and greater std
		double x, y, s;
		do {
			x = myRand(engine);
			y = myRand(engine);
			s = x*x + y*y;
		} while( s >= 1. );
		double frontFactor = std::sqrt(-2. * log(s) / s);
		dDecalPixel.x += x * frontFactor;
		dDecalPixel.y += y * frontFactor;*/
		
	}
	
	Vec3 tmpd = dirCentre + dDecalPixel;
	
	return Ray(pos, tmpd.x * right + tmpd.y * up - tmpd.z * dir);
}
