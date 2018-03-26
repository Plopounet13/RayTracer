//
//  Ray.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include "Ray.hpp"


Ray::Ray(){}

Ray::Ray(const Vec3& orig, const Vec3& dir){
	origine = orig;
	direction = dir;
	direction.normalize();
}
