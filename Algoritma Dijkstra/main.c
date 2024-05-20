#include "files.c"
#include <stdio.h>
#include <time.h>

int main() {
  struct timeval begin, end;  char filename[20];
  printf("Masukkan nama file input: ");
  scanf("%s", filename);

  int** maze;
  int row, col;
  int* start;
  int* dest;

  readMaze(filename, &maze, &row, &col, &start, &dest);
  gettimeofday(&begin, 0);
  dijkstra(maze, row, col, start, dest);
  gettimeofday(&end, 0);
  long sec = end.tv_sec - begin.tv_sec;
  long us = end.tv_usec - begin.tv_usec;
  double elapsed = sec * 1e3 + us * 1e-3;
  printf("Time: %lf ms\n", elapsed);  
  printMatrix(maze, row, col, start, dest);

  return 0;
}

