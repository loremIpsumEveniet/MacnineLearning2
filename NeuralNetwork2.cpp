#include <iostream>

#include "NeuralNetwork.h"
#include "ParseDataForNeuralNetwork.h"

double MyFitnessFunction(double* ModelOutput, double* CorrectOutput, int SizeOfOutput, double NumberOfTests) {

	//double Error = pow((log(ModelOutput + 1) - log(CorrectOutput + 1)), 2);
	//double Error = abs(CorrectOutput - ModelOutput);
	//double Error = abs((ModelOutput - CorrectOutput)*(ModelOutput - CorrectOutput) /ModelOutput);
	//double Error = ((ModelOutput - CorrectOutput)*(ModelOutput - CorrectOutput)*(ModelOutput - CorrectOutput)*(ModelOutput - CorrectOutput));
	//double Error;
	//if (CorrectOutput < 100) {
	//	Error *= 100;
	//}
	//double Error = abs(CorrectOutput - ModelOutput);

	//double Fitness =  (std::exp(-2 * Error))/NumberOfTests;
	//return Fitness;

	double Fitness = 0;
	int MaxCorrectIndex = std::max_element(CorrectOutput, &(CorrectOutput[SizeOfOutput - 1])) - CorrectOutput;
	//int MaxModelIndex = std::max_element(ModelOutput, &(ModelOutput[SizeOfOutput - 1])) - ModelOutput;
	
	
	for (int i = 0; i < SizeOfOutput; i++) {
		if (i == MaxCorrectIndex) {
			Fitness += ModelOutput[i];
		}
		else {
			Fitness -= ModelOutput[i];
		}

	}

	//else {
	//	Fitness = -1;
	//}
	//if (BiggestCorrectValue > 100 || BiggestCorrectValue < 0.0) {
	//	Fitness -= 11;
	//}
	//if (BiggestModelValue > 100 || BiggestModelValue < 0.0) {
	//	Fitness -= 11;
	//}
	return Fitness;

	//double Error = 0;
	/*for (int i = 0; i < SizeOfOutput; i++) {
		if (ModelOutput[i] > 100 || ModelOutput[i] < 0.0) {
			Error += 2 * (abs(CorrectOutput[i] - ModelOutput[i]) * abs(CorrectOutput[i] - ModelOutput[i]));
		}
		else {
			Error += abs(CorrectOutput[i] - ModelOutput[i])*abs(CorrectOutput[i] - ModelOutput[i]);
		}

	}*/

	//return 1 / (Error / NumberOfTests);
}
void PrintImageDataToText(double* ImageData, int SideOfSquareImage) {
	char LuminosityChart[] = " `.-\':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
	for (int i = 0; i < SideOfSquareImage; i++) {
		for (int j = 0; j < SideOfSquareImage; j++) {
			printf("%c", LuminosityChart[(unsigned int)ImageData[i * SideOfSquareImage + j] / 3]);
			std::cout << " ";

		}
		std::cout << "\n";
	}
}

void main() {



	int NetworkLayers[] = { 784,100,10};
	//int NetworkLayers[] = { 2,10,1 };
	int NumberOfModelsPerEpoch = 1000;
	int NumberOfEpochs = 1000;
	int NumberOfTestsPerModel = 10;

	double FractionOfParentsToBearChildren = 0.1;
	double MaxMutationChangePercent = 400;


	
	NeuralNetworkPopulation* MyPopulation = new NeuralNetworkPopulation(
		&NetworkLayers[0],
		(int)(sizeof(NetworkLayers) / sizeof(int)),
		NumberOfModelsPerEpoch,
		NumberOfEpochs,
		NumberOfTestsPerModel,
		FractionOfParentsToBearChildren,
		MaxMutationChangePercent
	);

	/*CSV_NeuralNetworkData* CSV_Data = new CSV_NeuralNetworkData(
		&NetworkLayers[0],
		(int)(sizeof(NetworkLayers) / sizeof(int)),
		999999.0,
		3,
		1000000,
		"C:/Users/Nikita/Desktop/MacnineLearning2-main/Data2.csv"
	);*/

	/*NeuralNetworkData* CSV_Data = new NeuralNetworkData(
		&NetworkLayers[0],
		(int)(sizeof(NetworkLayers) / sizeof(int)),
		999999.0,
		3,
		1000000,
		"C:/Users/Nikita/Desktop/MacnineLearning2-main/Data2.csv",
		NeuralNetworkDataType::CSV
	); */

	NeuralNetworkData* IMAGE_Data = new NeuralNetworkData(
		&NetworkLayers[0],
		(int)(sizeof(NetworkLayers) / sizeof(int)),
		999999.0,
		(28*28),
		10021,
		"C:/Users/Nikita/Desktop/MacnineLearning2-main/MNIST_TEST2",
		NeuralNetworkDataType::IMAGE_BLACKANDWHITE
	);
	//IMAGE_Data->double_Print2DArray(IMAGE_Data->NetworkCorrectArray, 1, 10021);
	//IMAGE_Data->double_GeneralPrintInputData();


	MyPopulation->TrainNeuralNetworkPopulation(IMAGE_Data, MyFitnessFunction);
		

	/*MyPopulation->GenePool[NumberOfModelsPerEpoch - 1]->PrintModel();
	while (true) {
		std::cout << "\nEnter Input\n";
		double Test[2];
		std::cin >> Test[0];
		std::cin >> Test[1];
		MyPopulation->GenePool[NumberOfModelsPerEpoch - 1]->RunNetwork(Test);

		std::cout << MyPopulation->GenePool[NumberOfModelsPerEpoch - 1]->TwoDArrayOfAllOutputs[sizeof(NetworkLayers) / sizeof(int) - 1][0];
	}*/

	

	//MyPopulation->GenePool[NumberOfModelsPerEpoch - 1]->PrintModel();
	while (true) {
		double ArrayOfImageInputs[784] = { };
		std::cout << "\nEnter Input Image Path\n";
		std::string ImagePath;
		std::cin >> ImagePath;

		for (const auto& entry : std::filesystem::directory_iterator(ImagePath)) {
			IMAGE_Data->LoadSingleImageToMLArray(ArrayOfImageInputs, entry);
		}
		NeuralNetworkModel* WinnerWinnerChickedDinner = MyPopulation->GenePool[NumberOfModelsPerEpoch - 1];
		WinnerWinnerChickedDinner->RunNetwork(ArrayOfImageInputs);
		std::cout << "\n";
		PrintImageDataToText(ArrayOfImageInputs, 28);
		for (int i = 0; i < WinnerWinnerChickedDinner->NetworkLayersNumberOfNeuronsDefinitions[WinnerWinnerChickedDinner->NumberOfNetworkLayers - 1]; i++) {
			//std::cout << i << ":" << WinnerWinnerChickedDinner->TwoDArrayOfAllOutputs[sizeof(NetworkLayers) / sizeof(int) - 1][i] << " ";
			std::cout << i << " = " << WinnerWinnerChickedDinner->NetworkFinalOutputPoiter[i] << "  ";
		}
		std::cout << "\n";
		//MyPopulation->GenePool[NumberOfModelsPerEpoch - 1]->PrintModel();
		//C:/Users/Nikita/Desktop/MacnineLearning2-main/MLTEST
	}


	free(IMAGE_Data);
	//free(MyPopulation->GenePool);
	std::cout << "\nEND!\n";
	
}


