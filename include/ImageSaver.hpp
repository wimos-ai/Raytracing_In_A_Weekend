#pragma once
#include "Image.h"
#include <cstdint>
#define __STDC_WANT_LIB_EXT1__ 1
#include <cstdio>

class PPMImageSaver
{
public:
	static void save(const Image& im, const char* fp);
};


class BMPImageSaver {
public:
    static void save(const Image& im, const char* filename);

private:
    static void writeBitmapFileHeader(FILE* file, size_t width, size_t height);

    static void writeBitmapInfoHeader(FILE* file, size_t width, size_t height);

    static void writeInt16(FILE* file, int16_t value);

    static void writeInt32(FILE* file, int32_t value);
};