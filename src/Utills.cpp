#include "Utills.hpp"
#include <cstdlib>
#include <ctime>

double RandUtils::rand()
{
    return std::rand() / (RAND_MAX + 1.0);
}

double RandUtils::rand(const Interval& in)
{
    return in.min() + (in.max() - in.min()) * RandUtils::rand();
}
