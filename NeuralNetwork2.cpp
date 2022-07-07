#include <iostream>

#include "NeuralNetworkModel.h"
#include "GeneticCPP.h"
#include "MergeSort.h"

void main() {

	int NetworkLayers[] = { 3,10,1 };
	int NumberOfModelsPerEpoch = 10000;
	int NumberOfEpochs = 100;
	int NumberOfTestsPerModel = 100;

	double FractionOfParentsToBearChildren = 0.2;
	double MaxMutationRate = 0.1;

	if (NumberOfModelsPerEpoch % 2 != 0) {
		std::cout << "FUCK YOU\n";
		return;
	}

	//NeuralNetworkModel* TestModel1 = new NeuralNetworkModel(NetworkLayers, sizeof(NetworkLayers) / sizeof(int));
	//TestModel1->PrintModel();
	//Genetic::MutateModelUsingCustomMutation(TestModel1, MaxMutationRate);
	//TestModel1->PrintModel();
	//NeuralNetworkModel* TestModel2 = new NeuralNetworkModel(NetworkLayers, sizeof(NetworkLayers) / sizeof(int));
	//NeuralNetworkModel* ChildModel = new NeuralNetworkModel(NetworkLayers, sizeof(NetworkLayers) / sizeof(int));
	//ChildModel->PrintModel();
	//Genetic::BreedModelsUsingUniformCrosover(ChildModel, TestModel1, TestModel2);
	//ChildModel->PrintModel();
	//Genetic::MutateModelUsingCustomMutation(ChildModel, MaxMutationRate);
	//ChildModel->PrintModel();
	//return;


	NeuralNetworkModel** GenePool = (NeuralNetworkModel**)malloc(0);

	if (sizeof(NetworkLayers) == sizeof(unsigned int)) {
		std::cout << "Error NetworkLayers Has To Be A Constant Array";
		return;
	}

	GenePool = (NeuralNetworkModel**)malloc(sizeof(NeuralNetworkModel**) * NumberOfModelsPerEpoch);
	for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {
		GenePool[ID] = new NeuralNetworkModel(NetworkLayers, sizeof(NetworkLayers) / sizeof(int));
	}

	double** Input = (double**)malloc(NumberOfTestsPerModel * sizeof(double*));
	double** CorrectInput = (double**)malloc(NumberOfTestsPerModel * sizeof(double*));

	for (int Test = 0; Test < NumberOfTestsPerModel; Test++) {

		Input[Test] = (double*)malloc(NetworkLayers[0] * sizeof(double));
		CorrectInput[Test] = (double*)malloc(NetworkLayers[(sizeof(NetworkLayers) / sizeof(NetworkLayers[0])) - 1] * sizeof(double));

		for (int InputInInput = 0; InputInInput < NetworkLayers[0]; InputInInput++) {
			Input[Test][InputInInput] = GenerateRandomDoubleFast(0, 100);
		}
		for (int InputInInput = 0; InputInInput < NetworkLayers[(sizeof(NetworkLayers) / sizeof(NetworkLayers[0])) - 1]; InputInInput++) {
			CorrectInput[Test][InputInInput] = Input[Test][2 * InputInInput] + (Input[Test][2 * InputInInput + 1]) + (Input[Test][2 * InputInInput + 2]);
			int aaa = 0;
		}

	}

	for (int Epoch = 0; Epoch < NumberOfEpochs; Epoch++) {


		for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {

			Genetic::TestModel(GenePool[ID], Input, CorrectInput, NumberOfTestsPerModel);
		}

		mergeSort(GenePool, 0,NumberOfModelsPerEpoch-1);

		//GenePool[NumberOfModelsPerEpoch - 1]->PrintModel();
		std::cout << "Current Generation = " << Epoch << "   Best Model = " << GenePool[NumberOfModelsPerEpoch - 1]->ModelFitness << "";

		double TestInput[] = { 2.0,2.0,2.0 };
		GenePool[NumberOfModelsPerEpoch - 1]->RunNetwork(TestInput);
		std::cout << "   2+2+2 = " << GenePool[NumberOfModelsPerEpoch - 1]->TwoDArrayOfAllOutputs[GenePool[NumberOfModelsPerEpoch - 1]->NetworkLayers-1][0];
		
		/*std::cout << "    ";
		for (int Layer = 1; Layer < GenePool[NumberOfModelsPerEpoch - 1]->NetworkLayers; Layer++) {
			std::cout << " ";
			for (int Weight = 0; Weight < GenePool[NumberOfModelsPerEpoch - 1]->NetworkDef[Layer] * GenePool[NumberOfModelsPerEpoch - 1]->NetworkDef[Layer - 1]; Weight++) {
				std::cout << GenePool[NumberOfModelsPerEpoch - 1]->TwoDArrayOfAllWeights[Layer][Weight] << " ";
			}
		}*/
		std::cout << "\n";

		if (Epoch == NumberOfEpochs - 1) {
			break;
		}

		int ParentChildDivisor = NumberOfModelsPerEpoch * (1-FractionOfParentsToBearChildren);

		for (int ChildID = 0; ChildID < ParentChildDivisor; ChildID++) {

			int RandomParent1 = GenerateRandomDoubleFast(ParentChildDivisor, NumberOfModelsPerEpoch - 1);
			int RandomParent2 = GenerateRandomDoubleFast(ParentChildDivisor, NumberOfModelsPerEpoch - 1);

			Genetic::BreedModelsUsingSinglePointCrosover(GenePool[ChildID], GenePool[RandomParent1], GenePool[RandomParent2]);
			
			Genetic::MutateModelUsingCustomMutation(GenePool[ChildID], MaxMutationRate);
		}

	}

	GenePool[NumberOfModelsPerEpoch - 1]->PrintModel();
	while (true) {
		std::cout << "\nEnter Numbers \n";
		double Test[3];
		std::cin >> Test[0];
		std::cin >> Test[1];
		std::cin >> Test[2];
		GenePool[NumberOfModelsPerEpoch - 1]->RunNetwork(Test);
		std::cout << GenePool[NumberOfModelsPerEpoch - 1]->TwoDArrayOfAllOutputs[sizeof(NetworkLayers) / sizeof(int) - 1][0];
	}


	free(GenePool);
	std::cout << "\nEND!\n";
}

