#ifndef _FILES_H_
#define _FILES_H_

#include "lib.h"

void readMaze(char* filename, int*** pMaze, int* pRow, int* pCol, Position* start, Position* dest);
void printMatrix(int rows, int cols, int** matrix);

#endif