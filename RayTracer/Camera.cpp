//
//  Camera.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cmath>
#include "Camera.hpp"

Camera::Camera(const Vec3& p, const Vec3& d, const Vec3& u, int w, int h, double f){
	pos = p;
	dir = normalize(d);
	up = normalize(u);
	width = w;
	height = h;
	fov = f;
	right = dir ^ up;
}

Camera::Camera(const Camera& c){
	pos = c.pos;
	dir = c.dir;
	up = c.up;
	width = c.width;
	height = c.height;
	fov = c.fov;
	right = c.right;
}


Ray Camera::cast_Ray(int i, int j) const{
	
	Vec3 d(i - width * 0.5 + 0.5, j - height * 0.5 + 0.5, -height / (2. * tan((fov * (M_PI / 180.)) / 2.)));
	
	return Ray(pos, d.x * right + d.y * up - d.z * dir);
}
