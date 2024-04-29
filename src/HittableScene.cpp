#include "HittableScene.hpp"

bool HittableScene::hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const
{
	double closest_so_far = ray_interval.max();
	bool hit_something = false;

	for (const std::unique_ptr<Shape>& item : m_shapes)
	{
		HitRecord tmp_rec;
		if (item->hit(ray, ray_interval, tmp_rec)) {
			hit_something = true;
			if (tmp_rec.t  < closest_so_far)
			{
				closest_so_far = tmp_rec.t;
				rec = tmp_rec;
			}
		}
	}
	return hit_something;
}
