#include "tspBB.h"
#include<iostream>

int main()
{
	tspBB *testUnit = new tspBB();
	int option;
	std::cout << "1 - Test algorithm\n2 - Make measurements\n";
	std::cin >> option;
	switch (option)
	{
	case 1:
	{
		testUnit->testAlgorithm();
		std::cin.ignore(2);
		break;
	}
	case 2:
	{
		testUnit->makeMeasurements();
		std::cin.ignore(2);
		break;
	}
	default:
	{
		std::cout << "Wrong input";
		std::cin.ignore(2);
		break;
	}
	}
	return 0;
}
