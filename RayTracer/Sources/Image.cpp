//
//  Image.cpp
//  RayTracer
//
//  Created by Loïs Paulin on 04/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#include <cstdlib>
#include <fstream>
#include <cmath>
#include "utilities.hpp"
#include "Image.hpp"

typedef unsigned char uchar;

Image::Image(int w, int h){
	width = w;
	height = h;
	data = (float*)malloc(3 * w * h * sizeof(float));
	memset(data, 0, 3 * w * h * sizeof(float));
}

Image::Image(const Image& i){
	width = i.width;
	height = i.height;
	data = (float*)malloc(3 * width * height * sizeof(float));
	memcpy(data, i.data, 3 * width * height * sizeof(float));
};

Image::~Image(){
	free(data);
}

float& Image::red(int i, int j){
	return data[3 * (i + j * width) + 0];
}
float& Image::green(int i, int j){
	return data[3 * (i + j * width) + 1];
}
float& Image::blue(int i, int j){
	return data[3 * (i + j * width) + 2];
}

float Image::red(int i, int j) const{
	return data[3 * (i + j * width) + 0];
}
float Image::green(int i, int j) const{
	return data[3 * (i + j * width) + 1];
}
float Image::blue(int i, int j) const{
	return data[3 * (i + j * width) + 2];
}

void Image::store(int i, int j, const Vec3& v){
	red(i, j) = v.x;
	green(i, j) = v.y;
	blue(i, j) = v.z;
}

float gammaCorrection(float v){
	static float gamma = 1.f / 2.2f;
	return std::pow(v, gamma);
}

void Image::save(const std::string& filename) const{
	
	unsigned char *img = NULL;
	int filesize = 54 + 3 * width * height;
	
	img = (uchar*)malloc(3 * width * height);
	
	for(int i = 0; i < width; ++i) {
		for(int j = 0; j < height; ++j) {
			int x = i;
			int y = (height-1) - j;
			float r = clamp(gammaCorrection(red(i, j)) * 255, 0.f, 255.f);
			float g = clamp(gammaCorrection(green(i, j)) * 255, 0.f, 255.f);
			float b = clamp(gammaCorrection(blue(i, j)) * 255, 0.f, 255.f);
			img[(x + y * width) * 3 + 2] = (unsigned char)(r);
			img[(x + y * width) * 3 + 1]  = (unsigned char)(g);
			img[(x + y * width) * 3 + 0]  = (unsigned char)(b);
		}
	}
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)( width );
	bmpinfoheader[ 5] = (unsigned char)( width >> 8);
	bmpinfoheader[ 6] = (unsigned char)( width >> 16);
	bmpinfoheader[ 7] = (unsigned char)( width >> 24);
	bmpinfoheader[ 8] = (unsigned char)( height );
	bmpinfoheader[ 9] = (unsigned char)( height >> 8);
	bmpinfoheader[10] = (unsigned char)( height >> 16);
	bmpinfoheader[11] = (unsigned char)( height >> 24);
	
	FILE* f = fopen(filename.c_str(), "w");
	
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	
	for(int i = 0; i < height; ++i) {
		fwrite(img + ( width * (height - i - 1) * 3), 3, width, f);
		fwrite(bmppad , 1, (4 - (width * 3) % 4) % 4, f);
	}
	
	free(img);
	fclose(f);
	
}
