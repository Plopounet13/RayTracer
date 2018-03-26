//
//  Light.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 20/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include "Light.hpp"

Light::Light(){}

Light::Light(const Vec3& p, const Vec3& c){
	pos = p;
	col = c;
}
