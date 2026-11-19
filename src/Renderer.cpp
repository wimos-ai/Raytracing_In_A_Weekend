#include <utility>

#include "Renderer.hpp"

// ------------- RendererCommon -------------

namespace RendererCommon
{
	Color3D sky_color(const Ray &sky_ray)
	{
		Vec3D unit_direction = sky_ray.direction().normalized();
		double a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * Vec3D(1.0, 1.0, 1.0) + a * Vec3D(0.5, 0.7, 1.0);
	}

	Vec3D get_ray_color(const Ray &ray, const HittableScene &scene, size_t depth, std::mt19937_64 &rd)
	{
		if (depth <= 0)
		{
			return Color3D(0, 0, 0);
		}

		HitRecord rec;
		// Having the interval be in the + direction prevents
		// Us from drawing stuff that is behind the camera
		if (!scene.hit(ray, Interval(0.001, Interval::INF), rec))
		{
			// return { 0,0,0 };
			return sky_color(ray);
		}

		Ray scattered;
		Color3D attenuation;
		Color3D emission = rec.material->emission();

		if (!rec.material->scatter(ray, rec, attenuation, scattered, rd))
			return emission;

		Color3D scatter_color = attenuation.cwiseProduct(get_ray_color(scattered, scene, depth - 1, rd));

		return scatter_color + emission;
	}

	RGB_Pixel render_pixel(size_t x, size_t y, size_t samples, size_t depth, const Camera &cam, const HittableScene &scene, std::mt19937_64 &rd)
	{
		Color3D color(0, 0, 0);
		for (size_t i = 0; i < samples; i++)
		{
			Ray curr_ray = cam.ray_at_pixel(x, y, rd);
			color = color + get_ray_color(curr_ray, scene, depth, rd);
		}
		return RGB_Pixel::normalize_average(color, samples);
	}

}

// ------------- Renderer -------------

Image Renderer::get_image()
{
	std::mt19937_64 rd;
	Image im(cam.width(), cam.height());

	for (size_t j = 0; j < im.height(); ++j)
	{
		for (size_t i = 0; i < im.width(); i++)
		{
			im.at(i, j) = RendererCommon::render_pixel(i, j, m_samples_per_pixel, m_max_depth, cam, scene, rd);
		}
	}

	return im;
}

// ------------ CMARenderer -------------------

CMARenderer::CMARenderer(size_t num_thds, const Camera &cam, const HittableScene &scene) : cam(cam),
																						   scene(scene),
																						   num_thds(num_thds),
																						   float_image(cam.width(), cam.height()),
																						   row_process_flags(cam.height())
{
	// Thread access constraints only hold as long as our indexes can't overlap
	assert(num_thds < cam.height() / 2);

	for (size_t i = 0; i < num_thds; i++)
	{
		this->threads.emplace_back([this]
								   { this->thread_main(); });
	}
}

CMARenderer::~CMARenderer()
{
	this->running.store(false, std::memory_order::relaxed);
	for (auto &thd : this->threads)
	{
		thd.join();
	}
}

void CMARenderer::update_row(size_t row, std::mt19937_64 &rd)
{
	for (size_t i = 0; i < cam.width(); i++)
	{
		Ray curr_ray = cam.ray_at_pixel(i, row, rd);
		float_image[i][row].first += RendererCommon::get_ray_color(curr_ray, this->scene, this->DEFAULT_DEPTH, rd);
		float_image[i][row].second += 1;
	}
}

void CMARenderer::thread_main()
{
	std::mt19937_64 rd;
	while (this->running.load(std::memory_order::relaxed))
	{
		for (size_t i = 0; i < this->cam.height(); i++)
		{
			bool expected = false;
			if (this->row_process_flags[i].compare_exchange_strong(expected, true, std::memory_order::acquire, std::memory_order::relaxed))
			{
				update_row(i, rd);
				this->row_process_flags[i].store(false, std::memory_order::release);
			}
			if (!this->running.load(std::memory_order::relaxed))
			{
				return;
			}
		}
	}
}

Image CMARenderer::get_image() noexcept
{
	Image im(cam.width(), cam.height());
	// TODO: Get locks

	auto pixels = im.data();
	auto hsv_colors = this->float_image.data();

	for (size_t i = 0; i < im.data().size(); i++)
	{
		pixels[i] = RGB_Pixel::normalize_average(hsv_colors[i].first, hsv_colors[i].second);
	}

	return im;
}