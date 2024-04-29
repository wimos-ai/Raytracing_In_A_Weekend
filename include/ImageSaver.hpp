#pragma once
#include "Image.hpp"
#include <cstdint>


class PPMImageSaver
{
public:
	static void save(const Image& image, const char* file_path);
};

class BMPImageSaver {
public:
    static void save(const Image& image, const char* file_path);
};