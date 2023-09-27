#pragma once
#include <limits>



class Interval
{
public:
	static constexpr double c_INFINITY = std::numeric_limits<double>::infinity();

	inline constexpr Interval() : m_min(-Interval::c_INFINITY), m_max(Interval::c_INFINITY) {};

	inline Interval(double min, double max) : m_min(min), m_max(max) {};

	inline bool contains(double x) const {
		return m_min <= x && x <= m_max;
	}

	inline bool surrounds(double x) const {
		return m_min < x && x < m_max;
	}

	inline double max() const { return m_max; }

	inline double min() const{ return m_min; }
private:
	double m_min;
	double m_max;

public:
	static const Interval EMPTY, REALS;
};

