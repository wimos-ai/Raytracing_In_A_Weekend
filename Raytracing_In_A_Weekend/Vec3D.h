#pragma once
#include <cmath>
#include "Utills.h"
#include "Interval.h"

class Vec3D
{
public:
	inline Vec3D() : m_x(0), m_y(0), m_z(0) {};
	inline Vec3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {};
	inline double& x() {
		return m_x;
	}

	inline double& y() {
		return m_y;
	}

	inline double& z() {
		return m_z;
	}

	inline const double& x() const {
		return m_x;
	}

	inline const double& y() const {
		return m_y;
	}

	inline const double& z() const {
		return m_z;
	}

	inline Vec3D operator+(const Vec3D& other) const{
		return Vec3D(this->m_x + other.m_x, this->m_y + other.m_y, this->m_z + other.m_z);
	}

	inline Vec3D operator-(const Vec3D& other) const {
		return Vec3D(this->m_x - other.m_x, this->m_y - other.m_y, this->m_z - other.m_z);
	}

	inline Vec3D operator*(double other) const{
		return Vec3D(this->m_x * other, this->m_y * other, this->m_z * other);
	}

	inline Vec3D operator/(double other) const{
		return Vec3D(this->m_x / other, this->m_y / other, this->m_z / other);
	}

	inline double length() const {
		return std::sqrt(length_squared());
	}

	inline double length_squared() const {
		return this->m_x * this->m_x + this->m_y * this->m_y + this->m_z * this->m_z;
	}

	inline double dot( const Vec3D& other) const {
		return this->m_x * other.m_x + this->m_y * other.m_y + this->m_z * other.m_z;
	}

	inline Vec3D cross(const Vec3D& other) const {
		const double x = this->m_y * other.m_z - this->m_z * other.m_y;
		const double y = this->m_z * other.m_x - this->m_x * other.m_z;
		const double z = this->m_x * other.m_y - this->m_y * other.m_x;
		return Vec3D(x, y, z);
	}

	inline Vec3D unit_vec() const {
		double length = this->length();
		return *this / length;
	}

	inline bool operator==(const Vec3D& other) const {
		return this->m_x == other.m_x && this->m_y == other.m_y && this->m_z == other.m_z;
	}

	static inline Vec3D random() {
		return Vec3D(RandUtils::rand(), RandUtils::rand(), RandUtils::rand());
	}

	static inline Vec3D random(const Interval& interval) {
		return Vec3D(RandUtils::rand(interval), RandUtils::rand(interval), RandUtils::rand(interval));
	}

	inline static Vec3D random_in_unit_sphere() {
		while (true) {
			Vec3D p = Vec3D::random(Interval( - 1, 1));
			if (p.length_squared() < 1)
				return p;
		}
	}

	inline static Vec3D random_unit_vec() {
		return Vec3D::random_in_unit_sphere().unit_vec();
	}

	inline static Vec3D random_on_hemisphere(const Vec3D& normal) {
		Vec3D on_unit_sphere = Vec3D::random_unit_vec();
		if (normal.dot(on_unit_sphere) > 0.0) // In the same hemisphere as the normal
			return on_unit_sphere;
		else
			return on_unit_sphere * -1;
	}

	inline bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		auto s = 1e-8;
		return (fabs(m_x) < s) && ((m_y) < s) && ((m_z) < s);
	}

	inline static Vec3D reflect(const Vec3D& v, const Vec3D& n) {
		return v - (n * (2 * v.dot(n)));
	}

private:
	double m_x, m_y, m_z;
};

inline Vec3D operator*(double d, const Vec3D& other) {
	return other * d;
}
inline Vec3D operator*(const Vec3D& u, const Vec3D& v) {
	return Vec3D(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

using Color3D = Vec3D;
