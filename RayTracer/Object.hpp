//
//  Object.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "Material.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

class Scene;

class Object{
public:
	Material* mat;
	Object();
	Object(Material* mat);
	virtual ~Object();
	virtual bool intersect(const Ray& r) const = 0;
	virtual bool intersect(const Ray& r, Intersection& inter) const  = 0;
	virtual Vec3 getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const;
};

#endif /* Object_hpp */