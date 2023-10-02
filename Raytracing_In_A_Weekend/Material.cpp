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

bool Dielectric::scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const
{
	attenuation = Color3D(1,1,1);
	double refraction_ratio = rec.front_face ? (1.0 / m_index_of_refraction) : m_index_of_refraction;

	Vec3D unit_direction = r_in.direction().unit_vec();
	double cos_theta = std::fmin(-unit_direction.dot(rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	Vec3D direction;
	if (cannot_refract || Dielectric::Schlicks_approx_refl(cos_theta, refraction_ratio) > RandUtils::rand())
		direction = Vec3D::reflect(unit_direction, rec.normal);
	else
		direction = Vec3D::refract(unit_direction, rec.normal, refraction_ratio);

	scattered = Ray(rec.point, direction);
	return true;
}

double Dielectric::Schlicks_approx_refl(double cos_val, double refl_idx)
{
	// Use Schlick's approximation for reflectance.
	//R(theta) ~ R_0 + (1-R_0)(1-cos(theta))^5
	double r0 = (1 - refl_idx) / (1 + refl_idx);
	r0 = r0 * r0;
	const double cv = (1 - cos_val);
	const double cv5 = (cv * cv) * (cv * cv) * cv;
	return r0 + (1 - r0) * cv5;
}