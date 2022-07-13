#pragma once
#include <iostream>

double** double_Make2DArray(int SizeX, int SizeY) {

	double** TempPointer = (double**)malloc(sizeof(double*) * (SizeY));
	for (int i = 0; i < SizeY; i++) {
		TempPointer[i] = (double*)malloc(sizeof(double) * (SizeX));
	}
	return TempPointer;
}
void double_Print2DArray(double** Array, int SizeX, int SizeY) {

	for (int i = 0; i < SizeY; i++) {
		for (int j = 0; j < SizeX; j++) {
			std::cout <<" " << Array[i][j];
		}
		std::cout << "\n";
	}
}