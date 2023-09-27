#pragma once
#include "Vec3D.h"
#include "Ray.h"
#include "RGB_Pixel.h"
#include "Interval.h"

struct HitReccord {
	Vec3D point;
	Vec3D normal;
	double t;
	bool front_face;

	void set_face_normal(const Ray& ray, const Vec3D& vec);
};

class Shape {
public:
	virtual bool hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec) = 0;
	virtual ~Shape() = default;
};


class Sphere: public Shape
{
public:
	Sphere(Vec3D translation, double radius);

	bool hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec) override;

private:
	Vec3D m_translation;
	double m_radius;
};

