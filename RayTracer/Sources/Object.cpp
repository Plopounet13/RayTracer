//
//  Object.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 29/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include "Object.hpp"


Vec3 Object::getCol(const Scene& s, const Intersection& inter, bool indirect, int nbRecur) const{
	return mat->getCol(s, inter, indirect, nbRecur);
}


Object::Object(){
	mat = nullptr;
}

Object::Object(Material* m){
	mat = m;
}

Object::~Object(){
	delete mat;
}
