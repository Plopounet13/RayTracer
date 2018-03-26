//
//  utilities.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 08/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef utilities_hpp
#define utilities_hpp

#include <algorithm>
#include "Vec3.hpp"

template <typename T>
inline T clamp(const T& v, const T& mini, const T& maxi){
	return std::min(maxi, std::max(mini, v));
}

Vec3 reflect(const Vec3& dir, const Vec3& norm);
Vec3 refract(const Vec3& i, const Vec3& norm, double n1, double n2, Vec3& usedNorm);

#endif /* utilities_hpp */
