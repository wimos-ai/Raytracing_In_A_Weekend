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


RGB_Pixel RGB_Pixel::normalize_average(const Color3D& other, size_t num_samples) {
	Color3D avg_color = other / static_cast<double>(num_samples);
	Color3D gamma_color = linear_to_gamma(avg_color);
	Color3D thresholded = gamma_color.cwiseMin(Color3D{ 1,1,1 });
	return RGB_Pixel(thresholded);
}