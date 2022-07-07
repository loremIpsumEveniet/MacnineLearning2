#pragma once

#include <random>

//double BetterRandomDouble(double Min, double Max) {
//	std::random_device rd;
//	std::default_random_engine eng(rd());
//	std::uniform_real_distribution<float> distr(Min, Max);
//	return distr(eng);
//}

double GenerateRandomDoubleFast(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

struct NeuralNetworkModel {

	double ModelFitness = -1;
	int* NetworkDef;
	int NetworkLayers;
	double** TwoDArrayOfAllWeights;
	double** TwoDArrayOfAllBiases;

	double** TwoDArrayOfAllOutputs;

	NeuralNetworkModel(int* NeuranNetworkModel, int NumberOfLayers) {

		if (NumberOfLayers > 0) {

			NetworkDef = (int*)calloc(NumberOfLayers, sizeof(int));
			memcpy(NetworkDef, NeuranNetworkModel, NumberOfLayers * sizeof(int));
			NetworkLayers = NumberOfLayers;

			TwoDArrayOfAllWeights = (double**)malloc(NumberOfLayers * sizeof(double**));
			TwoDArrayOfAllOutputs = (double**)malloc(NumberOfLayers * sizeof(double**));
			TwoDArrayOfAllBiases = (double**)malloc(NumberOfLayers * sizeof(double**));

			TwoDArrayOfAllWeights[0] = nullptr;
			TwoDArrayOfAllBiases[0] = nullptr;
			TwoDArrayOfAllOutputs[0] = (double*)calloc(NeuranNetworkModel[0], sizeof(double));

			for (int Layer = 1; Layer < NumberOfLayers; Layer++) {

				TwoDArrayOfAllWeights[Layer] = (double*)malloc(NetworkDef[Layer] * NetworkDef[Layer -1] * sizeof(double));
				TwoDArrayOfAllBiases[Layer] = (double*)malloc(NetworkDef[Layer] * sizeof(double));

				TwoDArrayOfAllOutputs[Layer] = (double*)calloc(NetworkDef[Layer], sizeof(double));

				for (int Bias = 0; Bias < NetworkDef[Layer]; Bias++) {

					TwoDArrayOfAllBiases[Layer][Bias] = GenerateRandomDoubleFast(0, 1);
					//TwoDArrayOfAllBiases[Layer][Bias] = 1.0;
				}

				for (int Weight = 0; Weight < NetworkDef[Layer] * NetworkDef[Layer - 1]; Weight++) {

					TwoDArrayOfAllWeights[Layer][Weight] = GenerateRandomDoubleFast(0, 1);

				}
			}
		}
	}
	void RunNetwork(double* Input) {
		

		for (int Index = 0; Index < NetworkDef[0]; Index++) {
			TwoDArrayOfAllOutputs[0][Index] = Input[Index];
		}

		for (int Layer = 1; Layer < NetworkLayers; Layer++) { //For Every Layer

			for (int Node = 0; Node < NetworkDef[Layer]; Node++) { //Clear Prev Output

				for (int PrevNode = 0; PrevNode < NetworkDef[Layer - 1]; PrevNode++) {

					TwoDArrayOfAllOutputs[Layer][Node] = 0;
				}
			}
			for (int Node = 0; Node < NetworkDef[Layer]; Node++) { //For Every Node In Layer

				for (int PrevNode = 0; PrevNode < NetworkDef[Layer - 1]; PrevNode++) { //For All PrevNode Per Node

					TwoDArrayOfAllOutputs[Layer][Node] += TwoDArrayOfAllOutputs[Layer - 1][PrevNode] * TwoDArrayOfAllWeights[Layer][PrevNode * (Node+1)];
				}
				//TwoDArrayOfAllOutputs[Layer][Node] += TwoDArrayOfAllBiases[Layer][Node];
			}
		}
	}

	void PrintModel() {

		std::cout << "\n";
		std::cout << NetworkLayers << " Layer Network\n";

		std::cout << "\nLayer " << 0 << " Inputs\n";
		for (int ii = 0; ii < NetworkDef[0]; ii++) {

			std::cout << TwoDArrayOfAllOutputs[0][ii] << " ";
		}
		for (int i = 1; i < NetworkLayers; i++) {

			std::cout << "\nLayer " << i << " Weights\n";
			for (int ii = 0; ii < NetworkDef[i] * NetworkDef[i-1]; ii++) {

				std::cout << TwoDArrayOfAllWeights[i][ii] << " ";
			}
			std::cout << "\nLayer " << i << " Biases\n";
			for (int ii = 0; ii < NetworkDef[i]; ii++) {

				std::cout << TwoDArrayOfAllBiases[i][ii] << " ";
			}
			std::cout << "\nLayer " << i << " Outputs\n";
			for (int ii = 0; ii < NetworkDef[i]; ii++) {

				std::cout << TwoDArrayOfAllOutputs[i][ii] << " ";
			}
		}
		std::cout << "\n\n";
	}
};

