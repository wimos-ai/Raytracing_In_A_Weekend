#pragma once
#include "Vec3D.h"
#include "Ray.h"


class Shape {
public:
	virtual bool hit(const Ray& ray) = 0;
	virtual ~Shape() = default;
};


class Sphere: public Shape
{
public:
	Sphere(Vec3D translation, double radius);

	bool hit(const Ray& ray);

private:
	Vec3D m_translation;
	double m_radius;
};

