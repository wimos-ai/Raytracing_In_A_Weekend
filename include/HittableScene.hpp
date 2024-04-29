#pragma once
#include <vector>
#include <memory>
#include "Shapes.hpp"
#include "Ownable.hpp"


class HittableScene: public Shape
{
public:
	bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const override;

	inline void add_shape(std::unique_ptr<Shape>&& obj) { m_shapes.push_back(std::move(obj));}

	inline void take_ownership(std::unique_ptr<Ownable>&& obj) {m_owned_objs.push_back(std::move(obj));}

	HittableScene() = default;

	HittableScene(HittableScene&& other) = default;

	~HittableScene() override = default;
private:
	std::vector<std::unique_ptr<Ownable>> m_owned_objs;
	std::vector<std::unique_ptr<Shape>> m_shapes;
};

