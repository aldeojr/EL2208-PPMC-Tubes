#include "files.h"
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
  char filename[20];
  printf("Masukkan nama file input: ");
  scanf("%s", filename);

  int** maze;
  int row, col;
  Position* start = malloc(sizeof(Position));
  Position* dest = malloc(sizeof(Position));

  readMaze(filename, &maze, &row, &col, start, dest);
  
  struct timeval begin, end;
  gettimeofday(&begin, 0);
  aStarSearch(maze, row, col, start, dest);
  gettimeofday(&end, 0);

  long sec = end.tv_sec - begin.tv_sec;
  long us = end.tv_usec - begin.tv_usec;
  double elapsed = sec * 1e3 + us * 1e-3;
  printf("Time: %lf ms\n", elapsed);

  return 0;
}