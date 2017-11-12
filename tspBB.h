#pragma once
#include<stdlib.h>
#include<string>
#include<limits.h>
#include<iostream>
#include<fstream>

class tspBB
{
private:
	static const int noOfCities = 17;

public:
	int calcPath[noOfCities + 1];
	int calcCost = INT_MAX;
	bool isNodeVisited[noOfCities];
	int adjacencyMatrix[noOfCities][noOfCities];

public:
	tspBB();
	~tspBB();
	int findFirstLowest(int adjacencyMatrix[noOfCities][noOfCities], int i);
	int findSecondLowest(int adjacencyMatrix[noOfCities][noOfCities], int i);
	void currToBest(int currentPath[]);
	void recurrence(int adjacancyMatrix[noOfCities][noOfCities], int currentBound, int currentCost, int currentHeight, int currentPath[]);
	void TSP(int adjacancyMatrix[noOfCities][noOfCities]);
	void testAlgorithm();
	void makeMeasurements();

};