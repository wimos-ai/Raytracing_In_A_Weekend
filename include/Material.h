#pragma once
#include "Ownable.hpp"
#include "Ray.h"
#include "Vec3D.h"


struct HitRecord;
class Material : public Ownable
{
public:

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const;
    virtual Vec3D emission() const;
};

class Lambertian : public Material {
public:
    inline explicit Lambertian(const Color3D& a) : m_albedo(a) {};

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D m_albedo;
};

class Metal : public Material {
public:
    inline explicit Metal(const Color3D& a) : m_albedo(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D m_albedo;
};

class FuzzyMetal : public Material {
public:
    inline FuzzyMetal(const Color3D& a, double fuzz) : m_albedo(a), m_fuzz(fuzz) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D m_albedo;
    double m_fuzz;
};

class Dielectric : public Material {
public:
    inline explicit Dielectric(double refract_idx) : m_index_of_refraction(refract_idx) {};

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    static double Schlicks_approx_refl(double cos_val, double refl_idx);
    double m_index_of_refraction;
};

class Emissive : public Material {
public:
    inline explicit Emissive(Color3D color_, float intensity_) : color(color_), intensity(intensity_){};

    virtual Vec3D emission() const override;

private:
    Color3D color;
    float intensity;
};