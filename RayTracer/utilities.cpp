//
//  utilities.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 08/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cmath>
#include "utilities.hpp"


Vec3 reflect(const Vec3& dir, const Vec3& norm){
	return dir - 2 * (dir * norm) * norm;
}

Vec3 refract(const Vec3& i, const Vec3& norm, double n1, double n2, Vec3& n){
	double ndotd = norm * i;
	double rappN;

	// ndotd < 0 => On rentre dans l'objet
	if (ndotd < 0){
		rappN = n1 / n2;
		n = norm;
	} else {
		ndotd = -ndotd;
		rappN = n2 / n1;
		n = -norm;
	}

	double transFact = 1. - rappN * rappN * (1. - ndotd * ndotd);

	Vec3 refDir;

	//Si racine complexe alors réfléchi
	if (transFact < 0) {
		refDir = reflect(i, norm);
		n = -n;
	} else {
		refDir = (rappN * i) - ((rappN * ndotd + sqrt(transFact)) * n);
	}
	
	return refDir;
}
