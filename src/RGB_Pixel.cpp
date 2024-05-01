#include "RGB_Pixel.hpp"
#include <limits>
#include "Vec3D.hpp"

RGB_Pixel::RGB_Pixel(const Vec3D &other) : r(static_cast<uint8_t>(255 * (other.x()))),
										   g(static_cast<uint8_t>(255 * (other.y()))),
										   b(static_cast<uint8_t>(255 * (other.z())))
{
}

RGB_Pixel RGB_Pixel::from_normal_vec(const Vec3D &other)
{
	Vec3D N = (other - Vec3D(0, 0, -1)).normalized();
	return RGB_Pixel(0.5 * Vec3D(N.x() + 1, N.y() + 1, N.z() + 1));
}
