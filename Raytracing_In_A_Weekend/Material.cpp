#include "Material.h"
#include "Shapes.h"

bool Lambertian::scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const
{
	Vec3D scatter_direction = rec.normal + Vec3D::random_unit_vec();
	// Catch degenerate scatter direction
	if (scatter_direction.near_zero())
		scatter_direction = rec.normal;
	scattered = Ray(rec.point, scatter_direction);
	attenuation = m_albedo;
	return true;
}

bool Metal::scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const
{
	Vec3D reflected = Vec3D::reflect(r_in.direction().unit_vec(), rec.normal);
	scattered = Ray(rec.point, reflected);
	attenuation = m_albedo;
	return true;
}

bool FuzzyMetal::scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const
{
	Vec3D reflected = Vec3D::reflect(r_in.direction().unit_vec(), rec.normal);
	scattered = Ray(rec.point, reflected + (m_fuzz * Vec3D::random_unit_vec()));
	attenuation = m_albedo;
	return true;
}
