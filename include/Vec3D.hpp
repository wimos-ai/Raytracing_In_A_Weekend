#pragma once
#include <Eigen/Dense>
#include <random>
#include "Interval.hpp"
using Vec3D = Eigen::Vector3d;
using Color3D = Vec3D;

inline Color3D linear_to_gamma(Color3D &color)
{
    return color.cwiseSqrt();
}

inline Vec3D randomVec3D(std::mt19937_64 &rd)
{
    return Vec3D(std::generate_canonical<double, -1u>(rd), std::generate_canonical<double, -1u>(rd), std::generate_canonical<double, -1u>(rd));
}

inline Vec3D randomVec3D(std::mt19937_64 &rd, const Interval &interval)
{
    std::uniform_real_distribution<double> dis(interval.min(), interval.max());
    return Vec3D(dis(rd), dis(rd), dis(rd));
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

inline Vec3D random_vec_in_unit_sphere(std::mt19937_64 &rd)
{
    while (true)
    {
        Vec3D p = randomVec3D(rd, Interval(-1, 1));
        if (p.squaredNorm() < 1)
            return p;
    }
}

inline Vec3D random_unit_vec(std::mt19937_64 &rd)
{
    return random_vec_in_unit_sphere(rd).normalized();
}