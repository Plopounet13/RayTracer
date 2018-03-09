//
//  BVH.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 25/01/2018.
//  Copyright © 2018 Loïs Paulin. All rights reserved.
//

#include <algorithm>
#include "BVH.hpp"
#include "Mesh.hpp"

#define MY_EPS 0.0001

struct predicat
{
	int axe;
	double coupe;
	
	predicat( const int _axe, double _coupe ) : axe(_axe), coupe(_coupe) {}
	
	bool operator() ( const Primitive& p ) const
	{
		if (axe == 0)
			return p.center.x < coupe;
		else if (axe == 1)
			return p.center.y < coupe;
		else if (axe == 2)
			return p.center.z < coupe;
		return false;
	}
};

Vec3 mid(const Vec3& a, const Vec3& b, const Vec3& c){
	return (a + b + c)/3;
}

Primitive::Primitive(const Vec3& a, const Vec3& b, const Vec3& c, int triangle) : box(min(a,min(b,c)), max(a, max(b,c))), center(mid(a,b,c)), triangle(triangle) {}

int BVH::build_node(int begin, int end) {
	if(end - begin <= 1)
	{
		nodes.emplace_back(primitives[begin].box, primitives[begin].triangle);
		return int(nodes.size()) - 1;
	}
	
	BBox globox = primitives[begin].box;
	
	for (int i = begin+1; i < end; ++i){
		globox.min = min(globox.min, primitives[i].box.min);
		globox.max = max(globox.max, primitives[i].box.max);
	}
	
	Vec3 sizeBox = globox.max - globox.min;
	
	int axe;
	float coupe;
	
	if (sizeBox.x > sizeBox.y && sizeBox.x > sizeBox.z){
		axe = 0;
		coupe = (globox.max.x + globox.min.x) / 2;
	} else if (sizeBox.y > sizeBox.z) {
		axe = 1;
		coupe = (globox.max.y + globox.min.y) / 2;
	} else {
		axe = 2;
		coupe = (globox.max.z + globox.min.z) / 2;
	}

	Primitive *pmid = std::partition(primitives.data() + begin, primitives.data() + end, predicat(axe, coupe));
	int mid= int(std::distance(primitives.data(), pmid));
	
	if (mid == begin || mid == end){
		mid = (begin + end) / 2;
	}
	
	int left = build_node(begin, mid);
	
	int right = build_node(mid, end);
	
	nodes.emplace_back(globox, left, right);
	return int(nodes.size())-1;
	
}

BVH::BVH(){
	racine = -1;
}

BVH::BVH(const std::vector<int>& faces, const std::vector<Vec3>& sommets, Mesh* m){
	mesh = m;
	primitives.reserve(faces.size() / 3);
	
	for (int i = 0; i < faces.size() / 3; ++i){
		primitives.emplace_back(sommets[faces[3*i+0]], sommets[faces[3*i+1]], sommets[faces[3*i+2]], i);
	}
	
	racine = build_node(0, int(primitives.size()));
	
}

bool BVH::intersect(const Ray& r, double tmax) const{
	return recurIntersect(r, tmax, nodes[racine]);
}

bool BVH::intersect(const Ray& r, Intersection& inter, double tmax) const{
	
	return recurIntersect(r, inter, tmax, nodes[racine]);
	
}

//Tests if ray intersect the scene without needing any info on intersection
bool BVH::recurIntersect(const Ray& r, const double& tmax, const Node& act) const{
	
	//Leaf
	if (act.left == -1){
		return mesh->intersectTriangle(r, act.primitive, tmax);
	}
	
	double tL1, tL2, tR1, tR2;
	
	bool interBoxL = nodes[act.left].box.intersect(r, tL1, tL2, tmax);
	bool interBoxR = nodes[act.right].box.intersect(r, tR1, tR2, tmax);
	
	//Checking boxes in order does not matter for this intersection testing
	if (interBoxL && recurIntersect(r, tL2 + MY_EPS, nodes[act.left]))
		return true;
	else if (interBoxR && recurIntersect(r, tR2 + MY_EPS, nodes[act.right]))
		return true;
	else
		return false;
	
}

bool BVH::recurIntersect(const Ray& r, Intersection& inter, const double& tmax, const Node& act) const{
	
	//Leaf
	if (act.left == -1){
		return mesh->intersectTriangle(r, act.primitive, inter, tmax);
	}
	
	double tL1, tL2, tR1, tR2;
	
	bool interBoxL = nodes[act.left].box.intersect(r, tL1, tL2, tmax);
	bool interBoxR = nodes[act.right].box.intersect(r, tR1, tR2, tmax);
	bool interL = false;
	bool interR = false;
	
	//If left closer than right
	if (tL1 < tR1){
		if (interBoxL){
			interL = recurIntersect(r, inter, tL2 + MY_EPS, nodes[act.left]);
			if (interL)
				tR2 = std::min(tR2, inter.t);
		}
		if (interBoxR){
			interR = recurIntersect(r, inter, tR2 + MY_EPS, nodes[act.right]);
		}
	} else {
		if (interBoxR){
			interR = recurIntersect(r, inter, tR2 + MY_EPS, nodes[act.right]);
			if (interR)
				tL2 = std::min(tL2, inter.t);
		}
		if (interBoxL){
			interL = recurIntersect(r, inter, tL2 + MY_EPS, nodes[act.left]);
		}
	}
	
	return interL || interR;
	
}





















