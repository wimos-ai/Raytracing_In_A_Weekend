#pragma once


#include "Camera.hpp"
#include "HittableScene.hpp"
#include "BlockArray.hpp"

#include <thread>
#include <memory>
#include <condition_variable>

namespace RendererCommon {
	Color3D sky_color(const Ray& sky_ray);
	RGB_Pixel render_pixel(size_t x, size_t y, size_t samples, size_t depth, const Camera& cam, const HittableScene& scene);
	Vec3D get_ray_color(const Ray& ray, const HittableScene& scene, size_t depth);
}

// Basic Single Threaded Renderer
class Renderer
{
public:
	Renderer(size_t samples_per_pixel, size_t max_depth, const Camera& cam, const HittableScene& scene)
		:m_samples_per_pixel(samples_per_pixel), m_max_depth(max_depth), cam(cam), scene(scene)
	{}
	Image render();
private:
	size_t m_samples_per_pixel; //Number of ray samples per pixel
	size_t m_max_depth;			//Number of times a ray can bounce
	const Camera& cam;
	const HittableScene& scene;
};

// Multithreaded Renderer
class MTRenderer
{
public:
	MTRenderer(size_t samples_per_pixel, size_t max_depth, size_t num_thds, const Camera& cam, const HittableScene& scene)
		:m_samples_per_pixel(samples_per_pixel), m_max_depth(max_depth), m_num_thds(num_thds), cam(cam), scene(scene)
	{}
	Image render();
private:
	void render_row(Image& im_out, size_t row_idx) const;
private:
	size_t m_samples_per_pixel; //Number of ray samples per pixel
	size_t m_max_depth;			//Number of times a ray can bounce
	size_t m_num_thds;			//Number of threads for ray bouncing
	const Camera& cam;
	const HittableScene& scene;
};

// Continuious Multithreaded Renderer
class CMRenderer {
public:
	// Creates and begins rendering
	CMRenderer(size_t num_thds, const Camera& cam, const HittableScene& scene);

	// Stops rendering
	~CMRenderer();

	// Gets currently rendered image
	Image get_image() noexcept;

private:
	class WorkerThread{
	public:
		WorkerThread(const Camera& cam, const HittableScene& scene);

		void main() noexcept;

		BlockArray2D<Color3D> image;
	    std::atomic<size_t> iters_per_pixel  = 0;
		std::atomic_bool dead;
		const Camera& cam;
		const HittableScene& scene;

		std::thread m_thread;

		friend class CMRenderer;
	};

private:
	std::vector<std::unique_ptr<WorkerThread>> thds;
	const Camera& cam;

	constexpr static size_t DEFAULT_DEPTH = 500;

};