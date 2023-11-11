#include "Camera.h"
#include <cassert>
#include <limits>
#include "Utills.h"
#include "ThreadPool.h"
#include <cstdio>

//Vec3D m_focal_point;		//Focal Point
//Vec3D m_direction;			//Image normal vector, may be discardable after construction
//Vec3D m_viewport_corner;	//Pixel 0,0 location
//Vec3D m_delta_width;		//Horizontal Pixel Delta
//Vec3D m_delta_height;		//Vertical Pixel Delta
//size_t m_width;				//Number of horizontal Pixels
//size_t m_height;			//Number of vertical pixels

Camera::Camera(const Vec3D& pos, const Vec3D& cam_dir, const Vec3D& image_up, double focal_len, size_t pix_width, size_t pix_height) : m_direction(cam_dir.unit_vec()), m_width(pix_width), m_height(pix_height)
{
	m_focal_point = pos - cam_dir.unit_vec() * focal_len;

	const double units_per_pixel = image_up.length() * 2.0 / static_cast<double>(pix_height);


	m_delta_width = image_up.cross(cam_dir).unit_vec() * units_per_pixel;
	m_delta_height = cam_dir.cross(m_delta_width).unit_vec() * units_per_pixel;


	m_viewport_corner = pos - ((m_delta_width * static_cast<double>(pix_width) + m_delta_height * static_cast<double>(pix_height)) / 2.0);
	m_viewport_corner = m_viewport_corner + (0.5 * m_delta_width + m_delta_height);
}



Ray Camera::ray_at_pixel(size_t width, size_t height) const
{
	Vec3D width_vec = m_delta_width * static_cast<double>(width);
	Vec3D height_vec = m_delta_height * static_cast<double>(height);
	Vec3D pix_location = m_viewport_corner + width_vec + height_vec;
	Vec3D fp_to_pixel = pix_location - m_focal_point;
	Vec3D random_deviation = pixel_sample_square();
	return Ray(m_focal_point, fp_to_pixel + random_deviation);
}


Vec3D Camera::pixel_sample_square() const
{
	// Returns a random point in the square surrounding a pixel at the origin.
	auto px = -0.5 + RandUtils::rand();
	auto py = -0.5 + RandUtils::rand();
	return (px * m_delta_width) + (py * m_delta_height);
}

std::pair<size_t, size_t> Camera::width_height_from_aspect_ratio(size_t width, double aspect_ratio) {

	size_t image_height = static_cast<size_t>((double)width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	return{ width, image_height };

}