//
//  Sphere.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cmath>
#include "Sphere.hpp"

#define MYEPS 0.001

Sphere::Sphere():Object(){}

Sphere::Sphere(const Vec3& c, double r, Material* m):Object(m){
	center = c;
	radius = r;
}

bool Sphere::intersect(const Ray& r, double tmax) const{
	
	Vec3 CO = r.origine - center;
	
	double b = 2 * r.direction * CO;
	double c = CO.norm2() - radius*radius;
	
	double delta = b * b - 4 * c;
	
	double t1 = (- b - sqrt(delta)) / 2;
	double t2 = (- b + sqrt(delta)) / 2;
	double t = (t1 >= 0) ? t1 : t2;
	
	return delta >= 0 && t >= 0 && t < tmax;
}


bool Sphere::intersect(const Ray& r, Intersection& inter, double tmax) const{
	
	Vec3 CO = r.origine - center;
	
	double b = 2 * r.direction * CO;
	double c = CO.norm2() - radius*radius;
	
	double delta = b * b - 4 * c;
	
	double t1 = (- b - sqrt(delta)) / 2;
	double t2 = (- b + sqrt(delta)) / 2;
	
	if (t1 >= 0){
		inter.pos = r.origine + t1 * r.direction;
		inter.t = t1;
	} else if (t2 >= 0) {
		inter.pos = r.origine + t2 * r.direction;
		inter.t = t2;
	}
	
	inter.norm = inter.pos - center;
	inter.norm.normalize();
	inter.obj = this;
	inter.fromDir = -r.direction;
	return delta >= 0 && inter.t >= 0 && inter.t < tmax;
}










