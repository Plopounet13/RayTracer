//
//  Triangle.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 07/03/2018.
//  Copyright © 2018 Loïs Paulin. All rights reserved.
//

#include "Triangle.hpp"

Triangle::Triangle(int ind, Mesh* m): ind(ind), m(m){}

BBox Triangle::getBoundingBox(){
	const Vec3& a = m->vertices[m->faces[3 * ind + 0].vertex];
	const Vec3& b = m->vertices[m->faces[3 * ind + 1].vertex];
	const Vec3& c = m->vertices[m->faces[3 * ind + 2].vertex];
	return BBox(min(a, min(b, c)), max(a, max(b, c)));
}

bool Triangle::intersect(const Ray& r, double tmax) const {
	return m->intersectTriangle(r, ind, tmax);
}

bool Triangle::intersect(const Ray& r, Intersection& inter, double tmax) const{
	return m->intersectTriangle(r, ind, inter, tmax);
}
