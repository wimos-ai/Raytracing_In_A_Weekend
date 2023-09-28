#include "Utills.h"
#include <cstdlib>
#include <ctime>

namespace {
	bool seed_rand() {
		std::srand(time(nullptr));
		return true;
	}
}


bool seeded_rand = seed_rand();

double RandUtils::rand()
{
    return std::rand() / (RAND_MAX + 1.0);
}

double RandUtils::rand(const Interval& in)
{
    return in.min() + (in.max() - in.min()) * RandUtils::rand();
}
