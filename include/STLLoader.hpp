#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include "Vec3D.h"

typedef struct STL_Triangle{
    Vec3D v1;
    Vec3D v2;
    Vec3D v3;
}STL_Triangle;

// Based on STL Specification from https://en.wikipedia.org/wiki/STL_(file_format)
class STLLoader{
public:
    static std::pair<std::string,std::vector<STL_Triangle>> load_stl(const char* file_path);

private:
    static bool is_ascii_stl(const char* file_path);
    static std::pair<std::string,std::vector<STL_Triangle>> load_ascii_stl(const char* file_path);
    static std::pair<std::string,std::vector<STL_Triangle>> load_binary_stl(const char* file_path);

};