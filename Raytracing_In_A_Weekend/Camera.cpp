#include "Camera.h"
#include <cassert>
#include <limits>
#include "Utills.h"

//Vec3D m_focal_point;		//Focal Point
//Vec3D m_direction;			//Image normal vector, may be discardable after construction
//Vec3D m_viewport_corner;	//Pixel 0,0 location
//Vec3D m_delta_width;		//Horizontal Pixel Delta
//Vec3D m_delta_height;		//Vertical Pixel Delta
//size_t m_width;				//Number of horizontal Pixels
//size_t m_height;			//Number of vertical pixels

Camera::Camera(Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height, CameraConfig* cfg) : m_direction(cam_dir.unit_vec()), m_height(pix_height), m_width(pix_width)
{
	m_focal_point = pos - cam_dir.unit_vec() * focal_len;

	const double units_per_pixel = image_up.length() * 2.0 / static_cast<double>(pix_height);


	m_delta_width = image_up.cross(cam_dir).unit_vec() * units_per_pixel;
	m_delta_height = image_up.unit_vec() * units_per_pixel;


	m_viewport_corner = pos - ((m_delta_width * static_cast<double>(pix_width) + m_delta_height * static_cast<double>(pix_height)) / 2.0);
	m_viewport_corner = m_viewport_corner + (0.5 * m_delta_width + m_delta_height);

	if (cfg != nullptr)
	{
		m_samples_per_pixel = cfg->samples_per_pixel;
	}
	else {
		m_samples_per_pixel = 10;
	}

	assert(image_up.dot(cam_dir) == 0);
	assert(m_delta_height.dot(m_delta_width) == 0);
	assert(m_delta_height.cross(m_delta_width).unit_vec() == (cam_dir.unit_vec() * -1));
}


Image Camera::snap(HittableScene& scene)
{
	Image im(m_width, m_height);

	for (size_t j = 0; j < im.height(); ++j) {
		for (size_t i = 0; i < im.width(); ++i) {
			Vec3D color(0, 0, 0);
			for (size_t k = 0; k < m_samples_per_pixel; k++)
			{
				Ray ray = ray_to_pixel(i, j);
				RGB_Pixel px = get_ray_color(ray, scene);
				Vec3D px_vec = Vec3D(px.r, px.g, px.b);
				color = color + px_vec;
			}
			color = color / static_cast<double>(m_samples_per_pixel);

			im.at(i, j) = RGB_Pixel(color.x(), color.y(),color.z());
		}
	}
	return im;
}


Ray Camera::ray_to_pixel(size_t width, size_t height)
{
	Vec3D width_vec = m_delta_width * static_cast<double>(width);
	Vec3D height_vec = m_delta_height * static_cast<double>(height);
	Vec3D pix_location = m_viewport_corner + width_vec + height_vec;
	Vec3D fp_to_pixel = pix_location - m_focal_point;
	Vec3D random_deviation = pixel_sample_square();
	return Ray(m_focal_point, fp_to_pixel + random_deviation);
}

RGB_Pixel Camera::get_ray_color(Ray& ray, HittableScene& scene)
{
	HitReccord rec;
	//Having the interval be in the + direction prevents 
	//Us from drawing stuff that is behind the camera
	if (scene.hit(ray, Interval(0.0, Interval::c_INFINITY), rec))
	{
		return RGB_Pixel::from_normal_vec(ray.at(rec.t));
	}
	Vec3D unit_direction = ray.direction().unit_vec();
	auto a = 0.5 * (unit_direction.y() + 1.0);
	Vec3D output_rgb = (1.0 - a) * Vec3D(1.0, 1.0, 1.0) + a * Vec3D(0.5, 0.7, 1.0);
	return RGB_Pixel(output_rgb);
}

Vec3D Camera::pixel_sample_square() const
{
	// Returns a random point in the square surrounding a pixel at the origin.
	auto px = -0.5 + RandUtils::rand();
	auto py = -0.5 + RandUtils::rand();
	return (px * m_delta_width) + (py * m_delta_height);
}


std::pair<size_t, size_t> Camera::width_height_from_aspect_ratio(size_t width, double aspect_ratio) {

	size_t image_height = static_cast<size_t>(width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	return{ width, image_height };

}