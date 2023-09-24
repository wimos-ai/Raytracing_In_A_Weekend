#pragma once
#include "RGB_Pixel.h"
#include "BlockArray.hpp"



class Image
{
public:
	inline Image(size_t width, size_t height) : pixels(width, height) {};

	inline size_t width() const {
		return pixels.dim_1();
	}

	inline size_t height() const {
		return pixels.dim_2();
	}

	RGB_Pixel& at(size_t x, size_t y);

	const RGB_Pixel& at(size_t x, size_t y) const;

private:
	BlockArray2D<RGB_Pixel> pixels;
};

