#pragma once
#include <utility> //std::pair

#include "Vec3D.h"
#include "Ray.h"
#include "Image.h"
#include "Shapes.h"
#include "HittableScene.h"


//See https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground/sendingraysintothescene
class Camera
{
private:
	
public:
	typedef struct CameraConfig
	{
		size_t samples_per_pixel;
		size_t max_depth;
		size_t num_thds;
	}CameraConfig;


	Camera(const Vec3D& pos, const Vec3D& direction, const Vec3D& image_up, double focal_len, size_t pix_width, size_t pix_height, CameraConfig* cfg = nullptr);
	Camera() = delete;

	Image snap(const HittableScene& scene);

	static std::pair<size_t, size_t> width_height_from_aspect_ratio(size_t width, double aspect_ratio);

private:
	Ray ray_to_pixel(size_t width, size_t height);
	void compute_row(Image& im_out, size_t row_idx, const HittableScene& scene);
	RGB_Pixel compute_color(size_t width, size_t height, const HittableScene& scene);
	Vec3D get_ray_color(const Ray& ray, const HittableScene& scene, size_t depth);
	Vec3D pixel_sample_square() const;
	static Color3D as_sky_color(const Ray& sky_ray);

private:
	
	Vec3D m_focal_point;		//Focal Point
	Vec3D m_direction;			//Image normal vector, may be discardable after construction
	Vec3D m_viewport_corner;	//Pixel 0,0 location
	Vec3D m_delta_width;		//Horizontal Pixel Delta
	Vec3D m_delta_height;		//Vertical Pixel Delta
	size_t m_width;				//Number of horizontal Pixels
	size_t m_height;			//Number of vertical pixels
	size_t m_samples_per_pixel; //Number of ray samples per pixel
	size_t m_max_depth;			//Number of times a ray can bounce
	size_t m_num_thds;			//Number of threads for ray bouncing

private:
	static constexpr size_t DEFAULT_SAMPLES_PER_PIXEL = 10;
	static constexpr size_t DEFAULT_MAX_DEPTH = 10;
	static const size_t DEFAULT_NUM_THREADS = 1;
};



