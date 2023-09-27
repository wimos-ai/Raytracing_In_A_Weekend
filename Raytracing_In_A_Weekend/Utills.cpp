#include "Utills.h"
#include <cstdio>
#include <ctime>

namespace {
	bool seed_rand() {
		srand(time(nullptr));
		return true;
	}
}


bool seeded_rand = seed_rand();

double RandUtils::rand()
{
    return rand() / (RAND_MAX + 1.0);
}

double RandUtils::rand(Interval in)
{
    return in.min() + (in.max() - in.min()) * RandUtils::rand();
}
