#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::string;
using std::stringstream;


struct CSV_NeuralNetworkData {

private:
    double** double_Make2DArray(int SizeX, int SizeY) {

        double** TempPointer = (double**)malloc(sizeof(double*) * (SizeY));
        for (int i = 0; i < SizeY; i++) {
            TempPointer[i] = (double*)malloc(sizeof(double) * (SizeX));
        }
        return TempPointer;
    }
public:
    double** NetworkInputArray;
    double** NetworkCorrectArray;

    int INPUT_SIZE_X;
    int CORRECT_SIZE_X;

    int DataSize_Y_ZEROINDEX;

    void double_Print2DArray(double** Array, int SizeX, int SizeY) {

        for (int i = 0; i < SizeY; i++) {
            for (int j = 0; j < SizeX; j++) {
                std::cout << " " << Array[i][j];
            }
            std::cout << "\n";
        }
    }
    void double_GeneralPrintInputData() {
    
        for (int i = 0; i < DataSize_Y_ZEROINDEX; i++) {
            for (int j = 0; j < INPUT_SIZE_X; j++) {
                std::cout << " " << NetworkInputArray[i][j];
            }
            for (int j = 0; j < CORRECT_SIZE_X; j++) {
                std::cout << " " << NetworkCorrectArray[i][j];
            }
            std::cout << "\n";
        }
    }

    CSV_NeuralNetworkData(int* NetworkLayers, int NumberOfNetworkLayers, double EMPTY_DATA_FLAG, int NUMBER_OF_DATA_X, int NUMBER_OF_DATA_Y_ONEINDEX, std::string CSV_FILE_PATH) {
       
        if (NetworkLayers[0] != (NUMBER_OF_DATA_X - NetworkLayers[NumberOfNetworkLayers - 1])) {
            std::cout << "CSV Not Compatible With Neural Network Input/Output";
            return;
        }

        INPUT_SIZE_X = NetworkLayers[0];
        CORRECT_SIZE_X = NetworkLayers[NumberOfNetworkLayers - 1];

        DataSize_Y_ZEROINDEX = NUMBER_OF_DATA_Y_ONEINDEX + 1;

        NetworkInputArray = double_Make2DArray(NetworkLayers[0], DataSize_Y_ZEROINDEX);
        NetworkCorrectArray = double_Make2DArray(NetworkLayers[NumberOfNetworkLayers - 1], DataSize_Y_ZEROINDEX);


        std::ifstream myfile;
        myfile.open(CSV_FILE_PATH);


        for (int i = 0; i < DataSize_Y_ZEROINDEX; i++) {

            string Line;
            std::getline(myfile, Line);
            stringstream str(Line);

            string CurrentWord;

            for (int j = 0; j < NetworkLayers[0]; j++) {

                std::getline(str, CurrentWord, ',');
                if (CurrentWord == "") {
                    NetworkInputArray[i][j] = EMPTY_DATA_FLAG;
                }
                else {
                    NetworkInputArray[i][j] = std::stof(CurrentWord);
                }
            }
            for (int j = 0; j < NetworkLayers[NumberOfNetworkLayers - 1]; j++) {

                std::getline(str, CurrentWord, ',');
                if (CurrentWord == "") {
                    NetworkCorrectArray[i][j] = EMPTY_DATA_FLAG;
                }
                else {
                    NetworkCorrectArray[i][j] = std::stof(CurrentWord);
                }
            }
        }
        myfile.close();
    }
};
