// Raytracing_In_A_Weekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "Image.h"
#include "ImageSaver.h"
#include "Camera.h"
#include "Renderer.h"

#include "STL.h"
#include "Ray.h"
#include "Vec3D.h"
#include "Shapes.h"
#include "Material.h"
#include "Utills.h"

void final_render()
{
	using std::make_unique;

	HittableScene world;

	{
		auto ground_material = make_unique<Lambertian>(Color3D(0.5, 0.5, 0.5));
		auto s2 = make_unique<Sphere>(Vec3D(0, -1000, 0), 1000, ground_material.get());

		world.push_back(std::move(s2));
		world.take_ownership(std::move(ground_material));
	}

	for (int_fast8_t a = -11; a < 11; a++)
	{
		for (int_fast8_t b = -11; b < 11; b++)
		{
			auto choose_mat = RandUtils::rand();
			Vec3D center(a + 0.9 * RandUtils::rand(), 0.2, b + 0.9 * RandUtils::rand());

			if ((center - Vec3D(4, 0.2, 0)).norm() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					// diffuse
					Vec3D albedo = (randomVec3D().cwiseProduct(randomVec3D()));
					auto sphere_material = make_unique<Lambertian>(albedo);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.push_back(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
				else if (choose_mat < 0.95)
				{
					// metal
					Vec3D albedo = randomVec3D(Interval(0.5, 1));
					double fuzz = RandUtils::rand(Interval(0, 0.5));
					auto sphere_material = make_unique<FuzzyMetal>(albedo, fuzz);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.push_back(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
				else
				{
					// glass
					auto sphere_material = make_unique<Dielectric>(1.5);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.push_back(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
			}
		}
	}

	auto material1 = make_unique<Dielectric>(1.5);
	auto s1 = make_unique<Sphere>(Vec3D(0, 1, 0), 1.0, material1.get());
	world.push_back(std::move(s1));
	world.take_ownership(std::move(material1));

	auto material2 = make_unique<Lambertian>(Color3D(0.4, 0.2, 0.1));
	auto s2 = make_unique<Sphere>(Vec3D(-4, 1, 0), 1.0, material2.get());
	world.push_back(std::move(s2));
	world.take_ownership(std::move(material2));

	auto material3 = make_unique<FuzzyMetal>(Vec3D(0.7, 0.6, 0.5), 0.0);
	auto s3 = make_unique<Sphere>(Vec3D(4, 1, 0), 1.0, material3.get());
	world.push_back(std::move(s3));
	world.take_ownership(std::move(material3));

	auto sz = Camera::width_height_from_aspect_ratio(600, 16.0 / 9.0);

	Camera cam(Vec3D(13, 2, 3), Vec3D(-13, -2, -3), Vec3D(0, -1, 0), 10, sz.first, sz.second);

	Renderer renderer(50, 50, 19);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Image im = renderer.render(cam, world);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << '+';

	BMPImageSaver::save(im, "image.bmp");
}

void img_13_shiny_metal()
{

	using std::make_unique;
	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 2, 0), Vec3D(0, -2, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

	HittableScene scene;
	auto material_ground = make_unique<Lambertian>(Color3D(0.8, 0.8, 0.0));
	auto material_center = make_unique<Lambertian>(Color3D(0.7, 0.3, 0.3));
	auto material_left = make_unique<Dielectric>(1);
	auto material_right = make_unique<Metal>(Color3D(0.8, 0.6, 0.2));

	auto s1 = make_unique<Sphere>(Vec3D(0.0, -100.5, -1.0), 100.0, material_ground.get());
	auto s2 = make_unique<Sphere>(Vec3D(0.0, 0.0, -1.0), 0.5, material_center.get());
	auto s3 = make_unique<Sphere>(Vec3D(-1.0, 0.0, -1.0), -0.5, material_left.get());
	auto s4 = make_unique<Sphere>(Vec3D(1.0, 0.0, -1.0), 0.5, material_right.get());

	scene.push_back(std::move(s1));
	scene.push_back(std::move(s2));
	scene.push_back(std::move(s3));
	scene.push_back(std::move(s4));

	Renderer render(100, 100, 1);
	Image im = render.render(cam, scene);

	BMPImageSaver::save(im, "image.bmp");
}

void interesting_green()
{
	using std::make_unique;

	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

	HittableScene scene;

	const Color3D RED(1, 0, 0);
	const Color3D GREEN(0, 1, 0);
	const Color3D BLUE(0, 0, 1);
	const Color3D WHITE(1, 1, 1);

	Lambertian material_ground(RED);
	Metal material_center(GREEN);
	Lambertian material_left(BLUE);
	Lambertian material_right(WHITE);

	auto s1 = make_unique<Sphere>(Vec3D(0.0, -100.5, -1.0), 100.0, &material_ground);
	auto s2 = make_unique<Sphere>(Vec3D(0.0, 0.0, -1.0), 0.5, &material_center);
	auto s3 = make_unique<Sphere>(Vec3D(-1.0, 0.0, -1.0), 0.5, &material_left);
	auto s4 = make_unique<Sphere>(Vec3D(1.0, 0.0, -1.0), 0.5, &material_right);

	scene.push_back(std::move(s1));
	scene.push_back(std::move(s2));
	scene.push_back(std::move(s3));
	scene.push_back(std::move(s4));

	Renderer renderer(100, 100, 20);
	auto im = renderer.render(cam, scene);

	BMPImageSaver::save(im, "image.bmp");
}

void forbidden_nacho()
{
	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(0, 0, -2), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

	HittableScene sceen;

	auto nacho_color = std::make_unique<Lambertian>(Vec3D(1, 1, 0));
	auto t1 = std::make_unique<Triangle>(Vec3D(0.0, 0.0, -1.0), Vec3D(0.0, -1.0, -1.0), Vec3D(-1, -1, -1.0), nacho_color.get());

	sceen.emplace_back(std::move(t1));
	sceen.take_ownership(std::move(nacho_color));

	Renderer renderer(50, 50, 20);
	auto im = renderer.render(cam, sceen);

	BMPImageSaver::save(im, "image.bmp");
}

void binary_stl_test()
{
	auto sz = Camera::width_height_from_aspect_ratio(1000, 16.0 / 9.0);

	// Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height
	Camera cam(Vec3D(0, 0, 0), Vec3D(1, -1, 1), Vec3D(0, -1, 0), 1.0, sz.first, sz.second);

	HittableScene sceen;

	auto stl_color = std::make_unique<Metal>(Vec3D(0, 1, 1));
	auto stl = std::make_unique<STL>("../ComplexBinarySTL.STL", stl_color.get());

	sceen.emplace_back(std::move(stl));
	sceen.take_ownership(std::move(stl_color));

	Renderer renderer(100, 100, 20);
	auto im = renderer.render(cam, sceen);

	BMPImageSaver::save(im, "image.bmp");
}

void emissionTest()
{
	using std::make_unique;
	HittableScene world;

	auto material1 = make_unique<FuzzyMetal>(Vec3D(1, 0, 0), 0.1);
	auto s1 = make_unique<Sphere>(Vec3D(0, 1, 0), 1.0, material1.get());
	world.push_back(std::move(s1));
	world.take_ownership(std::move(material1));

	auto material2 = make_unique<Dielectric>(1.5);
	auto s2 = make_unique<Sphere>(Vec3D(-4, 1, 0), 1.0, material2.get());
	world.push_back(std::move(s2));
	world.take_ownership(std::move(material2));

	auto material3 = make_unique<Emissive>(Color3D(1, 1, 1), 20);
	auto s3 = make_unique<Sphere>(Vec3D(4, 1, 0), 1.0, material3.get());
	world.push_back(std::move(s3));
	world.take_ownership(std::move(s3));

	auto sz = Camera::width_height_from_aspect_ratio(600, 16.0 / 9.0);

	Camera cam(Vec3D(13, 2, 3), Vec3D(-13, -2, -3), Vec3D(0, -1, 0), 10, sz.first, sz.second);

	Renderer renderer(1, 20, 1);
	auto im = renderer.render(cam, world);

	BMPImageSaver::save(im, "emissionTest.bmp");
}

int main()
{
	std::cout << "THREAD POOL" << std::endl;
	final_render();
	std::cout << std::endl;
}
