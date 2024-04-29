#pragma once
#include "Image.hpp"
#include <cstdint>
#include <vector>


class PPMImageSaver
{
public:
	static void save(const Image& image, const char* file_path);
	static std::vector<char> serialize(const Image& image);
};

class BMPImageSaver {
public:
    static void save(const Image& image, const char* file_path);
	static std::vector<char> serialize(const Image& image);
};