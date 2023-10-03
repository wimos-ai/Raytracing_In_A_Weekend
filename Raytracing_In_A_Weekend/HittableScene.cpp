#include "HittableScene.h"

bool HittableScene::hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const
{
	double closest_so_far = ray_interval.max();
	bool hit_something = false;

	for (const Shape* item : *this)
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

void HittableScene::take_ownership(void* obj)
{
	m_owned_objs.push_back(obj);
}

HittableScene::~HittableScene()
{
	for (size_t i = 0; i < m_owned_objs.size(); i++)
	{
		free(m_owned_objs[i]);
	}
}
