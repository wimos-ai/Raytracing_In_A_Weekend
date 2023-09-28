// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Image.h"
#include "ImageSaver.h"
#include "Camera.h"


#include "ray.h"
#include "Vec3D.h"
#include "Shapes.h"

#include <iostream>


int main()
{

	auto sz = Camera::width_height_from_aspect_ratio(400, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

	HittableScene sceen;

	Sphere s1(Vec3D(0, 0, -1), 0.5);
	Sphere s2(Vec3D(0, -100.5, -1), 100);
	Sphere s3(Vec3D(0, 0, -2), 0.5);


	sceen.push_back(&s3);
	sceen.push_back(&s2);
	sceen.push_back(&s1);
	

	auto im = cam.snap(sceen);

	BMPImageSaver::save(im, "image.bmp");
}
