//
//  Light.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 20/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include "Vec3.hpp"

class Light {
public:
	Vec3 pos;
	Vec3 col;
	
	Light();
	Light(const Vec3& pos, const Vec3& col);
	
	
};

#endif /* Light_hpp */
