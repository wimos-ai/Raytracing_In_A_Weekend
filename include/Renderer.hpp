#pragma once

#include "Camera.hpp"
#include "HittableScene.hpp"
#include "BlockArray.hpp"

#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>

// Basic Single Threaded Renderer
class Renderer
{
public:
	Renderer(size_t samples_per_pixel, size_t max_depth, const Camera &cam, const HittableScene &scene)
		: m_samples_per_pixel(samples_per_pixel), m_max_depth(max_depth), cam(cam), scene(scene)
	{
	}
	Image get_image();

private:
	size_t m_samples_per_pixel; // Number of ray samples per pixel
	size_t m_max_depth;			// Number of times a ray can bounce
	const Camera &cam;
	const HittableScene &scene;
};

// Continuous, Multithreaded, Atomic (Lock Free) Renderer
class CMARenderer
{
public:
	// Creates and begins rendering
	CMARenderer(size_t num_thds, const Camera &cam, const HittableScene &scene);

	// Stops rendering
	~CMARenderer();

	// Gets currently rendered image
	Image get_image() noexcept;

private:
	void update_row(size_t row, std::mt19937_64 &rd);
	void thread_main();

private:
	const Camera &cam;
	const HittableScene &scene;
	size_t num_thds;
	BlockArray2D<std::pair<Color3D, size_t>> float_image;
	std::vector<std::thread> threads;
	std::vector<std::atomic<bool>> row_process_flags;
	std::atomic<bool> running{true};

	constexpr static size_t DEFAULT_DEPTH = 1000;
};