#pragma once
#include "Ray.h"
#include "Vec3D.h"


struct HitReccord;
class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const HitReccord& rec, Color3D& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Color3D& a) : albedo(a) {};

    virtual bool scatter(const Ray& r_in, const HitReccord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D albedo;
};

class Metal : public Material {
public:
    Metal(const Color3D& a) : albedo(a) {}

    bool scatter(const Ray& r_in, const HitReccord& rec, Color3D& attenuation, Ray& scattered) const override;

private:
    Color3D albedo;
};