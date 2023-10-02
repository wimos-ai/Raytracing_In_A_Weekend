#pragma once
#include "Ray.h"
#include "Vec3D.h"


struct HitRecord;
class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Color3D& a) : m_albedo(a) {};

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D m_albedo;
};

class Metal : public Material {
public:
    Metal(const Color3D& a) : m_albedo(a) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D m_albedo;
};

class FuzzyMetal : public Material {
public:
    FuzzyMetal(const Color3D& a, double fuzz) : m_albedo(a), m_fuzz(fuzz) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D m_albedo;
    double m_fuzz;
};