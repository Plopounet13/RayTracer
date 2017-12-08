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
	
	void readFromObj(const std::string& filename);
	bool intersectTriangle(const Ray &ray, int i, Intersection& inter) const;
	bool intersectTriangle(const Ray &ray, int i) const;
	
public:
	Mesh();
	Mesh(const std::vector<Vec3>& sommets, const std::vector<Vec3>& normales, const std::vector<int>& faces, Material* m);
	Mesh(const std::string& filename, Material* m);
	
	bool intersect(const Ray& r) const;
	bool intersect(const Ray& r, Intersection& inter) const;
	
	
};

#endif /* Triangle_hpp */
