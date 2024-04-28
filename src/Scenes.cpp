#include "Scenes.hpp"
#include "Shapes.hpp"
#include "Material.hpp"
#include "STL.hpp"

HittableScene final_render()
{
	using std::make_unique;

	HittableScene world;

	{
		auto ground_material = make_unique<Lambertian>(Color3D(0.5, 0.5, 0.5));
		auto s2 = make_unique<Sphere>(Vec3D(0, -1000, 0), 1000, ground_material.get());

		world.add_shape(std::move(s2));
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
					world.add_shape(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
				else if (choose_mat < 0.95)
				{
					// metal
					Vec3D albedo = randomVec3D(Interval(0.5, 1));
					double fuzz = RandUtils::rand(Interval(0, 0.5));
					auto sphere_material = make_unique<FuzzyMetal>(albedo, fuzz);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.add_shape(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
				else
				{
					// glass
					auto sphere_material = make_unique<Dielectric>(1.5);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.add_shape(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
			}
		}
	}

	auto material1 = make_unique<Dielectric>(1.5);
	auto s1 = make_unique<Sphere>(Vec3D(0, 1, 0), 1.0, material1.get());
	world.add_shape(std::move(s1));
	world.take_ownership(std::move(material1));

	auto material2 = make_unique<Lambertian>(Color3D(0.4, 0.2, 0.1));
	auto s2 = make_unique<Sphere>(Vec3D(-4, 1, 0), 1.0, material2.get());
	world.add_shape(std::move(s2));
	world.take_ownership(std::move(material2));

	auto material3 = make_unique<FuzzyMetal>(Vec3D(0.7, 0.6, 0.5), 0.0);
	auto s3 = make_unique<Sphere>(Vec3D(4, 1, 0), 1.0, material3.get());
	world.add_shape(std::move(s3));
	world.take_ownership(std::move(material3));

	return world;
}

HittableScene img_13_shiny_metal()
{

	using std::make_unique;

	HittableScene scene;
	auto material_ground = make_unique<Lambertian>(Color3D(0.8, 0.8, 0.0));
	auto material_center = make_unique<Lambertian>(Color3D(0.7, 0.3, 0.3));
	auto material_left = make_unique<Dielectric>(1);
	auto material_right = make_unique<Metal>(Color3D(0.8, 0.6, 0.2));

	auto s1 = make_unique<Sphere>(Vec3D(0.0, -100.5, -1.0), 100.0, material_ground.get());
	auto s2 = make_unique<Sphere>(Vec3D(0.0, 0.0, -1.0), 0.5, material_center.get());
	auto s3 = make_unique<Sphere>(Vec3D(-1.0, 0.0, -1.0), -0.5, material_left.get());
	auto s4 = make_unique<Sphere>(Vec3D(1.0, 0.0, -1.0), 0.5, material_right.get());

	scene.add_shape(std::move(s1));
	scene.add_shape(std::move(s2));
	scene.add_shape(std::move(s3));
	scene.add_shape(std::move(s4));

	return scene;
}

HittableScene interesting_green()
{
	using std::make_unique;

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

	scene.add_shape(std::move(s1));
	scene.add_shape(std::move(s2));
	scene.add_shape(std::move(s3));
	scene.add_shape(std::move(s4));

	return scene;
}

HittableScene forbidden_nacho()
{
	HittableScene sceen;

	auto nacho_color = std::make_unique<Lambertian>(Vec3D(1, 1, 0));
	auto t1 = std::make_unique<Triangle>(Vec3D(0.0, 0.0, -1.0), Vec3D(0.0, -1.0, -1.0), Vec3D(-1, -1, -1.0), nacho_color.get());

	sceen.add_shape(std::move(t1));
	sceen.take_ownership(std::move(nacho_color));

    return sceen;
}

HittableScene binary_stl_test()
{

	HittableScene sceen;

	auto stl_color = std::make_unique<Metal>(Vec3D(0, 1, 1));
	auto stl = std::make_unique<STL>("../ComplexBinarySTL.STL", stl_color.get());

	sceen.add_shape(std::move(stl));
	sceen.take_ownership(std::move(stl_color));

	return sceen;
}

HittableScene emissionTest()
{
	using std::make_unique;

	HittableScene world;

	{
		auto ground_material = make_unique<Lambertian>(Color3D(0.5, 0.5, 0.5));
		auto s2 = make_unique<Sphere>(Vec3D(0, -1000, 0), 1000, ground_material.get());

		world.add_shape(std::move(s2));
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
					world.add_shape(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
				else if (choose_mat < 0.95)
				{
					// metal
					Vec3D albedo = randomVec3D(Interval(0.5, 1));
					double fuzz = RandUtils::rand(Interval(0, 0.5));
					auto sphere_material = make_unique<FuzzyMetal>(albedo, fuzz);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.add_shape(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
				else
				{
					// glass
					auto sphere_material = make_unique<Dielectric>(1.5);
					auto s = make_unique<Sphere>(center, 0.2, sphere_material.get());
					world.add_shape(std::move(s));
					world.take_ownership(std::move(sphere_material));
				}
			}
		}
	}

	auto material1 = make_unique<Emissive>(Color3D{ 1,1,1 },20);
	auto s1 = make_unique<Sphere>(Vec3D(0, 1, 0), 1.0, material1.get());
	world.add_shape(std::move(s1));
	world.take_ownership(std::move(material1));


	return world;
}
