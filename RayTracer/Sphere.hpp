//
//  Sphere.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "Vec3.hpp"
#include "Ray.hpp"
#include "Object.hpp"
#include "Material.hpp"
#include "Intersection.hpp"

class Sphere : public Object{
public:
	Vec3 center;
	double radius;
	
	Sphere();
	Sphere(const Vec3& c, double r, Material* m);
	
	bool intersect(const Ray& r, double tmax = DBL_MAX) const;
	bool intersect(const Ray& r, Intersection& inter, double tmax = DBL_MAX) const;
};

#endif /* Sphere_hpp */
