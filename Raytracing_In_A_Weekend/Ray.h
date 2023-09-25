#pragma once
#include "Vec3D.h"
class Ray
{
public:
    Ray() {}

    Ray(const Vec3D& origin, const Vec3D& direction) : orig(origin), dir(direction) {}

    Vec3D origin() const { return orig; }
    Vec3D direction() const { return dir; }

    Vec3D at(double t) const {
        return  dir * t +  orig;
    }

private:
    Vec3D orig;
    Vec3D dir;
};

