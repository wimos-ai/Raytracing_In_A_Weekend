#pragma once
#include "Vec3D.h"
#include "Ray.h"
#include "RGB_Pixel.h"

struct HitReccord {
	Vec3D point;
	Vec3D normal;
	double t;
};

class Shape {
public:
	virtual bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitReccord& rec) = 0;
	virtual ~Shape() = default;
};


class Sphere: public Shape
{
public:
	Sphere(Vec3D translation, double radius);

	bool hit(const Ray& ray, double ray_tmin, double ray_tmax, HitReccord& rec);

private:
	Vec3D m_translation;
	double m_radius;
};

