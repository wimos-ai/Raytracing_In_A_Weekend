#pragma once
#include <Eigen/Dense>
#include "Utills.h"
using Vec3D = Eigen::Vector3d;
using Color3D = Vec3D;

inline Color3D linear_to_gamma(Color3D&color){
    return color.cwiseSqrt();
}

inline Vec3D randomVec3D()
{
    return Vec3D(RandUtils::rand(), RandUtils::rand(), RandUtils::rand());
}

inline Vec3D randomVec3D(const Interval &interval)
{
    return Vec3D(RandUtils::rand(interval), RandUtils::rand(interval), RandUtils::rand(interval));
}

inline bool vec_near_zero(const Vec3D &v)
{
    // Return true if the vector is close to zero in all dimensions.
    auto s = 1e-8;
    return (fabs(v[0]) < s) && (fabs(v[1]) < s) && (fabs(v[2]) < s);
}

inline Vec3D vec_reflect(const Vec3D &v, const Vec3D &n)
{
    return v - (n * (2 * v.dot(n)));
}

inline Vec3D vec_refract(const Vec3D &ray_dir, const Vec3D &normal, double eta_div_eta_prime)
{
    double cos_theta = fmin(-ray_dir.dot(normal), 1.0);
    Vec3D r_out_perp = eta_div_eta_prime * (ray_dir + cos_theta * normal);
    Vec3D r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredNorm())) * normal;
    return r_out_perp + r_out_parallel;
}

inline Vec3D random_vec_in_unit_sphere() {
    while (true) {
        Vec3D p = randomVec3D(Interval( - 1, 1));
        if (p.squaredNorm() < 1)
            return p;
    }
}

inline Vec3D random_unit_vec() {
    return random_vec_in_unit_sphere().normalized();
}