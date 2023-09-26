#pragma once
#include "Vec3D.h"
class Ray
{
public:
    Ray() = default;

    inline Ray(const Vec3D& origin, const Vec3D& direction) : orig(origin), dir(direction) {}

    inline Vec3D origin() const { return orig; }
    inline Vec3D direction() const { return dir; }

    inline Vec3D at(double t) const {
        return  dir * t +  orig;
    }

private:
    Vec3D orig;
    Vec3D dir;
};

