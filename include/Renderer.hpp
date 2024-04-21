#pragma once


#include "Camera.h"
#include "HittableScene.h"

class Renderer
{
public:
	Renderer(size_t samples_per_pixel, size_t max_depth, size_t num_thds)
		:m_samples_per_pixel(samples_per_pixel), m_max_depth(max_depth), m_num_thds(num_thds)
	{}
	Image render(const Camera& cam, const HittableScene& scene);
private:
	static Color3D sky_color(const Ray& sky_ray);
	void render_row(Image& im_out, size_t row_idx, const Camera& cam, const HittableScene& scene) const;
	RGB_Pixel render_pixel(size_t x, size_t y, const Camera& cam, const HittableScene& scene) const;
	Vec3D get_ray_color(const Ray& ray, const HittableScene& scene, size_t depth) const;
private:
	size_t m_samples_per_pixel; //Number of ray samples per pixel
	size_t m_max_depth;			//Number of times a ray can bounce
	size_t m_num_thds;			//Number of threads for ray bouncing
};

