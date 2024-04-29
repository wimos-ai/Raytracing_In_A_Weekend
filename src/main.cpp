// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include <thread>

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

#include <iostream>

#include <chrono>

int main()
{
    HittableScene world{ emissionTest()};
    auto sz = Camera::width_height_from_aspect_ratio(600, 16.0 / 9.0);

    Camera cam(Vec3D(13, 2, 3), Vec3D(-13, -2, -3), Vec3D(0, -1, 0), 10, sz.first, sz.second);

    CMRenderer renderer(19, cam, world);

    std::this_thread::sleep_for(std::chrono::milliseconds(60000));
    BMPImageSaver::save(renderer.get_image(), "c.bmp");
    std::cout << "c" << std::endl;
}
