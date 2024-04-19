#pragma once
#include "Vec3D.h"
#include "Ray.h"
#include "RGB_Pixel.h"
#include "Interval.h"
#include "Material.h"

typedef struct HitRecord {
	Vec3D point;
	Vec3D normal;
	double t;
	const Material* material;
	bool front_face;

	void set_face_normal(const Ray& ray, const Vec3D& vec);
}HitRecord;

class Shape {
public:
	virtual bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const = 0;
	virtual ~Shape() = default;
};


class Sphere: public Shape
{
public:
	Sphere(const Vec3D& translation, double radius, const Material* mat);

	virtual bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const override;

private:
	Vec3D const m_translation;
	double const m_radius;
	const Material* const m_material;
};

class Triangle : public Shape {
public:
	Triangle(const Vec3D& v0, const Vec3D& v1, const Vec3D& v2, const Material* mat);

	virtual bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const override;

private:
	static Vec3D generate_normal(const Vec3D& v0, const Vec3D& v1, const Vec3D& v2);
private:
	Vec3D const m_v0;
	Vec3D const m_v1;
	Vec3D const m_v2;
	Vec3D const m_normal;
	const Material* const m_material;
};
