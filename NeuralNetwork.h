#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>

#include "NeuralNetworkModel.h"
#include "GeneticCPP.h"
#include "MergeSort.h"
#include "ParseCSVForNeuralNetwork.h"
#include "ParseDataForNeuralNetwork.h"

struct NeuralNetworkPopulation {

	int* NetworkLayers;
	int NumberOfNetworkLayers;

	int NumberOfModelsPerEpoch;
	int NumberOfEpochs;
	int NumberOfTestsPerModel;

	double FractionOfParentsToBearChildren;
	double MaxMutationChangePercent;

	NeuralNetworkModel** GenePool;

	NeuralNetworkPopulation(int* NetworkLayers_, int NumberOfNetworkLayers_, int NumberOfModelsPerEpoch_, int NumberOfEpochs_, int NumberOfTestsPerModel_, double FractionOfParentsToBearChildren_, double MaxMutationChangePercent_) {

		NetworkLayers = NetworkLayers_;
		NumberOfNetworkLayers = NumberOfNetworkLayers_;
		NumberOfModelsPerEpoch = NumberOfModelsPerEpoch_;
		NumberOfEpochs = NumberOfEpochs_;
		NumberOfTestsPerModel = NumberOfTestsPerModel_;

		FractionOfParentsToBearChildren = FractionOfParentsToBearChildren_;
		MaxMutationChangePercent = MaxMutationChangePercent_;

		GenePool = (NeuralNetworkModel**)malloc(sizeof(NeuralNetworkModel**) * NumberOfModelsPerEpoch);
		for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {
			GenePool[ID] = new NeuralNetworkModel(NetworkLayers, NumberOfNetworkLayers);
		}
	}


	void TrainNeuralNetworkPopulation(NeuralNetworkData* DATA, double (*FitnessFunction)(double* ModelOutput, double* CorrectOutput, int SizeOfOutput, double NumberOfTests)) {
		
		double** Input = DATA->NetworkInputArray;
		double** CorrectInput = DATA->NetworkCorrectArray;
		double DataSize_Y_ZEROINDEX = DATA->DataSize_Y;

		for (int Epoch = 0; Epoch < NumberOfEpochs; Epoch++) {

			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

			double AverageFitness = 0;

			for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {

				Genetic::TestModel(GenePool[ID], Input, CorrectInput, NumberOfTestsPerModel, DataSize_Y_ZEROINDEX, FitnessFunction);

				AverageFitness += (GenePool[ID]->ModelFitness) / NumberOfModelsPerEpoch;
			}

			mergeSort(GenePool, 0, NumberOfModelsPerEpoch - 1);

			double BestModelFitness = GenePool[NumberOfModelsPerEpoch - 1]->ModelFitness;

			std::cout << "Current Generation = " << std::left << std::setw(10) << Epoch << "   Average Model = " << std::left << std::setw(15) << AverageFitness << "   Best Model = " << std::left << std::setw(15) << BestModelFitness;

			if (Epoch == NumberOfEpochs - 1) {
				break;
			}

			int ParentChildDivisor = std::min(1.0,(NumberOfModelsPerEpoch * (1 - FractionOfParentsToBearChildren)));

			for (int ChildID = 0; ChildID < ParentChildDivisor; ChildID++) {

				int RandomParent1 = GenerateRandomDoubleFast(ParentChildDivisor, NumberOfModelsPerEpoch - 1);
				int RandomParent2 = GenerateRandomDoubleFast(ParentChildDivisor, NumberOfModelsPerEpoch - 1);

				Genetic::BreedModelsUsingUniformCrosover(GenePool[ChildID], GenePool[RandomParent1], GenePool[RandomParent2]);

				Genetic::MutateModel(GenePool[ChildID], MaxMutationChangePercent);
			}
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			std::cout << " Time Stats = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000 << "[ms]" << std::endl;
		}
	
	}
};