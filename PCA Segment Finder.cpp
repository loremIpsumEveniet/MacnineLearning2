#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "2DArrayHandling.h"

#define TABLE_X 61;
#define TABLE_Y 252;

#define CURSED_NUMBER 9999999

using std::string;
using std::stringstream;

struct Data {

    std::string CountryName;
    double* EntryData;

    Data(string Name, double* DonorArray, int ArraySize) {

        CountryName = Name;
        EntryData = (double*)malloc(sizeof(double) * ArraySize);
        for (int i = 0; i < ArraySize; i++) {
            EntryData[i] = DonorArray[i];
        }
    }

};

int main() {
    std::cout << "Hello World!\n";
  

    int Table_X = TABLE_X;
    int Table_Y = TABLE_Y;

    std::vector<string> CSVRowNames;

    double** CSVData = double_Make2DArray(Table_X, Table_Y);
    
    double** EUDistances = double_Make2DArray(Table_Y, Table_Y);;
    



    std::ifstream myfile;
    myfile.open("C:/Users/somet/Desktop/PCA Simularity Segment Finder/GDP YTY Segement Simularity Analysis CLEAN.csv");

    for (int i = 0; i < Table_Y; i++) {

        string Line;
        std::getline(myfile, Line);
        stringstream str(Line);
     
        string CurrentWord;
        
        std::getline(str, CurrentWord, ',');
        CSVRowNames.push_back(CurrentWord);

        for (int j = 0; j < Table_X; j++) {
 
            std::getline(str, CurrentWord, ',');
            if (CurrentWord == "") {
                CSVData[i][j] = CURSED_NUMBER;
            }
            else {
                CSVData[i][j] = std::stof(CurrentWord);
            }

        }
    }
   // double_Print2DArray(CSVData, Table_X, Table_Y);

   for (int u = 0; u < Table_Y; u++) {
        
        double* LookAt_Country = CSVData[u];

        for (int i = 0; i < Table_Y; i++) {

            double* LootTo_Country = CSVData[i];

            double currenteudistance = 0;

            for (int j = 0; j < Table_X; j++) {

                if ((LookAt_Country[j] == CURSED_NUMBER) || LootTo_Country[j] == CURSED_NUMBER) {
                    continue;
                }

                currenteudistance += abs(LookAt_Country[j] - LootTo_Country[j]);
                int uuuaua = 0;
               
            }
            EUDistances[u][i] = currenteudistance;
            int aaaa = 0;
        }
        std::cout << "\n";
    }
   double_Print2DArray(EUDistances, Table_Y, Table_Y);

    myfile.close();
}
