#pragma once
#include "Vec3D.h"
#include "Ray.h"
#include "RGB_Pixel.h"
#include "Interval.h"
#include "Material.h"

typedef struct HitReccord {
	Vec3D point;
	Vec3D normal;
	double t;
	Material* material;
	bool front_face;

	void set_face_normal(const Ray& ray, const Vec3D& vec);
}HitReccord;

class Shape {
public:
	virtual bool hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec) const = 0;
	virtual ~Shape() = default;
};


class Sphere: public Shape
{
public:
	Sphere(Vec3D translation, double radius, Material* mat);

	virtual bool hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec) const override;

private:
	Vec3D m_translation;
	double m_radius;
	Material* m_material;
};

