#include "Interval.h"

const Interval Interval::REALS(-Interval::c_INFINITY, Interval::c_INFINITY);
const Interval Interval::EMPTY(Interval::c_INFINITY, -Interval::c_INFINITY);