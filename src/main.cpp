// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "Image.h"
#include "ImageSaver.h"
#include "Camera.h"
#include "Renderer.h"


#include "Ray.h"
#include "Vec3D.h"
#include "Shapes.h"
#include "Material.h"
#include "Utills.h"

// void final_render() {
// 	HittableScene world;

// 	{
// 		Lambertian* ground_material = new Lambertian(Color3D(0.5, 0.5, 0.5));
// 		Sphere* s2 = new Sphere(Vec3D(0, -1000, 0), 1000, ground_material);

// 		world.push_back(s2);
// 		world.take_ownership(ground_material);
// 		world.take_ownership(s2);
// 	}


// 	for (int a = -11; a < 11; a++) {
// 		for (int b = -11; b < 11; b++) {
// 			auto choose_mat = RandUtils::rand();
// 			Vec3D center(a + 0.9 * RandUtils::rand(), 0.2, b + 0.9 * RandUtils::rand());

// 			if ((center - Vec3D(4, 0.2, 0)).norm() > 0.9) {
// 				if (choose_mat < 0.8) {
// 					// diffuse
// 					auto albedo = (randomVec3D().cwiseProduct(randomVec3D())).normalized();
// 					Material*  sphere_material = new Lambertian(albedo);
// 					Sphere* s = new Sphere(center, 0.2, sphere_material);
// 					world.push_back(s);
// 					world.take_ownership(sphere_material);
// 					world.take_ownership(s);
// 				}
// 				else if (choose_mat < 0.95) {
// 					// metal
// 					Vec3D albedo = randomVec3D(Interval(0.5,1));
// 					double fuzz = RandUtils::rand(Interval(0, 0.5));
// 					Material* sphere_material = new FuzzyMetal(albedo, fuzz);
// 					Sphere* s = new Sphere(center, 0.2, sphere_material);
// 					world.push_back(s);
// 					world.take_ownership(sphere_material);
// 					world.take_ownership(s);
// 				}
// 				else {
// 					// glass
// 					Material* sphere_material = new Dielectric(1.5);
// 					Sphere* s = new Sphere(center, 0.2, sphere_material);
// 					world.push_back(s);
// 					world.take_ownership(sphere_material);
// 					world.take_ownership(s);
// 				}
// 			}
// 		}
// 	}

// 	auto material1 = new Dielectric(1.5);
// 	Sphere* s1 = new Sphere(Vec3D(0, 1, 0), 1.0, material1);
// 	world.push_back(s1);
// 	world.take_ownership(s1);
// 	world.take_ownership(material1);

// 	auto material2 = new Lambertian(Color3D(0.4, 0.2, 0.1));
// 	Sphere* s2 = new Sphere(Vec3D(-4, 1, 0), 1.0, material2);
// 	world.push_back(s2);
// 	world.take_ownership(s2);
// 	world.take_ownership(material2);

// 	//auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
// 	//world.add(make_shared<sphere>(Vec3D(4, 1, 0), 1.0, material3));
// 	auto material3 = new FuzzyMetal(Vec3D(0.7, 0.6, 0.5), 0.0);
// 	Sphere* s3 = new Sphere(Vec3D(4, 1, 0), 1.0, material3);
// 	world.push_back(s3);
// 	world.take_ownership(s3);
// 	world.take_ownership(material3);


// 	auto sz = Camera::width_height_from_aspect_ratio(600, 16.0 / 9.0);


// 	//cam.vfov = 20;
// 	//cam.lookfrom = Vec3D(13, 2, 3);
// 	//cam.lookat = Vec3D(0, 0, 0);
// 	//cam.vup = Vec3D(0, 1, 0);

// 	//cam.defocus_angle = 0.6;
// 	//cam.focus_dist = 10.0;

// 	Camera cam(Vec3D(13, 2, 3), Vec3D(-13, -2, -3), Vec3D(0, -1, 0), 10, sz.first, sz.second);

// 	Renderer renderer(50, 50, 19);


// 	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// 	Image im = renderer.render(cam, world);
// 	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

// 	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count()<< '+';


// 	BMPImageSaver::save(im, "image.bmp");
// }


// void  img_13_shiny_metal() {
// 	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

// 	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
// 	Camera cam(Vec3D(0, 2, 0), Vec3D(0, -2, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

// 	HittableScene scene;
// 	Lambertian material_ground(Color3D(0.8, 0.8, 0.0));
// 	Lambertian material_center(Color3D(0.7, 0.3, 0.3));
// 	Dielectric material_left(1);
// 	Metal material_right(Color3D(0.8, 0.6, 0.2));

// 	Sphere s1(Vec3D(0.0, -100.5, -1.0), 100.0, &material_ground);
// 	Sphere s2(Vec3D(0.0, 0.0, -1.0), 0.5, &material_center);
// 	Sphere s3(Vec3D(-1.0, 0.0, -1.0), -0.5, &material_left);
// 	Sphere s4(Vec3D(1.0, 0.0, -1.0), 0.5, &material_right);

// 	scene.push_back(&s1);
// 	scene.push_back(&s2);
// 	scene.push_back(&s3);
// 	scene.push_back(&s4);

// 	Renderer render(100, 100, 1);
// 	Image im = render.render(cam, scene);

// 	BMPImageSaver::save(im, "image.bmp");
// }

// void interesting_green() {

// 	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

// 	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
// 	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

// 	HittableScene sceen;

// 	const Color3D RED(1, 0, 0);
// 	const Color3D GREEN(0, 1, 0);
// 	const Color3D BLUE(0, 0, 1);
// 	const Color3D WHITE(1, 1, 1);


// 	Lambertian material_ground(RED);
// 	Metal material_center(GREEN);
// 	Lambertian material_left(BLUE);
// 	Lambertian material_right(WHITE);

// 	Sphere s1(Vec3D(0.0, -100.5, -1.0), 100.0, &material_ground);
// 	Sphere s2(Vec3D(0.0, 0.0, -1.0), 0.5, &material_center);
// 	Sphere s3(Vec3D(-1.0, 0.0, -1.0), 0.5, &material_left);
// 	Sphere s4(Vec3D(1.0, 0.0, -1.0), 0.5, &material_right);

// 	sceen.push_back(&s2);
// 	sceen.push_back(&s1);
// 	sceen.push_back(&s3);
// 	sceen.push_back(&s4);

// 	Renderer renderer(100, 100, 20);
// 	auto im = renderer.render(cam,sceen);

// 	BMPImageSaver::save(im, "image.bmp");
// }


void forbidden_nacho() {
	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

	HittableScene sceen;

	auto nacho_color = std::make_unique<Lambertian>(Vec3D(1,1,0));
	auto t1 = std::make_unique<Triangle>(Vec3D(0.0, 0.0, -1.0), Vec3D(0.0, -1.0, -1.0), Vec3D(-1, -1, -1.0), nacho_color.get());

	sceen.emplace_back(std::move(t1));
	sceen.take_ownership(std::move(nacho_color));

	Renderer renderer(50, 50, 20);
	auto im = renderer.render(cam,sceen);

	BMPImageSaver::save(im, "image.bmp");
}

int main()
{
	std::cout << "THREAD POOL" << std::endl;
	forbidden_nacho();
	std::cout << std::endl;

}
