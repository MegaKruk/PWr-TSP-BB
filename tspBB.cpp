#include "Stopwatch.h"
#include "tspBB.h"

// znajduje najtañsz¹ krawêdz wierzcho³ka i
int tspBB::findFirstLowest(int adjacencyMatrix[noOfCities][noOfCities], int i)
{
	int fistLowest = INT_MAX;
	for (int k = 0; k < noOfCities; k++)
		if (adjacencyMatrix[i][k] < fistLowest && i != k)
			fistLowest = adjacencyMatrix[i][k];
	return fistLowest;
}

// znajduje drug¹ najtañsz¹ krawêdz wierzcho³ka i
int tspBB::findSecondLowest(int adjacencyMatrix[noOfCities][noOfCities], int i)
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

// zastêpuje najlepsze dotychczasowe rozwi¹zanie obecnym
void tspBB::currToBest(int currentPath[])
{
	for (int i = 0; i < noOfCities; i++)
		calcPath[i] = currentPath[i];
	calcPath[noOfCities] = currentPath[0];
}

// current bound to tutaj lower bound dla korzenia
void tspBB::recurrence(int adjacancyMatrix[noOfCities][noOfCities], int currentBound, int currentCost, int currentHeight, int currentPath[])
{
	// przypadek - odwiedziliœmy wszystkie wierzcho³ki po razie
	if (currentHeight == noOfCities)
	{
		// czy mo¿emy zamkn¹æ cykl miêdzy ostatnim, a pierwszym wierzcho³kiem
		if (adjacancyMatrix[currentPath[currentHeight - 1]][currentPath[0]] != 0)
		{
			int tmpCost = currentCost + adjacancyMatrix[currentPath[currentHeight - 1]][currentPath[0]];

			// jeœli znaleliœmy tañsz¹ drogê nadpisujemy rozwi¹zanie
			if (tmpCost < calcCost)
			{
				currToBest(currentPath);
				calcCost = tmpCost;
			}
		}
		return;
	}

	// w ka¿dym innym przypadku kontynuujemy rekurencjê po kolejnych wierzcho³kach
	for (int i = 0; i < noOfCities; i++)
	{
		// sprawdzamy, czy obecne miejsce w macierzy nie jest tym samym miastem, które ju¿ odwiedzono lub czy jest na przek¹tnej
		if (adjacancyMatrix[currentPath[currentHeight - 1]][i] != 0 && isNodeVisited[i] == false)
		{
			int tmp = currentBound;
			currentCost += adjacancyMatrix[currentPath[currentHeight - 1]][i];

			if (currentHeight == 1)
				currentBound -= ((findFirstLowest(adjacancyMatrix, currentPath[currentHeight-1])+findFirstLowest(adjacancyMatrix, i))/2);
			else
				currentBound -= ((findSecondLowest(adjacancyMatrix, currentPath[currentHeight-1])+findFirstLowest(adjacancyMatrix, i))/2);

			// jeœli lower bound (currentBound + currentCost) jest ni¿szy od naszego obecnego wyniku musimy kontynuowaæ poszukiwania
			if (currentBound + currentCost < calcCost)
			{
				currentPath[currentHeight] = i;
				isNodeVisited[i] = true;
				recurrence(adjacancyMatrix, currentBound, currentCost, currentHeight + 1, currentPath);
			}

			// jeœli nie to odcinamy wierzcho³ki i resetujemy tablicê stanów wierzcho³ków
			currentCost -= adjacancyMatrix[currentPath[currentHeight - 1]][i];
			currentBound = tmp;
			memset(isNodeVisited, false, sizeof(isNodeVisited));
			for (int j = 0; j <= currentHeight - 1; j++)
				isNodeVisited[currentPath[j]] = true;
		}
	}
}

// g³owna funkcja programu
void tspBB::TSP(int adjacancyMatrix[noOfCities][noOfCities])
{
	int currentPath[noOfCities + 1];

	// lower bound dla korzenia = suma wszystkich najtañszych i drugich najtañszych krawêdzi / 2
	int lowerBound = 0;
	memset(currentPath, -1, sizeof(currentPath));
	memset(isNodeVisited, 0, sizeof(currentPath));
	for (int i = 0; i < noOfCities; i++)
		lowerBound += (findFirstLowest(adjacancyMatrix, i) + findSecondLowest(adjacancyMatrix, i));

	// jeœli trzeba zaokr¹glij w górê
	if((lowerBound % 2 == 0))
		lowerBound = lowerBound / 2;
	else
		lowerBound = lowerBound / 2 + 1;

	// startujemy z wierzcho³ka (miasta) 0
	isNodeVisited[0] = true;
	currentPath[0] = 0;

	// wiêc zaczynamy algorytm od wartoœci kosztu równego 0 i od poziomu 1 wêz³a
	recurrence(adjacancyMatrix, lowerBound, 0, 1, currentPath);
}

// testowanie jednej instancji algorytmu dla danych z gr17_d.txt w folderze z kodem
void tspBB::testAlgorithm(void)
{
	int adjacencyMatrix[noOfCities][noOfCities];

	ifstream myFile("test.txt");
	for (int i = 0; i < noOfCities; i++)
	{
		cout << "\n";
		for (int j = 0; j < noOfCities; j++)
		{
			myFile >> adjacencyMatrix[i][j];
			cout << adjacencyMatrix[i][j] << " ";
		}

	}

	Stopwatch *timer = new Stopwatch();

	timer->point1 = chrono::high_resolution_clock::now();
	TSP(adjacencyMatrix);
	cout << endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
	//cout << "Link: https://people.sc.fsu.edu/~jburkardt/datasets/tsp/tsp.html";
	//cout << "\nGR17 is a set of 17 cities, from TSPLIB.\nThe minimal tour has length 2085.\n\n";
	cout << "Calculated minimal cost: " << calcCost << endl;
	cout << "Path: ";
	for (int i = 0; i <= noOfCities; i++)
		cout << calcPath[i];
}

// 100 pomiarów czasu oraz eksport pomiarów do pliku output.txt
void tspBB::makeMeasurements(void)
{
	int adjacencyMatrix[noOfCities][noOfCities];

	ifstream myFile("test.txt");
	for (int i = 0; i < noOfCities; i++)
		for (int j = 0; j < noOfCities; j++)
			myFile >> adjacencyMatrix[i][j];

	Stopwatch *timer = new Stopwatch();
	ofstream myOutput("output.txt");

	for (int i = 0; i < 51; i++)
	{
		timer->point1 = chrono::high_resolution_clock::now();
		TSP(adjacencyMatrix);
		if(i)
		myOutput << timer->countTimeDiff() << "\t" << calcCost << endl;
	}
	cout << "Done";
}

tspBB::tspBB()
{

}

tspBB::~tspBB()
{

}
