#pragma once
#include <limits>



class Interval
{
public:
	static constexpr double INF = std::numeric_limits<double>::infinity();

	inline constexpr Interval() : m_min(-Interval::INF), m_max(Interval::INF) {};

	inline Interval(double min, double max) : m_min(min), m_max(max) {};

	inline bool contains(double x) const {
		return m_min <= x && x <= m_max;
	}

	inline bool surrounds(double x) const {
		return m_min < x && x < m_max;
	}

	inline double max() const { return m_max; }

	inline double min() const{ return m_min; }

	inline double clamp(double d) const {
		if (d < m_min)
		{
			return m_min;
		}
		else if (d > m_max)
		{
			return m_max;
		}
		return d;
	}

private:
	double m_min;
	double m_max;

public:
	static const Interval EMPTY, REALS;
};

