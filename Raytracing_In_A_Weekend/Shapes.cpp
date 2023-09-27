#include "Shapes.h"

Sphere::Sphere(Vec3D translation, double radius): m_translation(translation), m_radius(radius)
{
}

bool Sphere::hit(const Ray& ray, double ray_tmin, double ray_tmax, HitReccord& rec)
{
    const Vec3D oc = ray.origin() - this->m_translation;
    const double a = ray.direction().length_squared();
    const double half_b = ray.direction().dot(oc);
    const double c = oc.length_squared() - m_radius * m_radius;

    const double discriminant = half_b * half_b -  a * c;

    if (discriminant < 0) {
        return -false;
    }
    
    const double sqrt_disc = std::sqrt(discriminant);

    double root = (-half_b - sqrt_disc) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (-half_b + sqrt_disc) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return false;
    }

    rec.t = root;
    rec.point = ray.at(root);
    rec.normal = (rec.point - m_translation) / m_radius;

    return true;
}

//RGB_Pixel Sphere::get_color(const Ray& ray, double discriminant)
//{
//    Vec3D n = (ray.at(discriminant) - m_translation).unit_vec();
//    Vec3D color =  0.5 * Vec3D(n.x() + 1, n.y() + 1, n.z() + 1);
//    return {
//        static_cast<uint8_t>(color.x() * 255),
//        static_cast<uint8_t>(color.y() * 255),
//        static_cast<uint8_t>(color.z() * 255)
//    };
//}
