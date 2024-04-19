#include "Shapes.h"
#include <cassert>

void HitRecord::set_face_normal(const Ray& ray, const Vec3D& vec) {
	front_face = ray.direction().dot(vec) < 0;
	normal = front_face ? vec : -1 * vec;
	//assert(vec.unit_vec() == vec);
}

Sphere::Sphere(const Vec3D& translation, double radius, const Material* mat) : m_translation(translation), m_radius(radius), m_material(mat)
{
}

bool Sphere::hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const
{
	const Vec3D oc = ray.origin() - this->m_translation;
	const double a = ray.direction().squaredNorm();
	const double half_b = ray.direction().dot(oc);
	const double c = oc.squaredNorm() - m_radius * m_radius;

	const double discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return false;
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
	rec.normal = ((rec.point - m_translation) / m_radius).normalized();
	rec.set_face_normal(ray, rec.normal);
	rec.material = m_material;

	return true;
}

Sphere::~Sphere(){};

bool Triangle::hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const
{
    
 
    // Step 1: finding P
    
    // check if the ray and plane are parallel.
    double NdotRayDirection = m_normal.dot(ray.direction());
    if (fabs(NdotRayDirection) < 1e-9) // almost 0
        return false; // they are parallel, so they don't intersect! 

    // compute d parameter using equation 2
    double d = -m_normal.dot(m_v0);
    
    // compute t (equation 3)
    double t = -(m_normal.dot(ray.origin()) + d) / NdotRayDirection;
    
    // check if the triangle is behind the ray
    if (t < 0) return false; // the triangle is behind
 
    // compute the intersection point using equation 1
    Vec3D P = ray.origin() + t * ray.direction();
 
    // Step 2: inside-outside test
    Vec3D C; // vector perpendicular to triangle's plane
 
    // edge 0
    Vec3D edge0 = m_v1 - m_v0; 
    Vec3D vp0 = P - m_v0;
    C = edge0.cross(vp0);
    if (m_normal.dot(C) < 0) return false; // P is on the right side
 
    // edge 1
    Vec3D edge1 = m_v2 - m_v1; 
    Vec3D vp1 = P - m_v1;
    C = edge1.cross(vp1);
    if (m_normal.dot(C) < 0)  return false; // P is on the right side
 
    // edge 2
    Vec3D edge2 = m_v0 - m_v2; 
    Vec3D vp2 = P - m_v2;
    C = edge2.cross(vp2);
    if (m_normal.dot(C) < 0) return false; // P is on the right side;

    rec.t = t;
    rec.point = -P;
    rec.normal = m_normal.normalized();
    rec.set_face_normal(ray, rec.normal);
    rec.material = m_material;
    
    return true; // this ray hits the triangle
}

Vec3D Triangle::generate_normal(const Vec3D& v0, const Vec3D& v1, const Vec3D& v2)
{
    // compute the plane's normal
    Vec3D v0v1 = v1 - v0;
    Vec3D v0v2 = v2 - v0;
    // no need to normalize
    return v0v1.cross(v0v2); // N
}

Triangle::Triangle(const Vec3D& v0, const Vec3D& v1, const Vec3D& v2, const Material* mat): 
    m_v0(v0), 
    m_v1(v1), 
    m_v2(v2),
    m_normal(Triangle::generate_normal(v0,v1,v2)), 
    m_material(mat) 
{
}
Triangle::~Triangle(){};