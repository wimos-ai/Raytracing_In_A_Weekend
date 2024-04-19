#include "STL.h"
#include "STLLoader.hpp"

STL::STL(const char *file_name, Material *mat_) : bounding_sphere(Vec3D(0, 0, 0), 0, nullptr), mat(mat_)
{
    auto stl = STLLoader::load_stl(file_name);
    name = std::move(stl.first);
    Vec3D avg_pt;
    Vec3D max_bbx_pt;

    for (auto& tri : stl.second)
    {
        avg_pt = avg_pt + tri.v1 + tri.v2 + tri.v3;
        max_bbx_pt = max_bbx_pt.cwiseMax(tri.v1.cwiseMax(tri.v2.cwiseMax(tri.v3)));
        triangles.emplace_back( tri.v1 , tri.v2 , tri.v3, mat);
    }
    avg_pt /= static_cast<double>(3 * stl.second.size());

    double radius = (max_bbx_pt - avg_pt).norm();

    bounding_sphere = Sphere(avg_pt, radius, mat);
    
}

bool STL::hit(const Ray &ray, const Interval &ray_interval, HitRecord &rec) const {
    HitRecord tmp;
    if (!bounding_sphere.hit(ray, ray_interval, tmp))
    {
        return false;
    }

    for (const auto& tri : triangles)
    {
        if (tri.hit(ray, ray_interval, tmp)){
            rec = tmp;
            return true;
        }
    }
    
    return false;
    
};

STL::~STL(){};