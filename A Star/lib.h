#ifndef _LIB_H_
#define _LIB_H_

typedef struct Position {
  int x, y;
} Position;

typedef struct List {
  // list to hold nodes
  Position* pos;
  struct List* next;
} List;

int isEmpty(List* list);
void push(List** pList, Position* pos);
Position* pop(List** pList);

void aStarSearch(int** maze, int row, int col, Position* start, Position* dest);

#endif