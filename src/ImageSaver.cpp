#include "ImageSaver.hpp"

#include <cstdio>
#include <array>
#include <fstream>
#include <bit>

static_assert(std::endian::native == std::endian::little);

// PPM Image Saver
void PPMImageSaver::save(const Image &image, const char *file_path)
{
	std::ofstream file_out(file_path);

	file_out << "P3\n"
			 << image.width() << ' ' << image.height() << "\n255\n";

	for (size_t h = 0; h < image.height(); h++)
	{
		for (size_t w = 0; w < image.width(); w++)
		{
			const RGB_Pixel &pixel = image.at(w, h);
			file_out << pixel.r << ' ' << pixel.g << ' ' << pixel.b << ' ';
		}
		file_out << '\n';
	}

	file_out.close();
}

namespace
{

	void writeInt16(std::ofstream &file, int16_t value)
	{
		char shifted_value(value & 0xFF);
		file.write(&shifted_value, 1);

		shifted_value = ((value >> 8) & 0xFF);
		file.write(&shifted_value, 1);
	}

	void writeInt32(std::ofstream &file, int32_t value)
	{
		char shifted_value = (value & 0xFF);
		file.write(&shifted_value, 1);

		shifted_value = ((value >> 8) & 0xFF);
		file.write(&shifted_value, 1);

		shifted_value = ((value >> 16) & 0xFF);
		file.write(&shifted_value, 1);

		shifted_value = ((value >> 24) & 0xFF);
		file.write(&shifted_value, 1);
	}

	void writeBitmapFileHeader(std::ofstream &file, size_t width, size_t height)
	{
		const int32_t fileSize = static_cast<int32_t>(54 + (3 * width * height)); // 54-byte header + RGB data
		file.write("BM", 2);
		writeInt32(file, fileSize);
		writeInt16(file, 0);
		writeInt16(file, 0);
		writeInt32(file, 54); // Offset to RGB data
	}

	void writeBitmapInfoHeader(std::ofstream &file, size_t width, size_t height)
	{
		writeInt32(file, 40); // Info header size
		writeInt32(file, static_cast<int32_t>(width));
		writeInt32(file, static_cast<int32_t>(height));
		writeInt16(file, 1);	// Number of color planes
		writeInt16(file, 24);	// Bits per pixel
		writeInt32(file, 0);	// Compression method (none)
		writeInt32(file, 0);	// Image size (can be 0 for uncompressed images)
		writeInt32(file, 2835); // Horizontal resolution (2835 pixels per meter)
		writeInt32(file, 2835); // Vertical resolution (2835 pixels per meter)
		writeInt32(file, 0);	// Number of colors in the color palette (0 for 24-bit)
		writeInt32(file, 0);	// Number of important colors used (0 means all are important)
	}

}

void BMPImageSaver::save(const Image &image, const char *file_path)
{
	size_t width = image.width();
	size_t height = image.height();

	std::ofstream file_out(file_path, std::ios::binary);

	// BMP header
	writeBitmapFileHeader(file_out, width, height);
	writeBitmapInfoHeader(file_out, width, height);

	// Write pixel data
	for (long long y = (static_cast<long long>(height)) - 1; y >= 0; y--)
	{
		for (size_t x = 0; x < width; x++)
		{
			RGB_Pixel pixel = image.at(x, y);
			std::array<unsigned char, 3> bgr_pixel{pixel.b, pixel.g, pixel.r};

			file_out.write(reinterpret_cast<const char *>(bgr_pixel.data()), sizeof(bgr_pixel));
		}
		// Padding to align each row to a multiple of 4 bytes
		for (size_t padding = 0; padding < (4 - (width * 3) % 4) % 4; padding++)
		{
			file_out.write("\0", 1);
		}
	}

	file_out.close();
}
