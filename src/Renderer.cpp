#include <utility>

#include "Renderer.hpp"
#include "ThreadPool.hpp"

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
	cam(cam),
	scene(scene),
	num_thds(num_thds),
	float_image(cam.width(), cam.height()),
	workerPool(num_thds)
{
	this->add_row_tasks();
	return;
}

CMRenderer::~CMRenderer()
{
	workerPool.clear_tasks();
}

void CMRenderer::add_row_tasks()
{
	for (size_t j = 0; j < cam.height(); ++j) {

		workerPool.submit_task([this, j]() {
			this->update_row(j);
			}
		);
	}
	workerPool.submit_task([this]() {
		this->add_row_tasks();
		}
	);
}

void CMRenderer::update_row(size_t row)
{
	for (size_t i = 0; i < cam.width(); i++)
	{
		Ray curr_ray = cam.ray_at_pixel(i, row);
		float_image[i][row].first += RendererCommon::get_ray_color(curr_ray, this->scene, this->DEFAULT_DEPTH);
		float_image[i][row].second += 1;
	}
}

Image CMRenderer::get_image() noexcept
{
	Image im(cam.width(), cam.height());
	for (size_t j = 0; j < cam.height(); ++j) {

		for (size_t i = 0; i < cam.width(); i++)
		{
			im.at(i, j) = RGB_Pixel::normalize_average(float_image[i][j].first, float_image[i][j].second);
		}
	}
	return im;
}
