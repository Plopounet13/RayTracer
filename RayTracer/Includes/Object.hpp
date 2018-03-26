//
//  Object.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <cfloat>
#include "Material.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "BBox.hpp"

class Scene;

class Object{
public:
	Material* mat;
	Object();
	Object(Material* mat);
	virtual ~Object();
	
	virtual bool intersect(const Ray& r, double tmax = DBL_MAX) const = 0;
	virtual bool intersect(const Ray& r, Intersection& inter, double tmax = DBL_MAX) const  = 0;
	virtual Vec3 getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const;
	virtual BBox getBoundingBox() const = 0;
};

#endif /* Object_hpp */
