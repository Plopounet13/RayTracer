//
//  Material.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 22/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cmath>
#include <random>
#include <algorithm>
#include "Scene.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "utilities.hpp"

#define MYEPS 0.001

extern std::default_random_engine engine;
std::uniform_real_distribution<double> unif(0,1);

Material::~Material(){}

DiffuseMat::DiffuseMat(){}

DiffuseMat::DiffuseMat(const Vec3& c){
	color = c;
}

Vec3 DiffuseMat::getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const{
	
	if (color == Vec3(.9, .9, .9))
		int a = 0;
	Vec3 col(0., 0., 0.);
	
	for (auto& l : s.lights){
		Vec3 lightDir = l.pos - inter.pos;
		double lightT = lightDir.norm();
		lightDir.normalize();
		Ray versLight(inter.pos + MYEPS * inter.norm, lightDir);
		Intersection interTmp;
		if (! s.intersect(versLight, interTmp) || interTmp.t > lightT){
			col += std::abs(inter.norm * lightDir) * l.col;
		}
	}
	
	if (nbRecur > 0 && indirect){
		Vec3 randDir;
		double r1 = unif(engine);
		double r2 = unif(engine);
		double msqrt = sqrt(1. - r2);
		randDir.x = cos(2. * M_PI * r1) * msqrt;
		randDir.y = sin(2. * M_PI * r1) * msqrt;
		randDir.z = sqrt(r2);
		
		Vec3 x, y, z;
		z = inter.norm;
		Vec3 randAxe(unif(engine)-0.5, unif(engine)-0.5, unif(engine)-0.5);
		x = z ^ randAxe;
		x.normalize();
		
		y = x ^ z;
		
		Vec3 realDir = normalize(x * randDir.x + y * randDir.y + z * randDir.z);
		
		Ray r(inter.pos + MYEPS * inter.norm, realDir);
		Intersection interTmp;
		col += s.getCol(r, indirect, nbRecur-1);
	}
	
	col = mult(col, color);
	
	return col;
	
}

SpecularMat::SpecularMat(){}

SpecularMat::SpecularMat(const Vec3& dc, const Vec3& sc) : diff(dc){
	specCol = sc;
}

Vec3 SpecularMat::getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const{
	Vec3 col;
	
	if (nbRecur > 0){
		Intersection tmpInter;
		Ray ref(inter.pos + MYEPS * inter.norm, reflect( - inter.fromDir, inter.norm));
		col = mult(specCol, s.getCol(ref, indirect, nbRecur - 1));
	}
	
	col += diff.getCol(s, inter, indirect, nbRecur - 1);
	
	return col;
}

TransparentMat::TransparentMat(){}

TransparentMat::TransparentMat(double mn){
	n = mn;
}

Vec3 TransparentMat::getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const {
	
	Vec3 col;
	if (nbRecur > 0){
		Vec3 newNorm;
		Vec3 refDir = refract(-inter.fromDir, inter.norm, N_AIR, n, newNorm);
		
		Ray ref (inter.pos - MYEPS * newNorm, refDir);
		
		col = s.getCol(ref, indirect, nbRecur - 1);
	}
	
	return col;
	
}





















