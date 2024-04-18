#include "Vec3D.h"

Vec3D operator*(const Vec3D& u, const Vec3D& v)
{
    return Vec3D(u.m_x * v.m_x, u.m_y * v.m_y, u.m_z * v.m_z);
}

Vec3D operator*(double d, const Vec3D& other) {
	return other * d;
}
