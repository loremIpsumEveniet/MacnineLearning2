#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::string;
using std::stringstream;

enum NeuralNetworkDataType {
    CSV = 1,
    IMAGE_COLOR = 2,
    IMAGE_BLACKANDWHITE = 3
};

struct NeuralNetworkData {

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

    int DataSize_Y;

    void double_Print2DArray(double** Array, int SizeX, int SizeY) {

        for (int i = 0; i < SizeY; i++) {
            for (int j = 0; j < SizeX; j++) {
                std::cout << " " << Array[i][j];
            }
            std::cout << "\n";
        }
    }
    void double_GeneralPrintInputData() {

        for (int i = 0; i < DataSize_Y; i++) {
            for (int j = 0; j < INPUT_SIZE_X; j++) {
                std::cout << " " << NetworkInputArray[i][j];
            }
            std::cout << "\n";
            for (int j = 0; j < CORRECT_SIZE_X; j++) {
                std::cout << " " << NetworkCorrectArray[i][j];
            }
            std::cout << "\n";
        }
    }

    NeuralNetworkData(int* NetworkLayers, int NumberOfNetworkLayers, double EMPTY_DATA_FLAG, int NUMBER_OF_DATA_X, int NUMBER_OF_DATA_Y_ONEINDEX, std::string PATH, enum NeuralNetworkDataType DATATYPE) {

        switch (DATATYPE) {
        case 1: //CSV
            CSV(NetworkLayers, NumberOfNetworkLayers, EMPTY_DATA_FLAG, NUMBER_OF_DATA_X, NUMBER_OF_DATA_Y_ONEINDEX, PATH);
            break;
        case 2: //IMAGE_COLOR
            //TODO
            break;
        case 3: //IMAGE_BLACKANDWHITE
            IMAGE_BLACKANDWHITE(NetworkLayers, NumberOfNetworkLayers, EMPTY_DATA_FLAG, NUMBER_OF_DATA_X, NUMBER_OF_DATA_Y_ONEINDEX, PATH);
            break;
        }
    }
private:
    void CSV(int* NetworkLayers, int NumberOfNetworkLayers, double EMPTY_DATA_FLAG, int NUMBER_OF_DATA_X, int NUMBER_OF_DATA_Y_ONEINDEX, std::string PATH) {

        if (NetworkLayers[0] != (NUMBER_OF_DATA_X - NetworkLayers[NumberOfNetworkLayers - 1])) {
            std::cout << "\nCSV Not Compatible With Neural Network Input/Output\n";
            return;
        }

        INPUT_SIZE_X = NetworkLayers[0];
        CORRECT_SIZE_X = NetworkLayers[NumberOfNetworkLayers - 1];

        DataSize_Y = NUMBER_OF_DATA_Y_ONEINDEX - 1;

        NetworkInputArray = double_Make2DArray(NetworkLayers[0], NUMBER_OF_DATA_Y_ONEINDEX);
        NetworkCorrectArray = double_Make2DArray(NetworkLayers[NumberOfNetworkLayers - 1], NUMBER_OF_DATA_Y_ONEINDEX);

        std::cout << "\nFILE LOADING BEGIN\n";
        std::ifstream myfile;
        myfile.open(PATH);

        for (int i = 0; i < DataSize_Y; i++) {

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
        std::cout << "\nFILE LOADING COMPLETE\n";
        myfile.close();
    }

    void IMAGE_BLACKANDWHITE(int* NetworkLayers, int NumberOfNetworkLayers, double EMPTY_DATA_FLAG, int NUMBER_OF_DATA_X, int NUMBER_OF_DATA_Y_ONEINDEX, std::string PATH) {

        if (NetworkLayers[0] != NUMBER_OF_DATA_X) {
            std::cout << "\nData Not Compatible With Neural Network Input/Output\n";
            return;
        }

        INPUT_SIZE_X = NetworkLayers[0];
        CORRECT_SIZE_X = NetworkLayers[NumberOfNetworkLayers - 1];

        DataSize_Y = NUMBER_OF_DATA_Y_ONEINDEX;

        NetworkInputArray = double_Make2DArray(NetworkLayers[0], NUMBER_OF_DATA_Y_ONEINDEX);
        NetworkCorrectArray = double_Make2DArray(NetworkLayers[NumberOfNetworkLayers - 1], NUMBER_OF_DATA_Y_ONEINDEX);

        std::cout << "\nIMAGES LOADING BEGIN\n";
        int i = 0;
        for (const auto& entry : std::filesystem::directory_iterator(PATH)) {
            if (i > DataSize_Y - 1) {
                break;
            }
            LoadSingleImageToMLArray(NetworkInputArray[i], entry);

            std::string CurrentCorrect = entry.path().filename().string();
            CurrentCorrect = CurrentCorrect.substr(0, CurrentCorrect.find(" "));
            int CurrectInt = std::stod(CurrentCorrect);

            for (int j = 0; j < NetworkLayers[NumberOfNetworkLayers - 1]; j++) {
                //NetworkCorrectArray[i][j] = std::stod(CurrentCorrect);
                NetworkCorrectArray[i][j] = 0.0;
            }
            NetworkCorrectArray[i][CurrectInt] = 100.0;
            i++;
        }
        std::cout << "\nIMAGES LOADING COMPLETE\n";
    }
    public:
        void LoadSingleImageToMLArray(double* MLArray, std::filesystem::directory_entry systemEntry) {

            int width, height, numComponents;

            std::string CurrentFilePath = systemEntry.path().string();
            unsigned char* data = stbi_load(CurrentFilePath.c_str(), &width, &height, &numComponents, 3);

            for (int j = 0; j + 3 <= width * height * 3; j += 3) {
                MLArray[j / 3] = (double)(data[j]);
            }
            delete(data);
        }
};
