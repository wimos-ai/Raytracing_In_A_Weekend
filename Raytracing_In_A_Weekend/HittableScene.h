#pragma once
#include <vector>
#include "Shapes.h"


class HittableScene: public std::vector<Shape*>, public Shape
{
public:
	bool hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec) override;
};

