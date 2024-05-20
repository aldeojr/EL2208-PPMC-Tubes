#include "lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void readMaze(char* filename, int*** pMaze, int* pRow, int* pCol, Position* start, Position* dest) {
  FILE* fp = fopen(filename, "r");
  char buf[1024];
  
  if (fp == NULL) {
    printf("File tidak ditemukan.\n");
    exit(0);
  }
  
  if (fgets(buf, 1024, fp) != NULL) {
    buf[strcspn(buf, "\n")] = 0;
    *pRow = 1;
    *pCol = strlen(buf);
    while (fgets(buf, 1024, fp) != NULL) {
      buf[strcspn(buf, "\n")] = 0;
      int col = strlen(buf);
      if (*pCol != col) {
        printf("Bentuk maze bukan persegi atau persegi panjang!\n");
        exit(0);
      }
      (*pRow)++;
    }
  } else {
    printf("File kosong!\n");
    exit(0);
  }

  (*pMaze) = malloc((*pRow) * sizeof(int*));
  (*pMaze)[0] = malloc((*pCol) * sizeof(int));

  rewind(fp);
  int ch, y = 0, x = 0;
  while ((ch = getc(fp)) != EOF) {
    if (ch == '\n') {
      y++;
      (*pMaze)[y] = malloc((*pCol) * sizeof(int));
      x = -1;
    } else if (ch == 'S') {
      start->x = x;
      start->y = y;
      (*pMaze)[y][x] = 1;
    } else if (ch == 'E') {
      dest->x = x;
      dest->y = y;
      (*pMaze)[y][x] = 1;
    } else if (ch == '.') (*pMaze)[y][x] = 1;
    else if (ch == '#') (*pMaze)[y][x] = 0;
    else {
      printf("Terdapat karakter tidak valid!\n");
      exit(0);
    }
    x++;
  }

  fclose(fp);
  return;
}

void printMatrix(int rows, int cols, int** matrix) { // debug -> (char (*(*)[N]))maze / (int(**) [12])maze, 11
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}