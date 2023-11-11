#include "ImageSaver.h"

#if defined(__STDC_LIB_EXT1__) | defined(_MSC_VER)
	#define  __STDC_WANT_LIB_EXT1__ 1
	#define USE_FOPEN_SAFE 1
#endif

#include <stdio.h>

namespace uint8_str_lookup {
	const char* str[] = {
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"10",
		"11",
		"12",
		"13",
		"14",
		"15",
		"16",
		"17",
		"18",
		"19",
		"20",
		"21",
		"22",
		"23",
		"24",
		"25",
		"26",
		"27",
		"28",
		"29",
		"30",
		"31",
		"32",
		"33",
		"34",
		"35",
		"36",
		"37",
		"38",
		"39",
		"40",
		"41",
		"42",
		"43",
		"44",
		"45",
		"46",
		"47",
		"48",
		"49",
		"50",
		"51",
		"52",
		"53",
		"54",
		"55",
		"56",
		"57",
		"58",
		"59",
		"60",
		"61",
		"62",
		"63",
		"64",
		"65",
		"66",
		"67",
		"68",
		"69",
		"70",
		"71",
		"72",
		"73",
		"74",
		"75",
		"76",
		"77",
		"78",
		"79",
		"80",
		"81",
		"82",
		"83",
		"84",
		"85",
		"86",
		"87",
		"88",
		"89",
		"90",
		"91",
		"92",
		"93",
		"94",
		"95",
		"96",
		"97",
		"98",
		"99",
		"100",
		"101",
		"102",
		"103",
		"104",
		"105",
		"106",
		"107",
		"108",
		"109",
		"110",
		"111",
		"112",
		"113",
		"114",
		"115",
		"116",
		"117",
		"118",
		"119",
		"120",
		"121",
		"122",
		"123",
		"124",
		"125",
		"126",
		"127",
		"128",
		"129",
		"130",
		"131",
		"132",
		"133",
		"134",
		"135",
		"136",
		"137",
		"138",
		"139",
		"140",
		"141",
		"142",
		"143",
		"144",
		"145",
		"146",
		"147",
		"148",
		"149",
		"150",
		"151",
		"152",
		"153",
		"154",
		"155",
		"156",
		"157",
		"158",
		"159",
		"160",
		"161",
		"162",
		"163",
		"164",
		"165",
		"166",
		"167",
		"168",
		"169",
		"170",
		"171",
		"172",
		"173",
		"174",
		"175",
		"176",
		"177",
		"178",
		"179",
		"180",
		"181",
		"182",
		"183",
		"184",
		"185",
		"186",
		"187",
		"188",
		"189",
		"190",
		"191",
		"192",
		"193",
		"194",
		"195",
		"196",
		"197",
		"198",
		"199",
		"200",
		"201",
		"202",
		"203",
		"204",
		"205",
		"206",
		"207",
		"208",
		"209",
		"210",
		"211",
		"212",
		"213",
		"214",
		"215",
		"216",
		"217",
		"218",
		"219",
		"220",
		"221",
		"222",
		"223",
		"224",
		"225",
		"226",
		"227",
		"228",
		"229",
		"230",
		"231",
		"232",
		"233",
		"234",
		"235",
		"236",
		"237",
		"238",
		"239",
		"240",
		"241",
		"242",
		"243",
		"244",
		"245",
		"246",
		"247",
		"248",
		"249",
		"250",
		"251",
		"252",
		"253",
		"254",
		"255"
	};
}


//PPM Image Saver
void PPMImageSaver::save(const Image& im, const char* fp)
{
	FILE* file_ptr;

#ifdef USE_FOPEN_SAFE
	if (fopen_s(&file_ptr, fp, "w") != 0)
	{
		std::printf("Error opening file in PPMImageSaver::save routine");
		std::exit(-1);
	}
#else
	file_ptr = fopen(fp, "w+");
    if (!fp) {
        std::printf("Error opening file in PPMImageSaver::save routine");
		std::exit(-1);
    }
#endif

	fprintf(file_ptr, "P3\n%zu %zu\n255\n", im.width(), im.height());


	for (size_t h = 0; h < im.height(); h++)
	{
		for (size_t w = 0; w < im.width(); w++)
		{
			const RGB_Pixel& px = im.at(w, h);

			//equiv fprintf(file_ptr, "%d %d %d ", px.r, px.g, px.b);
			fputs(uint8_str_lookup::str[px.r], file_ptr);
			fputc(' ', file_ptr);
			fputs(uint8_str_lookup::str[px.g], file_ptr);
			fputc(' ', file_ptr);
			fputs(uint8_str_lookup::str[px.b], file_ptr);
			fputc(' ', file_ptr);
		}
		fputc('\n', file_ptr);
	}


}

void BMPImageSaver::save(const Image& im, const char* filename) {
	size_t width = im.width();
	size_t height = im.height();

	FILE* file = {0};
	#ifdef USE_FOPEN_SAFE
		if (fopen_s(&file, filename, "w") != 0)
		{
			std::printf("Error opening file in PPMImageSaver::save routine");
			std::exit(-1);
		}
	#else
		file = fopen(filename, "w+");
		if (!file) {
			std::printf("Error opening file in PPMImageSaver::save routine");
			std::exit(-1);
		}
	#endif

	// BMP header
	writeBitmapFileHeader(file, width, height);
	writeBitmapInfoHeader(file, width, height);

	// Write pixel data
	for (long long y = ((long long)height) - 1; y >= 0; y--) {
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
	int32_t fileSize = static_cast<int32_t>(54 + 3 * width * height); // 54-byte header + RGB data
	fputc('B', file);
	fputc('M', file);
	writeInt32(file, fileSize);
	writeInt16(file, 0);
	writeInt16(file, 0);
	writeInt32(file, 54); // Offset to RGB data
}

void BMPImageSaver::writeBitmapInfoHeader(FILE* file, size_t width, size_t height) {
	writeInt32(file, 40); // Info header size
	writeInt32(file, static_cast<int32_t>(width));
	writeInt32(file, static_cast<int32_t>(height));
	writeInt16(file, 1); // Number of color planes
	writeInt16(file, 24); // Bits per pixel
	writeInt32(file, 0); // Compression method (none)
	writeInt32(file, 0); // Image size (can be 0 for uncompressed images)
	writeInt32(file, 2835); // Horizontal resolution (2835 pixels per meter)
	writeInt32(file, 2835); // Vertical resolution (2835 pixels per meter)
	writeInt32(file, 0); // Number of colors in the color palette (0 for 24-bit)
	writeInt32(file, 0); // Number of important colors used (0 means all are important)
}

void BMPImageSaver::writeInt16(FILE* file, int16_t value) {
	fputc(value & 0xFF, file);
	fputc((value >> 8) & 0xFF, file);
}

void BMPImageSaver::writeInt32(FILE* file, int32_t value) {
	fputc(value & 0xFF, file);
	fputc((value >> 8) & 0xFF, file);
	fputc((value >> 16) & 0xFF, file);
	fputc((value >> 24) & 0xFF, file);
}
