#include "ImageSaver.hpp"

#include <cstdio>
#include <array>
#include <fstream>
#include <bit>
#include <sstream>

static_assert(std::endian::native == std::endian::little);

namespace {
	void writePPM(const Image& image, std::ostream& out) {
		out << "P3\n" << image.width() << ' ' << image.height() << "\n255\n";

		for (size_t h = 0; h < image.height(); h++)
		{
			for (size_t w = 0; w < image.width(); w++)
			{
				const RGB_Pixel& pixel = image.at(w, h);
				out << pixel.r << ' ' << pixel.g << ' ' << pixel.b << ' ';
			}
			out << '\n';
		}
	}
}

// PPM Image Saver
void PPMImageSaver::save(const Image &image, const char *file_path)
{
	std::ofstream file_out(file_path);
	writePPM(image, file_out);
	file_out.close();
}

std::vector<char> PPMImageSaver::serialize(const Image& image) {
	std::stringstream bytes_out; 
	writePPM(image, bytes_out);
	auto str = bytes_out.str();
	return std::vector(str.begin(), str.end());
}

namespace
{
	static constexpr size_t BMP_FILE_HEADER_SIZE = 54;
	static constexpr size_t BMP_INFO_HEADER_SIZE = 40;
	static constexpr size_t BMP_BITS_PER_PIXEL = 24;

	void appendInt16(std::vector<char>& bytes, int16_t value)
	{
		char shifted_value(value & 0xFF);
		bytes.emplace_back(shifted_value);

		shifted_value = ((value >> 8) & 0xFF);
		bytes.emplace_back(shifted_value);
	}

	void appendInt32(std::vector<char>& bytes, int32_t value)
	{
		char shifted_value = (value & 0xFF);
		bytes.emplace_back(shifted_value);

		shifted_value = ((value >> 8) & 0xFF);
		bytes.emplace_back(shifted_value);

		shifted_value = ((value >> 16) & 0xFF);
		bytes.emplace_back(shifted_value);

		shifted_value = ((value >> 24) & 0xFF);
		bytes.emplace_back(shifted_value);
	}

	void writeBitmapFileHeader(std::vector<char>& bytes, size_t width, size_t height)
	{
		const int32_t fileSize = static_cast<int32_t>(54 + (3 * width * height)); // 54-byte header + RGB data
		bytes.emplace_back('B');
		bytes.emplace_back('M');
		appendInt32(bytes, fileSize);
		appendInt16(bytes, 0);
		appendInt16(bytes, 0);
		appendInt32(bytes, BMP_FILE_HEADER_SIZE); // Offset to RGB data
	}

	void writeBitmapInfoHeader(std::vector<char>& bytes, size_t width, size_t height)
	{
		appendInt32(bytes, BMP_INFO_HEADER_SIZE); // Info header size
		appendInt32(bytes, static_cast<int32_t>(width));
		appendInt32(bytes, static_cast<int32_t>(height));
		appendInt16(bytes, 1);	// Number of color planes
		appendInt16(bytes, BMP_BITS_PER_PIXEL);	// Bits per pixel
		appendInt32(bytes, 0);	// Compression method (none)
		appendInt32(bytes, 0);	// Image size (can be 0 for uncompressed images)
		appendInt32(bytes, 2835); // Horizontal resolution (2835 pixels per meter)
		appendInt32(bytes, 2835); // Vertical resolution (2835 pixels per meter)
		appendInt32(bytes, 0);	// Number of colors in the color palette (0 for 24-bit)
		appendInt32(bytes, 0);	// Number of important colors used (0 means all are important)
	}

	size_t get_bmp_size_estimate(const Image& image) {
		size_t num_pixels = image.height() * image.width();
		size_t num_pixel_bytes = num_pixels * (BMP_BITS_PER_PIXEL / 8);
		size_t cumulative_bytes = num_pixel_bytes + BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE;
		return cumulative_bytes;

	}

}

void BMPImageSaver::save(const Image &image, const char *file_path)
{
	std::ofstream file_out(file_path, std::ios::binary);
	auto vec = BMPImageSaver::serialize(image);
	file_out.write(vec.data(), vec.size());
	file_out.close();
}

std::vector<char> BMPImageSaver::serialize(const Image& image)
{
	std::vector<char> bytes;
	bytes.reserve(get_bmp_size_estimate(image));
	size_t width = image.width();
	size_t height = image.height();


	// BMP header
	writeBitmapFileHeader(bytes, width, height);
	writeBitmapInfoHeader(bytes, width, height);

	// Write pixel data
	for (long long y = (static_cast<long long>(height)) - 1; y >= 0; y--)
	{
		for (size_t x = 0; x < width; x++)
		{
			const RGB_Pixel& pixel = image.at(x, y);
			bytes.emplace_back(pixel.b);
			bytes.emplace_back(pixel.g);
			bytes.emplace_back(pixel.r);
		}
		// Padding to align each row to a multiple of 4 bytes
		for (size_t padding = 0; padding < (4 - (width * 3) % 4) % 4; padding++)
		{
			bytes.emplace_back('\0');
		}
	}
	return bytes;
}
