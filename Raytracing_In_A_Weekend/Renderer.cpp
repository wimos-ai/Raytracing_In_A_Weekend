#include "Renderer.h"
#include "ThreadPool.h"

Image Renderer::render(const Camera& cam, const HittableScene& scene)
{

	Image im(cam.width(), cam.height());

	if (m_num_thds > 1)
	{
		ThreadPool ray_trace_pool(m_num_thds);
		for (size_t j = 0; j < im.height(); ++j) {

			ray_trace_pool.submit_task([this, &im, cam, j, &scene]() {
				render_row(im, j, cam, scene);
				}
			);
		}
	}
	else {
		for (size_t j = 0; j < im.height(); ++j) {
			render_row(im, j,cam, scene);
		}
	}

	return im;

}

Color3D Renderer::sky_color(const Ray& sky_ray)
{
	Vec3D unit_direction = sky_ray.direction().unit_vec();
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * Vec3D(1.0, 1.0, 1.0) + a * Vec3D(0.5, 0.7, 1.0);
}

void Renderer::render_row(Image& im_out, size_t row_idx, const Camera& cam, const HittableScene& scene) const
{
	for (size_t i = 0; i < im_out.width(); i++)
	{
		im_out.at(i, row_idx) = render_pixel(i, row_idx, cam, scene);
	}
}

RGB_Pixel Renderer::render_pixel(size_t x, size_t y, const Camera& cam, const HittableScene& scene) const
{
	Vec3D color(0, 0, 0);
	for (size_t i = 0; i < m_samples_per_pixel; i++)
	{
		Ray curr_ray = cam.ray_at_pixel(x, y);
		color = color +  get_ray_color(curr_ray, scene, m_max_depth);
	}
	return RGB_Pixel::normalize_average(color, m_samples_per_pixel);
}

Vec3D Renderer::get_ray_color(const Ray& ray, const HittableScene& scene, size_t depth) const
{
	if (depth <= 0)
	{
		return Color3D(0, 0, 0);
	}

	HitRecord rec;
	//Having the interval be in the + direction prevents 
	//Us from drawing stuff that is behind the camera
	if (scene.hit(ray, Interval(0.001, Interval::INF), rec))
	{
		Ray scattered;
		Color3D attenuation;
		if (rec.material->scatter(ray, rec, attenuation, scattered))
			return attenuation * get_ray_color(scattered, scene, depth - 1);
		return Color3D(0, 0, 0);
	}
	return sky_color(ray);
}
