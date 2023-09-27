#pragma once
#include <cstdint>
#include "Vec3D.h"
typedef struct RGB_Pixel{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	RGB_Pixel() = default;

	template <typename R_t, typename G_t, typename B_t>
	inline RGB_Pixel(R_t r_in, G_t g_in, B_t b_in) : r(r_in), g(g_in), b(b_in) {};

	explicit RGB_Pixel(const Vec3D& other);
}RGB_Pixel;

