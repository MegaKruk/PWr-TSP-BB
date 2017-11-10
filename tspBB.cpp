#include<stdlib.h>
#include<string>
#include<limits.h>
#include<iostream>
#include<fstream>
#include "Stopwatch.h"

const int noOfCities = 10;
int calcPath[noOfCities + 1];
int calcCost = INT_MAX;
bool isNodeVisited[noOfCities];

// znajduje najta�sz� kraw�dz wierzcho�ka i
int findFirstLowest(int adjacencyMatrix[noOfCities][noOfCities], int i) 
{
	int fistLowest = INT_MAX;
	for (int k = 0; k < noOfCities; k++)
		if (adjacencyMatrix[i][k] < fistLowest && i != k)
			fistLowest = adjacencyMatrix[i][k];
	return fistLowest;
}

// znajduje drug� najta�sz� kraw�dz wierzcho�ka i
int findSecondLowest(int adjacencyMatrix[noOfCities][noOfCities], int i) 
{
	int firstLowest = INT_MAX, secondLowest = INT_MAX;
	for (int j = 0; j < noOfCities; j++)
	{
		if (i == j)
			continue;

		if (adjacencyMatrix[i][j] <= firstLowest)
		{
			secondLowest = firstLowest;
			firstLowest = adjacencyMatrix[i][j];
		}
		else if (adjacencyMatrix[i][j] <= secondLowest &&
			adjacencyMatrix[i][j] != firstLowest)
			secondLowest = adjacencyMatrix[i][j];
	}
	return secondLowest;
}

// zast�puje najlepsze dotychczasowe rozwi�zanie obecnym
void currToBest(int currentPath[]) 
{
	for (int i = 0; i < noOfCities; i++)
		calcPath[i] = currentPath[i];
	calcPath[noOfCities] = currentPath[0];
}

// current bound to tutaj lower bound dla korzenia
void recurrence(int adjacancyMatrix[noOfCities][noOfCities], int currentBound, int currentCost, int currentHeight, int currentPath[]) 
{
	// przypadek - odwiedzili�my wszystkie wierzcho�ki po razie
	if (currentHeight == noOfCities)
	{
		// czy mo�emy zamkn�� cykl mi�dzy ostatnim, a pierwszym wierzcho�kiem
		if (adjacancyMatrix[currentPath[currentHeight - 1]][currentPath[0]] != 0)
		{
			int tmpCost = currentCost + adjacancyMatrix[currentPath[currentHeight - 1]][currentPath[0]];

			// je�li znaleli�my ta�sz� drog� nadpisujemy rozwi�zanie
			if (tmpCost < calcCost)
			{
				currToBest(currentPath);
				calcCost = tmpCost;
			}
		}
		return;
	}

	// w ka�dym innym przypadku kontynuujemy rekurencj� po kolejnych wierzcho�kach
	for (int i = 0; i < noOfCities; i++)
	{
		// sprawdzamy, czy obecne miejsce w macierzy nie jest tym samym miastem, kt�re ju� odwiedzono lub czy jest na przek�tnej
		if (adjacancyMatrix[currentPath[currentHeight - 1]][i] != 0 && isNodeVisited[i] == false)
		{
			int tmp = currentBound;
			currentCost += adjacancyMatrix[currentPath[currentHeight - 1]][i];

			if (currentHeight == 1)
				currentBound -= ((findFirstLowest(adjacancyMatrix, currentPath[currentHeight-1])+findFirstLowest(adjacancyMatrix, i))/2);
			else
				currentBound -= ((findSecondLowest(adjacancyMatrix, currentPath[currentHeight-1])+findFirstLowest(adjacancyMatrix, i))/2);

			// je�li lower bound (currentBound + currentCost) jest ni�szy od naszego obecnego wyniku musimy kontynuowa� poszukiwania
			if (currentBound + currentCost < calcCost)
			{
				currentPath[currentHeight] = i;
				isNodeVisited[i] = true;
				recurrence(adjacancyMatrix, currentBound, currentCost, currentHeight + 1, currentPath);
			}

			// je�li nie to odcinamy wierzcho�ki i resetujemy tablic� stan�w wierzcho�k�w
			currentCost -= adjacancyMatrix[currentPath[currentHeight - 1]][i];
			currentBound = tmp;
			memset(isNodeVisited, false, sizeof(isNodeVisited));
			for (int j = 0; j <= currentHeight - 1; j++)
				isNodeVisited[currentPath[j]] = true;
		}
	}
}

// g�owna funkcja programu
void TSP(int adjacancyMatrix[noOfCities][noOfCities])
{
	int currentPath[noOfCities + 1];

	// lower bound dla korzenia = suma wszystkich najta�szych i drugich najta�szych kraw�dzi / 2
	int lowerBound = 0;
	memset(currentPath, -1, sizeof(currentPath));
	memset(isNodeVisited, 0, sizeof(currentPath));
	for (int i = 0; i < noOfCities; i++)
		lowerBound += (findFirstLowest(adjacancyMatrix, i) + findSecondLowest(adjacancyMatrix, i));

	// je�li trzeba zaokr�glij w g�r�
	if((lowerBound % 2 == 0))
		lowerBound = lowerBound / 2;
	else
		lowerBound = lowerBound / 2 + 1;

	// startujemy z wierzcho�ka (miasta) 0
	isNodeVisited[0] = true;
	currentPath[0] = 0;

	// wi�c zaczynamy algorytm od warto�ci kosztu r�wnego 0 i od poziomu 1 w�z�a
	recurrence(adjacancyMatrix, lowerBound, 0, 1, currentPath);
}

// testowanie jednej instancji algorytmu dla danych z gr17_d.txt w folderze z kodem
void testAlgorithm(void)
{
	int adjacencyMatrix[noOfCities][noOfCities];

	ifstream myFile("gr17_d.txt");
	for (int i = 0; i < noOfCities; i++)
		for (int j = 0; j < noOfCities; j++)
			myFile >> adjacencyMatrix[i][j];

	Stopwatch *timer = new Stopwatch();

	timer->point1 = chrono::high_resolution_clock::now();
	TSP(adjacencyMatrix);
	cout << timer->countTimeDiff() << " nanosecs to complete this action\n";
	//cout << "Link: https://people.sc.fsu.edu/~jburkardt/datasets/tsp/tsp.html";
	//cout << "\nGR17 is a set of 17 cities, from TSPLIB.\nThe minimal tour has length 2085.\n\n";
	cout << "Calculated minimal cost: " << calcCost << endl;
	cout << "Path: ";
	for (int i = 0; i <= noOfCities; i++)
		cout << calcPath[i];
}

// 100 pomiar�w czasu oraz eksport pomiar�w do pliku output.txt
void makeMeasurements(void)
{
	int adjacencyMatrix[noOfCities][noOfCities];

	ifstream myFile("test.txt");
	for (int i = 0; i < noOfCities; i++)
		for (int j = 0; j < noOfCities; j++)
			myFile >> adjacencyMatrix[i][j];

	Stopwatch *timer = new Stopwatch();
	ofstream myOutput("output.txt");

	for (int i = 0; i < 50; i++)
	{
		timer->point1 = chrono::high_resolution_clock::now();
		TSP(adjacencyMatrix);
		myOutput << timer->countTimeDiff() << "\t" << calcCost << endl;
	}
	cout << "Done";
}

int main()
{
	int option;
	cout << "1 - Test algorithm\n2 - Make measurements\n";
	cin >> option;
	switch (option)
	{
	case 1:
	{
		testAlgorithm();
		std::cin.ignore(2);
		break;
	}
	case 2:
	{
		makeMeasurements();
		std::cin.ignore(2);
		break;
	}
	default:
	{
		cout << "Wrong input";
		std::cin.ignore(2);
		break;
	}
	}
	return 0;
}
