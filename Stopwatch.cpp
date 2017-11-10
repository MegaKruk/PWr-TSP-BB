#include "Stopwatch.h"
#include <chrono>

Stopwatch::Stopwatch()
{
	point1 = chrono::high_resolution_clock::now();
}

long double Stopwatch::countTimeDiff()
{
	chrono::high_resolution_clock::time_point point2 = chrono::high_resolution_clock::now();
	chrono::duration<long double, nano> diff = point2 - point1;
	return diff.count();
}

Stopwatch::~Stopwatch()
{
	delete &point1;
}
