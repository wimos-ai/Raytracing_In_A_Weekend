#include "Image.h"

RGB_Pixel& Image::at(size_t m_x, size_t m_y)
{
    return pixels[m_x][m_y];
}

const RGB_Pixel& Image::at(size_t m_x, size_t m_y) const
{
    return pixels[m_x][m_y];
}
