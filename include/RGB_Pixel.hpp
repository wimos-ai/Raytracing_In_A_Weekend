#pragma once
#include <cstdint>
#include <cstddef>
#include "Vec3D.hpp"

using std::size_t;

typedef struct RGB_Pixel{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	inline constexpr RGB_Pixel(): r(0), g(0), b(0){};

	template <typename R_t, typename G_t, typename B_t>
	inline explicit constexpr RGB_Pixel(R_t r_in, G_t g_in, B_t b_in) : r(r_in), g(g_in), b(b_in) {};

	explicit RGB_Pixel(const Vec3D& other);

	static RGB_Pixel from_normal_vec(const Vec3D& other);
	static RGB_Pixel normalize_average(const Color3D& other, size_t num_samples);
}RGB_Pixel;
