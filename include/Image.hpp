#pragma once
#include "RGB_Pixel.hpp"
#include "BlockArray.hpp"



class Image
{
public:
	inline Image(size_t width, size_t height) : m_pixels(width, height) {};

	inline size_t width() const {
		return m_pixels.size();
	}

	inline size_t height() const {
		return m_pixels[0].size();
	}

	RGB_Pixel& at(size_t m_x, size_t m_y);

	const RGB_Pixel& at(size_t m_x, size_t m_y) const;

	std::span<RGB_Pixel> data() {
		return m_pixels.data();
	}

	std::span<const RGB_Pixel> data() const {
		return m_pixels.data();
	}

private:
	BlockArray2D<RGB_Pixel> m_pixels;
};

