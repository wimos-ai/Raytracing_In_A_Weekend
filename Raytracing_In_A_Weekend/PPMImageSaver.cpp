#include "PPMImageSaver.h"
#include <cstdio>

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
