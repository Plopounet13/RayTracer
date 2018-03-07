//
//  BBox.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 25/01/2018.
//  Copyright © 2018 Loïs Paulin. All rights reserved.
//

#include <algorithm>
#include "BBox.hpp"


BBox::BBox(){}

BBox::BBox(const BBox& b) : min(b.min), max(b.max) {}

BBox::BBox(const Vec3& min, const Vec3& max) : min(min), max(max){}

bool BBox::intersect(const Ray& r, double& t1, double& t2, double tmax) const{
	Vec3 invd (1.f / r.direction.x, 1.f / r.direction.y, 1.f / r.direction.z);
	// remarque : il est un peu plus rapide de stocker invd dans la structure Ray, ou dans l'appellant / algo de parcours, au lieu de la recalculer à chaque fois
	
	Vec3 rmin = min;
	Vec3 rmax = max;
	// le rayon entre dans la bbox par pmax et ressort par pmin, echanger...
	if(r.direction.x < 0) std::swap(rmin.x, rmax.x);
	if(r.direction.y < 0) std::swap(rmin.y, rmax.y);
	if(r.direction.z < 0) std::swap(rmin.z, rmax.z);
	
	Vec3 dmin = mult((rmin - r.origine), invd);        // intersection avec les plans -U -V -W attachés à rmin
	Vec3 dmax = mult((rmax - r.origine), invd);        // intersection avec les plans +U +V +W attachés à rmax
	t1 = std::max(dmin.x, std::max(dmin.y, std::max(dmin.z, 0.)));        // borne min de l'intervalle d'intersection
	t2 = std::min(dmax.x, std::min(dmax.y, std::min(dmax.z, tmax)));    // borne max
									
	// ne renvoie vrai que si l'intersection est valide (l'intervalle n'est pas degenere)
	return (t1 <= t2);
	
}
