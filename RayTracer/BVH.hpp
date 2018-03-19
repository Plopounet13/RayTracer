//
//  BVH.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 25/01/2018.
//  Copyright © 2018 Loïs Paulin. All rights reserved.
//

#ifndef BVH_hpp
#define BVH_hpp

#include <vector>
#include "BBox.hpp"

class Mesh;
class indices;

class Node {
public:
	BBox box;
	int left;
	int right;
	int primitive;
	
	Node(const BBox& box, int l, int r) : box(box), left(l), right(r), primitive(-1){}
	Node(const BBox& box, int p) : box(box), left(-1), right(-1), primitive(p){}
};

class Primitive {
public:
	BBox box;
	Vec3 center;
	int triangle;
	
	Primitive(const Vec3& a, const Vec3& b, const Vec3& c, int triangle);
};

class BVH {
	Mesh* mesh;
	std::vector<Node> nodes;
	std::vector<Primitive> primitives;
	int racine;
	
	int build_node(const int begin, const int end);
	bool recurIntersect(const Ray& r, Intersection& inter, const double& tmax, const Node& act) const;
	bool recurIntersect(const Ray& r, const double& tmax, const Node& act) const;
	
public:
	BVH(); 
	BVH(const std::vector<indices>& faces, const std::vector<Vec3>& vertices, Mesh* m);
	
	bool intersect(const Ray& r, double tmax = DBL_MAX) const;
	bool intersect(const Ray& r, Intersection& inter, double tmax = DBL_MAX) const;
	
};

#endif /* BVH_hpp */
