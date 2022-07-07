#pragma once
#include "NeuralNetworkModel.h"
#include "cmath"

namespace Genetic {

	void CopyModels(NeuralNetworkModel* ModelTo, NeuralNetworkModel* ModelFrom) {
		for (int Layer = 1; Layer < ModelTo->NetworkLayers; Layer++) {
			for (int Weight = 0; Weight < ModelTo->NetworkDef[Layer] * ModelTo->NetworkDef[Layer - 1]; Weight++) {
				ModelTo->TwoDArrayOfAllWeights[Layer][Weight] = ModelFrom->TwoDArrayOfAllWeights[Layer][Weight];
			}
		}
	}
	void MutateToDefaultModel(NeuralNetworkModel* Model, int ProbablityOfIndividualGeneMutation) {

		for (int Layer = 1; Layer < Model->NetworkLayers; Layer++) {
			for (int Weight = 0; Weight < Model->NetworkDef[Layer] * Model->NetworkDef[Layer - 1]; Weight++) {

				int Threshold = round(GenerateRandomDoubleFast(0, 100));
				if (Threshold <= ProbablityOfIndividualGeneMutation) {
					Model->TwoDArrayOfAllWeights[Layer][Weight] = GenerateRandomDoubleFast(0, 1);
				}
			}
		}
	}
	void MutateModelUsingCustomMutation(NeuralNetworkModel* Model, double MutationRate) {

		for (int Layer = 1; Layer < Model->NetworkLayers; Layer++) {

			for (int Bias = 0; Bias < Model->NetworkDef[Layer]; Bias++) {

				int MutationSign = round(GenerateRandomDoubleFast(0, 1));
				MutationSign = MutationSign > 0 ? 1 : -1;

				double MutationActual = GenerateRandomDoubleFast(0.0001, MutationRate);

				Model->TwoDArrayOfAllBiases[Layer][Bias] = Model->TwoDArrayOfAllBiases[Layer][Bias] + (MutationSign * (MutationActual * Model->TwoDArrayOfAllBiases[Layer][Bias]));

			}
			for (int Weight = 0; Weight < Model->NetworkDef[Layer] * Model->NetworkDef[Layer - 1]; Weight++) {

				int MutationSign = round(GenerateRandomDoubleFast(0, 1));
				MutationSign = MutationSign > 0 ? 1 : -1;

				double MutationActual = GenerateRandomDoubleFast(0.0001, MutationRate);

				Model->TwoDArrayOfAllWeights[Layer][Weight] = Model->TwoDArrayOfAllWeights[Layer][Weight] + (MutationSign * (MutationActual * Model->TwoDArrayOfAllWeights[Layer][Weight]));
			}
		}
	}
	void TestModel(NeuralNetworkModel* Model, double** Input, double** CorrectInput, int NumberOfTests) {


		auto CalculateAbsoluteModelFitness = [](NeuralNetworkModel* Model, double** Input, double** CorrectInput, int NumberOfTests) {

			double ModelFitness = 0;
			double TempError = 0;
			for (int Test = 0; Test < NumberOfTests; Test++) {

				Model->RunNetwork(Input[Test]);

				for (int FinalLayerNode = 0; FinalLayerNode < Model->NetworkDef[Model->NetworkLayers - 1]; FinalLayerNode++) {

					double CorrectAnswer = CorrectInput[Test][FinalLayerNode];
					double ModelNodeOutput = Model->TwoDArrayOfAllOutputs[Model->NetworkLayers - 1][FinalLayerNode];

					double Error = CorrectAnswer - ModelNodeOutput;
					TempError += Error + 0.0000000001;

				}
			}
			ModelFitness = (1 / TempError) / NumberOfTests / Model->NetworkDef[Model->NetworkLayers - 1];
		};


		auto CalculateLogarithmModelFitness = [](NeuralNetworkModel* Model, double** Input, double** CorrectInput, int NumberOfTests) {

			double ModelFitness = 0;
			double TempError = 0;
			for (int Test = 0; Test < NumberOfTests; Test++) {

				Model->RunNetwork(Input[Test]);

				for (int FinalLayerNode = 0; FinalLayerNode < Model->NetworkDef[Model->NetworkLayers - 1]; FinalLayerNode++) {

					double CorrectAnswer = CorrectInput[Test][FinalLayerNode];
					double ModelNodeOutput = Model->TwoDArrayOfAllOutputs[Model->NetworkLayers - 1][FinalLayerNode];
					TempError += pow((log(ModelNodeOutput + 1) - log(CorrectAnswer + 1)), 2);
				}
			}



			ModelFitness = 1 / (sqrt(TempError)) / NumberOfTests / Model->NetworkDef[Model->NetworkLayers - 1];

			return ModelFitness;
		};

		Model->ModelFitness = -1;

		Model->ModelFitness = CalculateLogarithmModelFitness(Model, Input, CorrectInput, NumberOfTests);

	}


	void BreedModelsUsingUniformCrosover(NeuralNetworkModel* Child1, NeuralNetworkModel* Parent1, NeuralNetworkModel* Parent2) {

		for (int Layer = 1; Layer < Child1->NetworkLayers; Layer++) {

			for (int Bias = 0; Bias < Child1->NetworkDef[Layer]; Bias++) {

				int Seed = round(GenerateRandomDoubleFast(0, 100));

				if (Seed <= 49) {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent1->TwoDArrayOfAllBiases[Layer][Bias];
				}
				else {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent2->TwoDArrayOfAllBiases[Layer][Bias];
				}
			}

			for (int Weight = 0; Weight < Child1->NetworkDef[Layer] * Child1->NetworkDef[Layer - 1]; Weight++) {

				int Seed = round(GenerateRandomDoubleFast(0, 100));

				if (Seed <= 49) {
					Child1->TwoDArrayOfAllWeights[Layer][Weight] = Parent1->TwoDArrayOfAllWeights[Layer][Weight];
				}
				else {
					Child1->TwoDArrayOfAllWeights[Layer][Weight] = Parent2->TwoDArrayOfAllWeights[Layer][Weight];
				}
			}
		}
	}
	void BreedModelsUsingSinglePointCrosover(NeuralNetworkModel* Child1, NeuralNetworkModel* Parent1, NeuralNetworkModel* Parent2) {

		for (int Layer = 1; Layer < Child1->NetworkLayers; Layer++) {

			for (int Bias = 0; Bias < Child1->NetworkDef[Layer]; Bias++) {

				int Seed = round(GenerateRandomDoubleFast(0, 100));

				if (Seed <= 49) {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent1->TwoDArrayOfAllBiases[Layer][Bias];
				}
				else {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent2->TwoDArrayOfAllBiases[Layer][Bias];
				}
			}

			int Seed = round(GenerateRandomDoubleFast(1, Child1->NetworkDef[Layer] * Child1->NetworkDef[Layer - 1]));

			for (int Weight = 0; Weight < Child1->NetworkDef[Layer] * Child1->NetworkDef[Layer - 1]; Weight++) {

				if (Weight <= Seed) {
					Child1->TwoDArrayOfAllWeights[Layer][Weight] = Parent1->TwoDArrayOfAllWeights[Layer][Weight];
				}
				else {
					Child1->TwoDArrayOfAllWeights[Layer][Weight] = Parent2->TwoDArrayOfAllWeights[Layer][Weight];
				}
			}
		}
	}

	void BubbleSortPopulationByFitness(NeuralNetworkModel** GenePool, int NumberOfModelsPerEpoch) {

		//Terrible Terible Bubble Sort
		while (true) {
			int Swaps = 0;
			for (int ID = 1; ID < NumberOfModelsPerEpoch; ID++) {
				if (GenePool[ID - 1]->ModelFitness > GenePool[ID]->ModelFitness) {

					NeuralNetworkModel* Temp = GenePool[ID - 1];
					GenePool[ID - 1] = GenePool[ID];
					GenePool[ID] = Temp;
					Swaps++;
				}
			}
			if (Swaps == 0) {
				break;
			}
		}
	}


	//void InitPopulation(NeuralNetworkModel** EmptyGenePool, int NumberOfModelsPerEpoch, int* NetworkLayers) {

	//	if (sizeof(NetworkLayers) == sizeof(unsigned int)) {
	//		std::cout << "Error NetworkLayers Has To Be A Constant Array";
	//		return;
	//	}

	//	EmptyGenePool = (NeuralNetworkModel**)malloc(sizeof(NeuralNetworkModel**) * NumberOfModelsPerEpoch);
	//	for (int ID = 0; ID < NumberOfModelsPerEpoch; ID++) {
	//		EmptyGenePool[ID] = new NeuralNetworkModel(NetworkLayers, sizeof(NetworkLayers) / sizeof(int));
	//	}
	//}
}