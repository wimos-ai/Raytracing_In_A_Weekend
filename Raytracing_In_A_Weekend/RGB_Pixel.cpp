#include "RGB_Pixel.h"
#include <limits>
#include "Vec3D.h"
#include <cassert>

RGB_Pixel::RGB_Pixel(const Vec3D& other):
	r(static_cast<uint8_t>(255.999 * other.x())),
	g(static_cast<uint8_t>(255.999 * other.y())),
	b(static_cast<uint8_t>(255.999 * other.z()))
{
	assert((other.x() <= 1.0) && (other.y() <= 1.0) && (other.z() <= 1.0));
}

RGB_Pixel RGB_Pixel::from_normal_vec(const Vec3D& other) {
	Vec3D N = (other - Vec3D(0, 0, -1)).unit_vec();
	return RGB_Pixel(0.5 * Vec3D(N.x() + 1, N.y() + 1, N.z() + 1));
}

RGB_Pixel RGB_Pixel::normalize_average(const Vec3D& other, size_t num_samples)
{
	return RGB_Pixel(other/static_cast<double>(num_samples));
}
