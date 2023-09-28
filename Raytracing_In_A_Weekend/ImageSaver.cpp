#include "ImageSaver.h"
#include <cstdio>
#include <fstream>
#include <iostream>

//PPM Image Saver
void PPMImageSaver::save(const Image& im, const char* fp)
{
	FILE* file_ptr;

	if (fopen_s(&file_ptr, fp, "w") != 0)
	{
		std::printf("Error opening file in PPMImageSaver::save routine");
		std::exit(-1);
	}

	fprintf(file_ptr, "P3\n%llu %llu\n255\n", im.width(), im.height());


	for (size_t h = 0; h < im.height(); h++)
	{
		for (size_t w = 0; w < im.width(); w++)
		{
			const RGB_Pixel& px = im.at(w, h);
			fprintf(file_ptr, "%d %d %d ", px.r, px.g, px.b);
		}
		fprintf(file_ptr, "\n");
	}


}

void BMPImageSaver::save(const Image& im, const char* filename) {
	size_t width = im.width();
	size_t height = im.height();

	FILE* file;
	errno_t err = fopen_s(&file, filename, "wb");
	if (err != 0) {
		std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
		return;
	}

	// BMP header
	writeBitmapFileHeader(file, width, height);
	writeBitmapInfoHeader(file, width, height);

	// Write pixel data
	for (signed long long y = height - 1; y >= 0; y--) {
		for (size_t x = 0; x < width; x++) {
			RGB_Pixel pixel = im.at(x, y);
			fputc(pixel.b, file);
			fputc(pixel.g, file);
			fputc(pixel.r, file);
		}
		// Padding to align each row to a multiple of 4 bytes
		for (size_t padding = 0; padding < (4 - (width * 3) % 4) % 4; padding++) {
			fputc(0, file);
		}
	}

	fclose(file);
}


void BMPImageSaver::writeBitmapFileHeader(FILE* file, size_t width, size_t height) {
	int fileSize = 54 + 3 * width * height; // 54-byte header + RGB data
	fputc('B', file);
	fputc('M', file);
	writeInt32(file, fileSize);
	writeInt16(file, 0);
	writeInt16(file, 0);
	writeInt32(file, 54); // Offset to RGB data
}

void BMPImageSaver::writeBitmapInfoHeader(FILE* file, size_t width, size_t height) {
	writeInt32(file, 40); // Info header size
	writeInt32(file, width);
	writeInt32(file, height);
	writeInt16(file, 1); // Number of color planes
	writeInt16(file, 24); // Bits per pixel
	writeInt32(file, 0); // Compression method (none)
	writeInt32(file, 0); // Image size (can be 0 for uncompressed images)
	writeInt32(file, 2835); // Horizontal resolution (2835 pixels per meter)
	writeInt32(file, 2835); // Vertical resolution (2835 pixels per meter)
	writeInt32(file, 0); // Number of colors in the color palette (0 for 24-bit)
	writeInt32(file, 0); // Number of important colors used (0 means all are important)
}

void BMPImageSaver::writeInt16(FILE* file, int value) {
	fputc(value & 0xFF, file);
	fputc((value >> 8) & 0xFF, file);
}

void BMPImageSaver::writeInt32(FILE* file, int value) {
	fputc(value & 0xFF, file);
	fputc((value >> 8) & 0xFF, file);
	fputc((value >> 16) & 0xFF, file);
	fputc((value >> 24) & 0xFF, file);
}
