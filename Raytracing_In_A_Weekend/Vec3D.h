#pragma once
#include <cmath>

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

private:
	double m_x, m_y, m_z;
};

inline Vec3D operator*(double d, const Vec3D& other) {
	return other * d;
}

using Color3D = Vec3D;
