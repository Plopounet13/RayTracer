//
//  Image.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <string>
#include <cstring>
#include "Vec3.hpp"

class Image{
	
public:
	float* data;
	int width;
	int height;
	
	Image(int w, int h);
	Image(const Image& i);
	~Image();
	float& red(int i, int j);
	float& green(int i, int j);
	float& blue(int i, int j);
	float red(int i, int j) const;
	float green(int i, int j) const;
	float blue(int i, int j) const;
	void store(int i, int j, const Vec3& v);
	void save(const std::string& filename) const;
};

#endif /* Image_hpp */
