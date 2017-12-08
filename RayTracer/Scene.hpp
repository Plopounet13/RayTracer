//
//  Scene.hpp
//  RayTracer
//
//  Created by Loïs Paulin on 21/11/2017.
//  Copyright © 2017 Loïs Paulin. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <vector>
#include <string>
#include "Light.hpp"
#include "Object.hpp"
#include "Camera.hpp"
#include "Intersection.hpp"

#define NB_RECUR 5

class Scene {
	
public:
	std::vector<Light> lights;
	std::vector<Object*> objects;
	Camera c;
	Vec3 skyColor;
	
	Scene();
	Scene(const Scene& s);
	Scene(const std::vector<Light>& l, const std::vector<Object*>& o, const Camera& cam, const Vec3& sky);
	~Scene();
	
	bool intersect(const Ray& r) const;
	bool intersect(const Ray& r, Intersection& inter) const;
	
	Vec3 getCol(const Ray& r, bool indirect, int nbRecur) const;
	
	void render(std::string filename, int nbRay, int nbRecur = NB_RECUR) const;
};

#endif /* Scene_hpp */
