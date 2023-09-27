#pragma once
#include "Vec3D.h"
#include "Interval.h"





namespace LAlgUtils {
	Vec3D rotateVector(const Vec3D& in, double alpha, double beta, double gamma);
}

namespace RandUtils {
	double rand();
	double rand(Interval in);
}
