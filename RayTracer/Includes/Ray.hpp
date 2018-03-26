//
//  Ray.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "Vec3.hpp"

class Ray{
public:
	Vec3 origine;
	Vec3 direction;
	
	Ray();
	Ray(const Vec3& orig, const Vec3& dir);
};

#endif /* Ray_hpp */
