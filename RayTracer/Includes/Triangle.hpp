//
//  Triangle.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 07/03/2018.
//  Copyright © 2018 Loïs Paulin. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include "Object.hpp"
#include "Mesh.hpp"
#include "BBox.hpp"

class Triangle : public Object {
	int ind;
	Mesh* m;
	
public:
	Triangle(int ind, Mesh* m);
	BBox getBoundingBox();
	bool intersect(const Ray& r, double tmax = DBL_MAX) const;
	bool intersect(const Ray& r, Intersection& inter, double tmax = DBL_MAX) const;
};

#endif /* Triangle_hpp */
