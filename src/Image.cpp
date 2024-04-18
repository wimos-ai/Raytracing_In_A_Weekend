#include "Image.h"
#include <cstdint>

RGB_Pixel& Image::at(size_t m_x, size_t m_y)
{
    return m_pixels[m_x][m_y];
}

const RGB_Pixel& Image::at(size_t m_x, size_t m_y) const
{
    return m_pixels[m_x][m_y];
}
