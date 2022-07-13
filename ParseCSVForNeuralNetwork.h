#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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
    void double_Print2DArray(double** Array, int SizeX, int SizeY) {

        for (int i = 0; i < SizeY; i++) {
            for (int j = 0; j < SizeX; j++) {
                std::cout << " " << Array[i][j];
            }
            std::cout << "\n";
        }
    }

    double** NetworkInputArray;
    double** NetworkCorrectArray;

    CSV_NeuralNetworkData(int* NetworkLayers, int NumberOfNetworkLayers, double EMPTY_DATA_FLAG, int NUMBER_OF_DATA_X, int NUMBER_OF_DATA_Y, std::string CSV_FILE_PATH) {
       
        if (NetworkLayers[0] != (NUMBER_OF_DATA_X - NetworkLayers[NumberOfNetworkLayers - 1])) {
            std::cout << "CSV Not Compatible With Neural Network Input/Output";
            return;
        }

        NetworkInputArray = double_Make2DArray(NetworkLayers[0], NUMBER_OF_DATA_Y);
        NetworkCorrectArray = double_Make2DArray(NetworkLayers[NumberOfNetworkLayers - 1], NUMBER_OF_DATA_Y);


        std::ifstream myfile;
        myfile.open(CSV_FILE_PATH);


        for (int i = 0; i < NUMBER_OF_DATA_Y; i++) {

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
