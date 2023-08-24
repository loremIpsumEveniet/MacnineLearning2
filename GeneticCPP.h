#pragma once
#include "NeuralNetworkModel.h"
#include "cmath"

namespace Genetic {

	void CopyModels(NeuralNetworkModel* ModelTo, NeuralNetworkModel* ModelFrom) {
		for (int Layer = 1; Layer < ModelTo->NumberOfNetworkLayers; Layer++) {
			for (int Weight = 0; Weight < ModelTo->NetworkLayersNumberOfNeuronsDefinitions[Layer] * ModelTo->NetworkLayersNumberOfNeuronsDefinitions[Layer - 1]; Weight++) {
				ModelTo->TwoDArrayOfAllWeights[Layer][Weight] = ModelFrom->TwoDArrayOfAllWeights[Layer][Weight];
			}
		}
	}
	void MutateToDefaultModel(NeuralNetworkModel* Model, int ProbablityOfIndividualGeneMutation) {

		for (int Layer = 1; Layer < Model->NumberOfNetworkLayers; Layer++) {
			for (int Weight = 0; Weight < Model->NetworkLayersNumberOfNeuronsDefinitions[Layer] * Model->NetworkLayersNumberOfNeuronsDefinitions[Layer - 1]; Weight++) {

				int Threshold = round(GenerateRandomDoubleFast(0, 100));
				if (Threshold <= ProbablityOfIndividualGeneMutation) {
					Model->TwoDArrayOfAllWeights[Layer][Weight] = GenerateRandomDoubleFast(0, 1);
				}
			}
		}
	}
	void MutateModel(NeuralNetworkModel* Model, double MutationChangePercent_) {

		//double MutationChangeToPercent = 1 - (std::min(0.9999, MutationChange));
		double MutationChangePercent = std::max((MutationChangePercent_ / 100), 0.0);

		for (int Layer = 1; Layer < Model->NumberOfNetworkLayers; Layer++) {

			for (int Bias = 0; Bias < Model->NetworkLayersNumberOfNeuronsDefinitions[Layer]; Bias++) {

				double CurrentBias = Model->TwoDArrayOfAllWeights[Layer][Bias];
				double MutationRandomDelta = GenerateRandomDoubleFast(-MutationChangePercent * CurrentBias, MutationChangePercent * CurrentBias);

				//Model->TwoDArrayOfAllBiases[Layer][Bias] += MutationSign * (MutationActual * Model->TwoDArrayOfAllBiases[Layer][Bias]);
				Model->TwoDArrayOfAllBiases[Layer][Bias] += MutationRandomDelta;

			}
			for (int Weight = 0; Weight < Model->NetworkLayersNumberOfNeuronsDefinitions[Layer] * Model->NetworkLayersNumberOfNeuronsDefinitions[Layer - 1]; Weight++) {

				double CurrentWeight = Model->TwoDArrayOfAllWeights[Layer][Weight];
				double MutationRandomDelta = GenerateRandomDoubleFast(-MutationChangePercent * CurrentWeight, MutationChangePercent * CurrentWeight);

				//Model->TwoDArrayOfAllWeights[Layer][Weight] += MutationSign * (MutationActual * Model->TwoDArrayOfAllWeights[Layer][Weight]);
				Model->TwoDArrayOfAllWeights[Layer][Weight] += MutationRandomDelta;
			}
		}
	}

	void BreedModelsUsingUniformCrosover(NeuralNetworkModel* Child1, NeuralNetworkModel* Parent1, NeuralNetworkModel* Parent2) {

		for (int Layer = 1; Layer < Child1->NumberOfNetworkLayers; Layer++) {

			for (int Bias = 0; Bias < Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer]; Bias++) {

				int Seed = round(GenerateRandomDoubleFast(0, 100));

				if (Seed <= 49) {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent1->TwoDArrayOfAllBiases[Layer][Bias];
				}
				else {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent2->TwoDArrayOfAllBiases[Layer][Bias];
				}
			}

			for (int Weight = 0; Weight < Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer] * Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer - 1]; Weight++) {

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

		for (int Layer = 1; Layer < Child1->NumberOfNetworkLayers; Layer++) {

			for (int Bias = 0; Bias < Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer]; Bias++) {

				int Seed = round(GenerateRandomDoubleFast(0, 100));

				if (Seed <= 49) {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent1->TwoDArrayOfAllBiases[Layer][Bias];
				}
				else {
					Child1->TwoDArrayOfAllBiases[Layer][Bias] = Parent2->TwoDArrayOfAllBiases[Layer][Bias];
				}
			}

			int Seed = round(GenerateRandomDoubleFast(1, Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer] * Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer - 1]));

			for (int Weight = 0; Weight < Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer] * Child1->NetworkLayersNumberOfNeuronsDefinitions[Layer - 1]; Weight++) {

				if (Weight <= Seed) {
					Child1->TwoDArrayOfAllWeights[Layer][Weight] = Parent1->TwoDArrayOfAllWeights[Layer][Weight];
				}
				else {
					Child1->TwoDArrayOfAllWeights[Layer][Weight] = Parent2->TwoDArrayOfAllWeights[Layer][Weight];
				}
			}
		}
	}

	void TestModel(NeuralNetworkModel* Model, double** Input, double** CorrectInput, int NumberOfTests, int DataSize_Y_ZEROINDEX, double (*FitnessFunction)(double* ModelOutput, double* CorrectOutput, int SizeOfOutput, double NumberOfTests)) {

		double ModelFitness = 0;

		for (int Test = 0; Test < NumberOfTests; Test++) {

			int RandomDataLine = (int)GenerateRandomDoubleFast(0, DataSize_Y_ZEROINDEX - 1);

			Model->RunNetwork(Input[RandomDataLine]);

			double* CorrectAnswer = CorrectInput[RandomDataLine];
			double* ModelNodeOutput = Model->TwoDArrayOfAllOutputs[Model->NumberOfNetworkLayers - 1];

			ModelFitness += FitnessFunction(ModelNodeOutput, CorrectAnswer, Model->NetworkLayersNumberOfNeuronsDefinitions[Model->NumberOfNetworkLayers - 1], NumberOfTests);

		}

		Model->ModelFitness = ModelFitness;
	}
}