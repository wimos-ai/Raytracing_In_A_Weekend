#include "Shapes.h"

Sphere::Sphere(Vec3D translation, double radius): m_translation(translation), m_radius(radius)
{
}

bool Sphere::hit(const Ray& ray)
{
    Vec3D oc = ray.origin() - this->m_translation;
    auto a = ray.direction().dot(ray.direction());
    auto b = 2.0 * ray.direction().dot(oc);
    auto c = oc.dot(oc) - m_radius * m_radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}
