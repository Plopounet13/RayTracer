//
//  Intersection.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 23/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Intersection_hpp
#define Intersection_hpp

#include "Vec3.hpp"
class Object;

class Intersection {
public:
	Vec3 pos;
	Vec3 norm;
	Vec3 fromDir;
	const Object* obj;
	double t;
	
	
	Intersection();
	Intersection(const Intersection& i);
	Intersection( double t, const Vec3& pos, const Vec3& norm, const Vec3& fromDir, Object* obj );
	
};


#endif /* Intersection_hpp */
