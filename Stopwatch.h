#pragma once
#include<chrono>
using namespace std;

class Stopwatch
{
public:
	chrono::high_resolution_clock::time_point point1;
	chrono::high_resolution_clock::time_point point2;
	long double diff;
	long double countTimeDiff();
	Stopwatch();
	~Stopwatch();
};

