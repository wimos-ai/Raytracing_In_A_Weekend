#pragma once
#include <vector>
#include <memory>
#include "Shapes.h"
#include "Ownable.hpp"


class HittableScene: public std::vector<std::unique_ptr<Shape>>, public Shape
{
public:
	bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const override;

	void take_ownership(std::unique_ptr<Ownable>&& obj);

	~HittableScene() override;
private:
	std::vector<std::unique_ptr<Ownable>> m_owned_objs;
};

