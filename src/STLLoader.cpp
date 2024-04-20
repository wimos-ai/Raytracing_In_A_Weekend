#include <cstdio>
#include <memory>
#include <cstdint>

#include "STLLoader.hpp"
#include "Shapes.h"
#include "Material.h"

constexpr const size_t ONE_KB = 1024;

constexpr const char *ASCII_HEADER_BEGIN = "solid ";
constexpr const char *ASCII_FACET_BEGIN = "facet normal ";
constexpr const char *ASCII_FACET_END = "endfacet";
constexpr const char *ASCII_LOOP_BEGIN = "outer loop ";
constexpr const char *ASCII_LOOP_END = "endloop";
constexpr const char *ASCII_FILE_END = "endsolid ";

namespace
{
    void safe_read(char *buff, size_t len, std::ifstream &istream)
    {
        istream >> std::ws;
        istream.read(buff, len);

        if ( istream.fail())
        {
            throw std::runtime_error("Failed to read buffer");
        }
    }

    bool is_ascii_stl_nm(std::ifstream &file)
    {
        // header
        char tmp[20] = {0};

        // file.clear();
        file.seekg(0);

        safe_read(tmp, std::strlen(ASCII_HEADER_BEGIN), file);

        if (std::strcmp(tmp, ASCII_HEADER_BEGIN) == 0)
        {
            return true;
        }

        return false;
    }
}

// An ASCII STL file begins with the line:
// solid <name>
//  where name is an optional string (though if name is omitted there must still be a space after solid
// Therefore all ascii files start with "solid " which is 6 chars so 7 with the null terminator
bool STLLoader::is_ascii_stl(const char *file_path)
{
    std::ifstream file(file_path, std::ios::binary);

    return is_ascii_stl_nm(file);
}

std::pair<std::string, std::vector<STL_Triangle>> STLLoader::load_stl(const char *file_path)
{

    if (STLLoader::is_ascii_stl(file_path))
    {
        return STLLoader::load_ascii_stl(file_path);
    }
    else
    {
        return STLLoader::load_binary_stl(file_path);
    }
}
/* Minimal ASCII file example
solid name
    facet normal ni nj nk
        outer loop
            vertex v1x v1y v1z
            vertex v2x v2y v2z
            vertex v3x v3y v3z
        endloop
    endfacet
endsolid name

Indenting is optional
*/
std::pair<std::string, std::vector<STL_Triangle>> STLLoader::load_ascii_stl(const char *file_path)
{
    std::ifstream file(file_path, std::ios::in);

    char line_buffer[ONE_KB] = {0};

    file >> std::ws;
    file.getline(line_buffer, sizeof(line_buffer));

    std::string name(line_buffer + std::strlen(ASCII_HEADER_BEGIN));

    std::vector<STL_Triangle> stl;

    // Read in each facet
    while (true)
    {
        Vec3D normal{0, 0, 0};
        Vec3D v1{0, 0, 0};
        Vec3D v2{0, 0, 0};
        Vec3D v3{0, 0, 0};

        // Parse Normal or find end of file marker
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));

        if (std::strncmp(line_buffer, ASCII_FILE_END, std::strlen(ASCII_FILE_END)) == 0)
        {
            break;
        }

        if (std::strncmp(line_buffer, ASCII_FACET_BEGIN, std::strlen(ASCII_FACET_BEGIN)) == 0)
        {
            if (std::sscanf(line_buffer, "facet normal %lf %lf %lf", &normal[0], &normal[1], &normal[2]) != 3)
            {
                throw std::runtime_error("Malformed ASCII STL facet normal line");
            }
        }
        else
        {
            throw std::runtime_error("Malformed ASCII STL");
        }

        // Parse outer loop
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));
        if (std::strncmp(line_buffer, ASCII_LOOP_BEGIN, std::strlen(ASCII_LOOP_BEGIN) != 0))
        {
            throw std::runtime_error("Malformed ASCII STL");
        }

        // Parse first vertice
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));
        if (std::sscanf(line_buffer, "vertex %lf %lf %lf", &v1[0], &v1[1], &v1[2]) != 3)
        {
            throw std::runtime_error("Malformed ASCII STL vertex");
        }

        // Parse second vertice
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));
        if (std::sscanf(line_buffer, "vertex %lf %lf %lf", &v2[0], &v2[1], &v2[2]) != 3)
        {
            throw std::runtime_error("Malformed ASCII STL vertex");
        }

        // Parse third vertice
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));
        if (std::sscanf(line_buffer, "vertex %lf %lf %lf", &v3[0], &v3[1], &v3[2]) != 3)
        {
            throw std::runtime_error("Malformed ASCII STL vertex");
        }

        // Parse end outer loop
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));
        if (std::strncmp(line_buffer, ASCII_LOOP_END, std::strlen(ASCII_LOOP_END)) != 0)
        {
            throw std::runtime_error("Malformed ASCII STL, did not find end of loop");
        }

        // Parse end of facet
        file >> std::ws;
        file.getline(line_buffer, sizeof(line_buffer));
        if (std::strncmp(line_buffer, ASCII_FACET_END, std::strlen(ASCII_FACET_END)) != 0)
        {
            throw std::runtime_error("Malformed ASCII STL, did not find end of facet");
        }

        STL_Triangle tri = {v1, v2, v3};
        stl.push_back(tri);
    }

    return std::make_pair(name, stl);
}

#pragma pack(push, 1)
typedef struct BinarySTLTriangle
{
    float normal_vec[3];
    float v1[3];
    float v2[3];
    float v3[3];
    uint16_t attribute;
} BinarySTLTriangle;
#pragma pack(pop) // disables the effect of #pragma pack from now on

auto a = sizeof(BinarySTLTriangle);
static_assert(sizeof(BinarySTLTriangle) == 50);

std::pair<std::string, std::vector<STL_Triangle>> STLLoader::load_binary_stl(const char *file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::in);
    char header[80] = {0};
    if (file.readsome(header, sizeof(header)) != sizeof(header))
    {
        throw std::runtime_error("Malformed Binary STL, could not read header");
    }
    uint32_t num_triangles = 0;
    if (file.readsome(reinterpret_cast<char *>(&num_triangles), sizeof(num_triangles)) != sizeof(num_triangles))
    {
        throw std::runtime_error("Malformed Binary STL, could not read number of triangles");
    }

    std::vector<STL_Triangle> stl(num_triangles);
    for (uint32_t i = 0; i < num_triangles; i++)
    {
        // Read in triangle
        BinarySTLTriangle triangle;
        file.read(reinterpret_cast<char *>(&triangle), sizeof(triangle));
        if (file.eof() && file.fail())
        {
            throw std::runtime_error("Malformed Binary STL, could not read triangle");
        }

        STL_Triangle tri{
            Vec3D{triangle.v1[0], triangle.v1[1], triangle.v1[2]},
            Vec3D{triangle.v2[0], triangle.v2[1], triangle.v2[2]},
            Vec3D{triangle.v3[0], triangle.v3[1], triangle.v3[2]}};

        stl[i] = tri;
    }

    return std::make_pair(std::string(file_path), stl);
}
