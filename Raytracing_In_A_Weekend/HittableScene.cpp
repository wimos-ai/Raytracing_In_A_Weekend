#include "HittableScene.h"

bool HittableScene::hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec) const
{
	auto closest_so_far = ray_interval.max();
	bool hit_something = false;

	for (const auto& item : *this)
	{
		HitReccord tmp_rec;
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
