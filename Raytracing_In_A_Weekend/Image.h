#pragma once
#include "RGB_Pixel.h"
#include "BlockArray.hpp"



class Image
{
public:
	inline Image(size_t width, size_t height) : m_pixels(width, height) {};

	inline size_t width() const {
		return m_pixels.dim_1();
	}

	inline size_t height() const {
		return m_pixels.dim_2();
	}

	RGB_Pixel& at(size_t m_x, size_t m_y);

	const RGB_Pixel& at(size_t m_x, size_t m_y) const;

private:
	BlockArray2D<RGB_Pixel> m_pixels;
};

