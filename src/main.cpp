// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "Scenes.hpp"
#include "Image.hpp"
#include "ImageSaver.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"

#include "STL.hpp"
#include "Ray.hpp"
#include "Vec3D.hpp"
#include "Shapes.hpp"
#include "Material.hpp"
#include "Utills.hpp"

int main()
{
    HittableScene world{final_render()};
    auto sz = Camera::width_height_from_aspect_ratio(600, 16.0 / 9.0);

    Camera cam(Vec3D(13, 2, 3), Vec3D(-13, -2, -3), Vec3D(0, -1, 0), 10, sz.first, sz.second);

    Renderer renderer(1, 20, 20);
    auto im = renderer.render(cam, world);

    BMPImageSaver::save(im, "emissionTest.bmp");
}
