#include "Shapes.h"
#include <cassert>

void HitReccord::set_face_normal(const Ray& ray, const Vec3D& vec) {
	front_face = ray.direction().dot(vec) < 0;
	normal = front_face ? vec : -1 * vec;
	//assert(vec.unit_vec() == vec);
}

Sphere::Sphere(Vec3D translation, double radius) : m_translation(translation), m_radius(radius)
{
}

bool Sphere::hit(const Ray& ray, const Interval& ray_interval, HitReccord& rec)
{
	const Vec3D oc = ray.origin() - this->m_translation;
	const double a = ray.direction().length_squared();
	const double half_b = ray.direction().dot(oc);
	const double c = oc.length_squared() - m_radius * m_radius;

	const double discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return -false;
	}

	const double sqrt_disc = std::sqrt(discriminant);

	double root = (-half_b - sqrt_disc) / a;
	if (!ray_interval.surrounds(root)) {
		root = (-half_b + sqrt_disc) / a;
		if (!ray_interval.surrounds(root))
			return false;
	}



	rec.t = root;
	rec.point = ray.at(root);
	rec.normal = ((rec.point - m_translation) / m_radius).unit_vec();
	rec.set_face_normal(ray, rec.normal);

	return true;
}

