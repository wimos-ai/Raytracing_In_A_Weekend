// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Image.h"
#include "ImageSaver.h"
#include "Camera.h"


#include "ray.h"
#include "Vec3D.h"
#include "Shapes.h"
#include "Material.h"


void  img_13_shiny_metal() {
	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

	Camera::CameraConfig cfg = { 0 };
	cfg.samples_per_pixel = 100;
	cfg.max_depth = 100;
	cfg.num_thds = 20;

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second, &cfg);

	HittableScene scene;
	Lambertian material_ground(Color3D(0.8, 0.8, 0.0));
	Lambertian material_center(Color3D(0.7, 0.3, 0.3));
	Metal material_left(Color3D(0.8, 0.8, 0.8));
	Metal material_right(Color3D(0.8, 0.6, 0.2));

	Sphere s1(Vec3D(0.0, -100.5, -1.0), 100.0, &material_ground);
	Sphere s2(Vec3D(0.0, 0.0, -1.0), 0.5, &material_center);
	Sphere s3(Vec3D(-1.0, 0.0, -1.0), 0.5, &material_left);
	Sphere s4(Vec3D(1.0, 0.0, -1.0), 0.5, &material_right);

	scene.push_back(&s1);
	scene.push_back(&s2);
	scene.push_back(&s3);
	scene.push_back(&s4);

	Image im = cam.snap(scene);

	BMPImageSaver::save(im, "image.bmp");
}

void interesting_green() {

	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);


	Camera::CameraConfig cfg = { 0 };
	cfg.samples_per_pixel = 100;
	cfg.max_depth = 100;
	cfg.num_thds = 20;

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second, &cfg);

	HittableScene sceen;

	const Color3D RED(1, 0, 0);
	const Color3D GREEN(0, 1, 0);
	const Color3D BLUE(0, 0, 1);
	const Color3D WHITE(1, 1, 1);


	Lambertian material_ground(RED);
	Metal material_center(GREEN);
	Lambertian material_left(BLUE);
	Lambertian material_right(WHITE);

	Sphere s1(Vec3D(0.0, -100.5, -1.0), 100.0, &material_ground);
	Sphere s2(Vec3D(0.0, 0.0, -1.0), 0.5, &material_center);
	Sphere s3(Vec3D(-1.0, 0.0, -1.0), 0.5, &material_left);
	Sphere s4(Vec3D(1.0, 0.0, -1.0), 0.5, &material_right);

	sceen.push_back(&s2);
	sceen.push_back(&s1);
	sceen.push_back(&s3);
	sceen.push_back(&s4);

	auto im = cam.snap(sceen);

	BMPImageSaver::save(im, "image.bmp");
}


int main()
{
	img_13_shiny_metal();
}
