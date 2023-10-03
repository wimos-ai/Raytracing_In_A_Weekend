#pragma once
#include <vector>
#include "Shapes.h"


class HittableScene: public std::vector<Shape*>, public Shape
{
public:
	bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const override;

	void take_ownership(void* obj);

	~HittableScene();
private:
	std::vector<void*> m_owned_objs;
};

