#pragma once
#include<chrono>
using namespace std;

class Stopwatch
{
private:
	long double diff;

public:
	chrono::high_resolution_clock::time_point point1;
	chrono::high_resolution_clock::time_point point2;

public:
	long double countTimeDiff();
	Stopwatch();
	~Stopwatch();
};

