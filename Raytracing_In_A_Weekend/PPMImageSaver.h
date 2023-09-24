#pragma once
#include "Image.h"

class PPMImageSaver
{
public:
	static void save(const Image& im, const char* fp);
};

