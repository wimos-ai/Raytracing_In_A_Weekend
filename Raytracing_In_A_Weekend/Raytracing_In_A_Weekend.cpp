// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Image.h"
#include "PPMImageSaver.h"
#include "Camera.h"


#include "ray.h"
#include "Vec3D.h"
#include "Shapes.h"

#include <iostream>


int main()
{

	auto sz = Camera::width_height_from_aspect_ratio(400, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1, sz.first, sz.second);

	std::vector<Shape*> sceen;

	Sphere s1({ 0,0,-1 }, .5);

	sceen.push_back(&s1);

	auto im = cam.snap(sceen);

	PPMImageSaver::save(im, "image.ppm");
}
