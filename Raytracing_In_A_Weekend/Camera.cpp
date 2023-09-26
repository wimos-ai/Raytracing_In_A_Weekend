#include "Camera.h"
#include <cassert>
#include <limits>

namespace {
	Vec3D calc_focal_point(Vec3D pos, Vec3D cam_dir, double focal_len) {
		return pos - cam_dir.unit_vec() * focal_len;
	}

	Vec3D calc_delta_width(Vec3D cam_dir, Vec3D up, double units_per_pixel) {
		Vec3D delta_width_direction = cam_dir.cross(up).unit_vec();

		return  delta_width_direction * units_per_pixel;
	}

	Vec3D calc_delta_height(Vec3D image_height, double units_per_pixel) {
		Vec3D delta_height_direction = image_height.unit_vec();
		return image_height * -units_per_pixel;
	}

	double units_per_pixel(Vec3D image_up, size_t height_px) {
		return image_up.length() * 2 / static_cast<double>(height_px);
	}

	Vec3D calc_viewport_corner(Vec3D pos, Vec3D camera_dir, Vec3D image_up, size_t pix_height, size_t pix_width) {
		Vec3D top = (pos + image_up);
		double upp = units_per_pixel(image_up, pix_height);
		Vec3D delta_width = calc_delta_width(camera_dir, image_up, upp);
		double half_width_pixels = static_cast<double>(pix_width) / 2.0;
		top = top - (delta_width * half_width_pixels);
		return top;
	}

}


Camera::Camera(Vec3D pos, Vec3D cam_dir, Vec3D image_up, double focal_len, size_t pix_width, size_t pix_height) :
	m_focal_point(calc_focal_point(pos, cam_dir, focal_len)),
	m_direction(cam_dir.unit_vec()),
	m_delta_width(calc_delta_width(cam_dir, image_up, units_per_pixel(image_up, pix_height))),
	m_delta_height(calc_delta_height(image_up, units_per_pixel(image_up, pix_height))),
	m_height(pix_height),
	m_width(pix_width),
	m_viewport_corner(calc_viewport_corner(pos, cam_dir, image_up, pix_height, pix_width))
{
	assert(image_up.dot(cam_dir) == 0);
	assert(m_delta_height.dot(m_delta_width) == 0);
	assert(m_delta_height.cross(m_delta_width).unit_vec() == (cam_dir.unit_vec() * -1));
}

Image Camera::snap(std::vector<Shape*>& shapes)
{
	Image im(m_width, m_height);

	Ray ray;
	int j = 0;
	int i = 0;
	for (j = 0; j < im.height(); ++j) {
		for (i = 0; i < im.width(); ++i) {
			ray = ray_to_pixel(i, j);
			im.at(i, j) = get_ray_color(ray, shapes);
		}
	}
	ray = ray_to_pixel(i, j);
	return im;
}


Ray Camera::ray_to_pixel(size_t width, size_t height)
{
	Vec3D width_vec = m_delta_width * static_cast<double>(width);
	Vec3D height_vec = m_delta_height * static_cast<double>(height);
	Vec3D pix_location = m_viewport_corner + width_vec + height_vec;
	Vec3D fp_to_pixel = pix_location - m_focal_point;
	return Ray(m_focal_point, fp_to_pixel);
}

RGB_Pixel Camera::get_ray_color(Ray& ray, std::vector<Shape*>& shapes)
{
	for (auto &it: shapes)
	{
		if (it->hit(ray)) {
			return RGB_Pixel{ 255,0,0 };
		}
	}


	Vec3D unit_direction = ray.direction().unit_vec();
	auto a = 0.5 * (unit_direction.y() + 1.0);
	Vec3D output_rgb = (1.0 - a) * Vec3D(1.0, 1.0, 1.0) + a * Vec3D(0.5, 0.7, 1.0);
	return {
		static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * output_rgb.x()),
		static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * output_rgb.y()),
		static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * output_rgb.z())
	};
}

std::pair<size_t, size_t> Camera::width_height_from_aspect_ratio(size_t width, double aspect_ratio) {

	size_t image_height = static_cast<size_t>(width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	return{ width, image_height };

}