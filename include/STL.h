#include "Shapes.h"
#include <vector>

class STL : public Shape
{
public:
    STL(const char *file_name, Material* mat);

    virtual bool hit(const Ray& ray, const Interval& ray_interval, HitRecord& rec) const override;

    ~STL() override;

private:
    std::vector<Triangle> triangles;
    Material* mat;
    Sphere bounding_sphere;
    std::string name;
};