//
//  Vec3.h
//  Vec3
//
//  Created by Loïs Paulin on 07/09/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Vector_hpp
#define Vector_hpp

/**
 *
 **/
class Vec3{
	
public:
	double x;
	double y;
	double z;
	
	Vec3(double a, double b, double c);
	Vec3(const Vec3& v);
	Vec3();
	
	double norm2() const;
	double norm() const;
	void normalize();
	friend Vec3 normalize(const Vec3& v);
	
	void rotate(const Vec3& rot);
	
	friend double dist2(const Vec3& a, const Vec3& b);
	friend double dist(const Vec3& a, const Vec3& b);
	
	friend Vec3 min(const Vec3& a, const Vec3& b);
	friend Vec3 max(const Vec3& a, const Vec3& b);
	
	friend Vec3 operator+ (const Vec3& a, const Vec3& b);
	friend Vec3& operator+= (Vec3& a, const Vec3& b);
	
	friend Vec3 operator- (const Vec3& a, const Vec3& b);
	friend const Vec3& operator-= (Vec3& a, const Vec3& b);
	
	friend Vec3 operator^ (const Vec3& a, const Vec3& b);
	friend double operator* (const Vec3& a, const Vec3& b);
	
	friend Vec3 operator* (const double& s, const Vec3& v);
	friend Vec3 operator* (const Vec3& v, const double& s);
	friend const Vec3& operator*= (Vec3& v, const double& s);
	
	friend Vec3 operator/ (const Vec3& v, const double& s);
	friend const Vec3& operator/= (Vec3& v, const double& s);
	
	friend Vec3 operator- (const Vec3& v);
	
	friend Vec3 mult(const Vec3& a, const Vec3& b);
	
	friend bool operator== (const Vec3& a, const Vec3& b);
	friend bool operator!= (const Vec3& a, const Vec3& b);
	
};


#endif /* Vector_hpp */
