#include <utility>

#include "Renderer.hpp"

// ------------- RendererCommon -------------
Color3D RendererCommon::sky_color(const Ray& sky_ray)
{
	Vec3D unit_direction = sky_ray.direction().normalized();
	double a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * Vec3D(1.0, 1.0, 1.0) + a * Vec3D(0.5, 0.7, 1.0);
}

RGB_Pixel RendererCommon::render_pixel(size_t x, size_t y, size_t samples, size_t depth, const Camera& cam, const HittableScene& scene)
{
	Color3D color(0, 0, 0);
	for (size_t i = 0; i < samples; i++)
	{
		Ray curr_ray = cam.ray_at_pixel(x, y);
		color = color + get_ray_color(curr_ray, scene, depth);
	}
	return RGB_Pixel::normalize_average(color, samples);
}

Vec3D RendererCommon::get_ray_color(const Ray& ray, const HittableScene& scene, size_t depth)
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
		return { 0,0,0 };
		//return sky_color(ray);
	}

	Ray scattered;
	Color3D attenuation;
	Color3D emission = rec.material->emission();

	if (!rec.material->scatter(ray, rec, attenuation, scattered))
		return emission;

	Color3D scatter_color = attenuation.cwiseProduct(get_ray_color(scattered, scene, depth - 1));

	return scatter_color + emission;
}

// ------------- Renderer -------------

Image Renderer::render()
{

	Image im(cam.width(), cam.height());

	for (size_t j = 0; j < im.height(); ++j)
	{
		for (size_t i = 0; i < im.width(); i++)
		{
			im.at(i, j) = RendererCommon::render_pixel(i, j, m_samples_per_pixel, m_max_depth, cam, scene);
		}
	}

	return im;
}

// ------------- MTRenderer -------------

void MTRenderer::render_row(Image& im_out, size_t row_idx) const
{
	for (size_t i = 0; i < im_out.width(); i++)
	{
		im_out.at(i, row_idx) = RendererCommon::render_pixel(i, row_idx, m_samples_per_pixel, m_max_depth, cam, scene);
	}
}

Image MTRenderer::render() {
	Image im(cam.width(), cam.height());

	ThreadPool ray_trace_pool(m_num_thds);
	for (size_t j = 0; j < im.height(); ++j) {

		ray_trace_pool.submit_task([this, j, &im]() {
			this->render_row(im, j);
			}
		);
	}
	return im;
}

// ------------- CMRenderer -------------

CMRenderer::CMRenderer(size_t num_thds, const Camera& cam, const HittableScene& scene) :
	thds(),
	cam(cam)
{
	for (size_t i = 0; i < num_thds; i++)
	{
		thds.emplace_back(std::make_unique<WorkerThread>(cam, scene));
	}
}

Image CMRenderer::get_image() noexcept
{
	BlockArray2D<Color3D> float_image(cam.width(), cam.height());
	//this->pause();

	// Determine the total number of iterations per pixel
	size_t iters = 0;
	for (auto& it : thds)
	{
		iters += it->iters_per_pixel;
	}

	Color3D* cumulative_image_start = &float_image[0][0];

	// Sum worker thread images together weighted by number of iterations per pixel per thread
	for (auto& it : thds)
	{
		double percentage = static_cast<double>(it->iters_per_pixel) / static_cast<double>(iters);
		Color3D* it_im_start = &it->image[0][0];
		for (size_t i = 0; i < cam.width() * cam.height(); i++)
		{
			cumulative_image_start[i] += it_im_start[i] * percentage;
		}
	}

	//this->resume();

	Image im_out(cam.width(), cam.height());
	RGB_Pixel* rgb_start = &im_out.at(0, 0);
	for (size_t i = 0; i < cam.width() * cam.height(); i++)
	{
		Color3D gamma_color = linear_to_gamma(cumulative_image_start[i]);
		Color3D thresholded = gamma_color.cwiseMin(Color3D{ 1,1,1 });
		rgb_start[i] = RGB_Pixel(thresholded);
	}
	return im_out;
}

CMRenderer::~CMRenderer()
{
	for (auto& it : thds)
	{
		it->dead = true;
	}

	for (auto& it : thds)
	{
		it->join();
	}
}


CMRenderer::WorkerThread::WorkerThread(const Camera& cam, const HittableScene& scene) :
	std::thread([this]() {this->main(); }),
	image(cam.width(), cam.height()),
	iters_per_pixel(0),
	dead(false),
	cam(cam),
	scene(scene)
{
}

void CMRenderer::WorkerThread::main() noexcept {
	while (true)
	{
		if (dead)
		{
			return;
		}

		for (size_t j = 0; j < image.size(); ++j)
		{
			for (size_t i = 0; i < image[j].size(); i++)
			{
				Ray curr_ray = cam.ray_at_pixel(j, i);
				image[j][i] += RendererCommon::get_ray_color(curr_ray, scene, DEFAULT_DEPTH);
			}
		}
		iters_per_pixel++;
	}
}
