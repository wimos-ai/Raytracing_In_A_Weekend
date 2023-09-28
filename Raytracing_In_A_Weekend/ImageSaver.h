#pragma once
#include "Image.h"

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

    static void writeInt16(FILE* file, int value);

    static void writeInt32(FILE* file, int value);
};