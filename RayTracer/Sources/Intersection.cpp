
//
//  Intersection.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 23/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include "Vec3.hpp"
#include "Intersection.hpp"

Intersection::Intersection(){}

Intersection::Intersection(const Intersection& i){
	t = i.t;
	pos = i.pos;
	norm = i.norm;
	obj = i.obj;
	fromDir = i.fromDir;
}

Intersection::Intersection( double mt, const Vec3& p, const Vec3& n, const Vec3& d, Object* o ){
	t = mt;
	pos = p;
	norm = n;
	fromDir = d;
	obj = o;
}
