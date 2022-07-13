#pragma once
#include <iostream>

#include "NeuralNetworkModel.h"
#include "GeneticCPP.h"
#include "MergeSort.h"

struct NeuralNetworkPopulation {

	int* NetworkLayers;
	int NumberOfNetworkLayers;

	int NumberOfModelsPerEpoch;
	int NumberOfEpochs;
	int NumberOfTestsPerModel;

	double FractionOfParentsToBearChildren;
	double MaxMutationRate;

	NeuralNetworkModel** GenePool;

	NeuralNetworkPopulation(int* NetworkLayers_, int NumberOfNetworkLayers_, int NumberOfModelsPerEpoch_, int NumberOfEpochs_, int NumberOfTestsPerModel_, double FractionOfParentsToBearChildren_, double MaxMutationRate_) {

		NetworkLayers = NetworkLayers_;
		NumberOfNetworkLayers = NumberOfNetworkLayers_;
		NumberOfModelsPerEpoch = NumberOfModelsPerEpoch_;
		NumberOfEpochs = NumberOfEpochs_;
		NumberOfTestsPerModel = NumberOfTestsPerModel_;

		FractionOfParentsToBearChildren = FractionOfParentsToBearChildren_;
		MaxMutationRate = MaxMutationRate_;

		GenePool = (NeuralNetworkModel**)malloc(sizeof(NeuralNetworkModel**) * NumberOfModelsPerEpoch);
		for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {
			GenePool[ID] = new NeuralNetworkModel(NetworkLayers, NumberOfNetworkLayers);
		}
	}
	void TrainNeuralNetworkPopulation(double** Input, double** CorrectInput) {

		for (int Epoch = 0; Epoch < NumberOfEpochs; Epoch++) {


			for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {

				Genetic::TestModel(GenePool[ID], Input, CorrectInput, NumberOfTestsPerModel);
			}

			mergeSort(GenePool, 0, NumberOfModelsPerEpoch - 1);

			std::cout << "Current Generation = " << Epoch << "   Best Model = " << GenePool[NumberOfModelsPerEpoch - 1]->ModelFitness << "\n";

			if (Epoch == NumberOfEpochs - 1) {
				break;
			}

			int ParentChildDivisor = NumberOfModelsPerEpoch * (1 - FractionOfParentsToBearChildren);

			for (int ChildID = 0; ChildID < ParentChildDivisor; ChildID++) {

				int RandomParent1 = GenerateRandomDoubleFast(ParentChildDivisor, NumberOfModelsPerEpoch - 1);
				int RandomParent2 = GenerateRandomDoubleFast(ParentChildDivisor, NumberOfModelsPerEpoch - 1);

				Genetic::BreedModelsUsingSinglePointCrosover(GenePool[ChildID], GenePool[RandomParent1], GenePool[RandomParent2]);

				Genetic::MutateModelUsingCustomMutation(GenePool[ChildID], MaxMutationRate);
			}

		}
	}
};