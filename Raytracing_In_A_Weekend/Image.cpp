#include "Image.h"

RGB_Pixel& Image::at(size_t x, size_t y)
{
    return pixels[x][y];
}

const RGB_Pixel& Image::at(size_t x, size_t y) const
{
    return pixels[x][y];
}
