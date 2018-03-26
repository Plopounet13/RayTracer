//
//  BBox.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 25/01/2018.
//  Copyright © 2018 Loïs Paulin. All rights reserved.
//

#ifndef BBox_hpp
#define BBox_hpp

#include <cfloat>
#include "Vec3.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

class BBox {
	
public:
	Vec3 min;
	Vec3 max;
	BBox();
	BBox(const BBox& b);
	BBox(const Vec3& min, const Vec3& max);
	bool intersect(const Ray& r, double& t1, double& t2, double tmax = DBL_MAX) const;
	
};

#endif /* BBox_hpp */
