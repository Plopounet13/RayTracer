//
//  Mesh.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 30/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <vector>
#include <string>
#include "Object.hpp"

class Mesh : public Object{
	
	std::vector<Vec3> sommets;
	std::vector<Vec3> normales;
	std::vector<int> faces;
	std::vector<int> normalesFace;
	std::vector<Vec3> smoothNormales;
	
	void readFromObj(const std::string& filename);
	bool intersectTriangle(const Ray &ray, int i, Intersection& inter) const;
	bool intersectTriangle(const Ray &ray, int i) const;
	bool isTrigo(int a, int b, int c);
	
public:
	Mesh();
	Mesh(const std::vector<Vec3>& sommets, const std::vector<Vec3>& normales, const std::vector<int>& faces, const std::vector<int>& normalesFace, Material* m);
	Mesh(const std::string& filename, double scale, const Vec3& loc, const Vec3& newX, const Vec3& newY, const Vec3& newZ, Material* m);
	
	bool intersect(const Ray& r) const;
	bool intersect(const Ray& r, Intersection& inter) const;
	
	
};

#endif /* Triangle_hpp */
