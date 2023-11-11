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
public:


	Camera(const Vec3D& pos, const Vec3D& direction, const Vec3D& image_up, double focal_len, size_t pix_width, size_t pix_height);
	Camera() = delete;

	static std::pair<size_t, size_t> width_height_from_aspect_ratio(size_t width, double aspect_ratio);

	size_t width() const {
		return m_width;
	}
	size_t height() const {
		return m_height;
	}

	Ray ray_at_pixel(size_t width, size_t height) const;
private:
	Vec3D pixel_sample_square() const;

private:
	
	Vec3D m_focal_point;		//Focal Point
	Vec3D m_direction;			//Image normal vector, may be discardable after construction
	Vec3D m_viewport_corner;	//Pixel 0,0 location
	Vec3D m_delta_width;		//Horizontal Pixel Delta
	Vec3D m_delta_height;		//Vertical Pixel Delta
	size_t m_width;				//Number of horizontal Pixels
	size_t m_height;			//Number of vertical pixels
};



