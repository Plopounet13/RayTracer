//
//  Camera.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Vec3.hpp"
#include "Ray.hpp"

class Camera {

public:
	Vec3 pos;
	Vec3 dir;
	Vec3 up;
	int width;
	int height;
	double fov;
	Vec3 right;
	
	Camera(const Vec3& p = Vec3(0, 0, 0), const Vec3& d = Vec3(0, 0, -1), const Vec3& u = Vec3(0, 1, 0), int w = 320, int h = 320, double f = 60.f);
	Camera(const Camera& c);
	
	Ray cast_Ray(int i, int j) const;
	
};

#endif /* Camera_hpp */
