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
#include "BVH.hpp"
#include "Object.hpp"

class Mesh : public Object{
	
	std::vector<Vec3> sommets;
	std::vector<Vec3> normales;
	std::vector<int> faces;
	std::vector<int> normalesFace;
	std::vector<Vec3> smoothNormales;
	
	BVH m_bvh;
	
	void readFromObj(const std::string& filename);
	void recomputeNormals();
	bool isTrigo(int a, int b, int c);
	
public:
	bool intersectTriangle(const Ray &ray, int i, double tmax = DBL_MAX) const;
	bool intersectTriangle(const Ray &ray, int i, Intersection& inter, double tmax = DBL_MAX) const;
	Mesh();
	Mesh(const std::vector<Vec3>& sommets, const std::vector<Vec3>& normales, const std::vector<int>& faces, const std::vector<int>& normalesFace, Material* m);
	Mesh(const std::string& filename, double scale, const Vec3& loc, const Vec3& newX, const Vec3& newY, const Vec3& newZ, Material* m);
	
	bool intersect(const Ray& r, double tmax = DBL_MAX) const;
	bool intersect(const Ray& r, Intersection& inter, double tmax = DBL_MAX) const;
	
};

#endif /* Triangle_hpp */
