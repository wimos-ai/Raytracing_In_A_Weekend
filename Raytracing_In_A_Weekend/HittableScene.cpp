#include "HittableScene.h"

bool HittableScene::hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec)
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


//	if (rec.t > 0.0) {
//Vec3D N = (ray.at(rec.t) - Vec3D(0, 0, -1)).unit_vec();
//return RGB_Pixel(0.5 * Vec3D(N.x() + 1, N.y() + 1, N.z() + 1));
//		}