#include "RGB_Pixel.h"
#include <limits>

RGB_Pixel::RGB_Pixel(const Vec3D& other):
	r(static_cast<uint8_t>(std::numeric_limits<uint8_t>::max()* other.x())),
	g(static_cast<uint8_t>(std::numeric_limits<uint8_t>::max()* other.y())),
	b(static_cast<uint8_t>(std::numeric_limits<uint8_t>::max()* other.z()))
{
}
