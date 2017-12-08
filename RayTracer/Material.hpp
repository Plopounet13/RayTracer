//
//  Material.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 22/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "Intersection.hpp"
#include "Vec3.hpp"

#define N_AIR 1.

class Scene;

class Material {
public:
	virtual ~Material();
	virtual Vec3 getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const = 0;
	
};

class DiffuseMat : public Material {
	Vec3 color;
public:
	DiffuseMat();
	DiffuseMat(const Vec3& c);
	Vec3 getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const;
	
};

class SpecularMat : public Material {
	DiffuseMat diff;
	Vec3 specCol;
public:
	SpecularMat();
	SpecularMat(const Vec3& diffCol, const Vec3& specCol);
	Vec3 getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const;
	
};

class TransparentMat : public Material {
	double n;
public:
	TransparentMat();
	TransparentMat(double n);
	Vec3 getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const;
	
};

#endif /* Material_hpp */
