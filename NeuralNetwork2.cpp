#include <iostream>

#include "NeuralNetwork.h"
#include "ParseCSVForNeuralNetwork.h"

void main() {

	int NetworkLayers[] = { 32,50,50,1 };
	int NumberOfModelsPerEpoch = 10000;
	int NumberOfEpochs = 200;
	int NumberOfTestsPerModel = 3;

	double FractionOfParentsToBearChildren = 0.2;
	double MaxMutationRate = 0.001;

	
	NeuralNetworkPopulation* Network = new NeuralNetworkPopulation(
		&NetworkLayers[0],
		(int)(sizeof(NetworkLayers) / sizeof(int)),
		NumberOfModelsPerEpoch,
		NumberOfEpochs,
		NumberOfTestsPerModel,
		FractionOfParentsToBearChildren,
		MaxMutationRate
	);

	CSV_NeuralNetworkData* Data = new CSV_NeuralNetworkData(
		&NetworkLayers[0],
		(int)(sizeof(NetworkLayers) / sizeof(int)),
		999999.0,
		33,
		60,
		"C:/Users/somet/Desktop/NeuralNetwork2/countries_test_data.csv"
	);
	//Data->double_Print2DArray(Data->NetworkInputArray, 32, 60);
	Network->TrainNeuralNetworkPopulation(Data->NetworkInputArray, Data->NetworkCorrectArray);
		

	Network->GenePool[NumberOfModelsPerEpoch - 1]->PrintModel();
	while (true) {
		std::cout << "\nEnter Numbers \n";
		double Test[2];
		std::cin >> Test[0];
		std::cin >> Test[1];
		Network->GenePool[NumberOfModelsPerEpoch - 1]->RunNetwork(Test);
		std::cout << Network->GenePool[NumberOfModelsPerEpoch - 1]->TwoDArrayOfAllOutputs[sizeof(NetworkLayers) / sizeof(int) - 1][0];
	}


	free(Network->GenePool);
	std::cout << "\nEND!\n";
}

