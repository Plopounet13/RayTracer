//
//  Vec3.cpp
//  Vector
//
//  Created by Loïs Paulin on 07/09/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include "Vec3.hpp"
#include <cmath>
#include <algorithm>


Vec3::Vec3(const Vec3& v){
	x = v.x;
	y = v.y;
	z = v.z;
}

Vec3::Vec3(double a, double b, double c){
	x = a;
	y = b;
	z = c;
}

Vec3::Vec3(){
	x = 0.;
	y = 0.;
	z = 0.;
}

Vec3 operator+ (const Vec3& a, const Vec3& b){
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3& operator+= (Vec3& a, const Vec3& b){
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vec3 operator- (const Vec3& a, const Vec3& b){
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

const Vec3& operator-= (Vec3& a, const Vec3& b){
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

double operator* (const Vec3& a, const Vec3& b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 operator^ (const Vec3& a, const Vec3& b){
	return Vec3(a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y);
}

double Vec3::norm2() const{
	return x*x + y*y + z*z;
}

double Vec3::norm() const{
	return sqrt(norm2());
}

void Vec3::normalize(){
	double n = norm();
	x /= n;
	y /= n;
	z /= n;
}

Vec3 normalize(const Vec3& v){
	Vec3 ret = v;
	ret.normalize();
	return ret;
}



Vec3 min(const Vec3& a, const Vec3& b){
	return  Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vec3 max(const Vec3& a, const Vec3& b){
	return  Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

double dist2(const Vec3& a, const Vec3& b){
	return (a - b).norm2();
}

double dist(const Vec3& a, const Vec3& b){
	return (a - b).norm();
}

Vec3 operator* (const double& s, const Vec3& v){
	return Vec3(s * v.x, s * v.y, s * v.z);
}

Vec3 operator* (const Vec3& v, const double& s){
	return s * v;
}

Vec3 operator/ (const Vec3& v, const double& s){
	return Vec3(v.x / s, v.y / s, v.z / s);
}

const Vec3& operator/= (Vec3& v, const double& s){
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

Vec3 operator- (const Vec3& v){
	return Vec3(-v.x, -v.y, -v.z);
}

Vec3 mult(const Vec3& a, const Vec3& b){
	return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}


