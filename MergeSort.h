#pragma once

#include "NeuralNetworkModel.h"

void swapping(NeuralNetworkModel* a, NeuralNetworkModel* b) {     //swap the content of a and b
    NeuralNetworkModel* temp;
    temp = a;
    a = b;
    b = temp;
}
void merge(NeuralNetworkModel** array, int l, int m, int r) {
    int i, j, k, nl, nr;
    //size of left and right sub-arrays
    nl = m - l + 1; nr = r - m;
    NeuralNetworkModel** larr = (NeuralNetworkModel**)malloc(nl * sizeof(NeuralNetworkModel**));//[nl]
    NeuralNetworkModel** rarr = (NeuralNetworkModel**)malloc(nr * sizeof(NeuralNetworkModel**));//[nr];


    //fill left and right sub-arrays
    for (i = 0; i < nl; i++)
        larr[i] = array[l + i];
    for (j = 0; j < nr; j++)
        rarr[j] = array[m + 1 + j];
    i = 0; j = 0; k = l;
    //marge temp arrays to real array
    while (i < nl && j < nr) {
        if (larr[i]->ModelFitness <= rarr[j]->ModelFitness) {
            array[k] = larr[i];
            i++;
        }
        else {
            array[k] = rarr[j];
            j++;
        }
        k++;
    }
    while (i < nl) {       //extra element in left array
        array[k] = larr[i];
        i++; k++;
    }
    while (j < nr) {     //extra element in right array
        array[k] = rarr[j];
        j++; k++;
    }
    free(rarr);
    free(larr);
}
void mergeSort(NeuralNetworkModel** array, int left, int right) {
    int m;
    if (left < right) {
        int m = left + (right - left) / 2;
        // Sort first and second arrays
        mergeSort(array, left, m);
        mergeSort(array, m + 1, right);
        merge(array, left, m, right);
    }
}