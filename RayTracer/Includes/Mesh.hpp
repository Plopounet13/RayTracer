//
//  Mesh.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 30/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <string>
#include "BVH.hpp"
#include "Object.hpp"

class indices {
public:
	int vertex;
	int normal;
	int texCoords;
	indices(int v, int n, int t):vertex(v), normal(n), texCoords(t){}
	indices(){}
};

class Mesh : public Object{
	friend class Triangle;
	
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<indices> faces;
	std::vector<Vec3> smoothnormals;
	std::vector<std::pair<double, double>> texCoords;
	
	BVH m_bvh;
	
	void readFromObj(const std::string& filename);
	void recomputeNormals();
	bool isTrigo(int a, int b, int c);
	
public:
	bool intersectTriangle(const Ray &ray, int i, double tmax = DBL_MAX) const;
	bool intersectTriangle(const Ray &ray, int i, Intersection& inter, double tmax = DBL_MAX) const;
	Mesh();
	Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec3>& normals, const std::vector<indices>& faces, Material* m);
	Mesh(const std::string& filename, double scale, const Vec3& loc, const Vec3& newX, const Vec3& newY, const Vec3& newZ, Material* m);
	
	bool intersect(const Ray& r, double tmax = DBL_MAX) const;
	bool intersect(const Ray& r, Intersection& inter, double tmax = DBL_MAX) const;
	BBox getBoundingBox() const;
	
};

#endif /* Triangle_hpp */
