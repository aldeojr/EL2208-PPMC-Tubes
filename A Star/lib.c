#include "lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// A* Search Algorithm
// HASHNODE
typedef struct HashNode {
  // node's position is key to hash table
  Position* parent;
  int local, global; // heuristics
  int expanded; // expanded node check
} HashNode;

// LIST
void printList(List* list) {
  List* temp = list;
  while (temp->next != NULL) {
    printf("(%d, %d) -> ", temp->pos->x, temp->pos->y);
    temp = temp->next;
  }
  printf("(%d, %d)", temp->pos->x, temp->pos->y);
}

int isEmpty(List* list) {
  if (list == NULL) return 1;
  else return 0;
}

void push(List** pList, Position* pos) {
  List* newEl = malloc(sizeof(List));
  newEl->pos = pos;
  newEl->next = *pList;
  *pList = newEl;
}

Position* pop(List** pList) {
  if (isEmpty(*pList)) {
    printf("List kosong!\n");
    return NULL;
  }

  List* temp = *pList;
  Position* pos = temp->pos;
  *pList = (*pList)->next;
  free(temp);
  return pos;
}

void swap(List* a, List* b) {
  Position* temp = a->pos;
  a->pos = b->pos;
  b->pos = temp;
}

void sortList(List* list, HashNode*** hashTable) {
  int swapped;
  List* p1;
  List* lp = NULL; // last node pointer (highest priority)

  if (list == NULL) return;

  do {
    swapped = 0;
    p1 = list;

    while (p1->next != lp) { // loop until first sorted element
      if (hashTable[p1->pos->y][p1->pos->x]->global
       > hashTable[p1->next->pos->y][p1->next->pos->x]->global) {
        swap(p1, p1->next);
        swapped = 1;
      }
      p1 = p1->next;
    }
    lp = p1;
  } while (swapped);
}

// ALGO
int heuristic(Position* pos1, Position* pos2) {
  // Manhattan Distance heuristic
  return (abs(pos1->x - pos2->x) + abs(pos1->y - pos2->y));
}

int isValid(Position* pos, int row, int col) {
  return (pos->x >= 0) && (pos->x < col) && (pos->y >= 0) && (pos->y < row);
}

void printNodes(HashNode*** nodes, int row, int col) {
  for (int y = 0; y < row; y++) {
    for (int x = 0; x < row; x++) {
      printf("<%2d,%2d,(%2d,%2d)> ", nodes[y][x]->global, nodes[y][x]->local, nodes[y][x]->parent->x, nodes[y][x]->parent->y);
      // printf("<%2d,%2d,(%2d,%2d)> ", x, y, nodes[y][x]->parent->x, nodes[y][x]->parent->y);
    }
    printf("\n");
  }
}

void drawPath(int** maze, int row, int col, List* path, Position* start, Position* dest) {
  char map[row][col];
  for (int y = 0; y < row; y++) {
    for (int x = 0; x < col; x++) {
      if (maze[y][x]) map[y][x] = '.';
      else map[y][x] = '#';
    }
  }

  List* temp = path;
  int step = -1; // account for starting node
  while (temp != NULL) {
    step++;
    map[temp->pos->y][temp->pos->x] = ' ';
    temp = temp->next;
  }

  map[start->y][start->x] = 'S';
  map[dest->y][dest->x] = 'E';

  printf("Jumlah langkah: %d\n", step);
  for (int y = 0; y < row; y++) {
    for (int x = 0; x < col; x++) {
      printf("%c ", map[y][x]);
    }
    printf("\n");
  }
}

void aStarSearch(int** maze, int row, int col, Position* start, Position* dest) {
  // row -> y, col -> x
  HashNode*** nodes = malloc(sizeof(HashNode*) * row);
  // initialize nodes
  for (int y = 0; y < row; y++) {
    nodes[y] = malloc(sizeof(HashNode) * col);
    for (int x = 0; x < col; x++) {
      nodes[y][x] = malloc(sizeof(HashNode));
      nodes[y][x]->expanded = 0;  
      nodes[y][x]->local = nodes[y][x]->global = row * col;
      Position* parent = malloc(sizeof(Position));
      parent->x = parent->y = -1;
      nodes[y][x]->parent = parent;
    }
  }

  // setup starting node
  nodes[start->y][start->x]->local = 0;
  nodes[start->y][start->x]->global = heuristic(start, dest);
  nodes[start->y][start->x]->parent = start;

  List* frontier = NULL; // nodes to expand
  push(&frontier, start);
  Position* curr = start;
  while (!isEmpty(frontier)) { // loop until no more node to expand
    sortList(frontier, nodes);

    while (!isEmpty(frontier) && nodes[frontier->pos->y][frontier->pos->x]->expanded) {
      // dump already expanded nodes
      pop(&frontier);
    }

    if (isEmpty(frontier)) break; // safety check if dumped repeatedly until NULL is left

    curr = frontier->pos;
    nodes[curr->y][curr->x]->expanded = 1;
    int dy[4] = {-1, 0, 1, 0};
    int dx[4] = {0, -1, 0, 1};
    
    // check neighbors
    for (int i = 0; i < 4; i++) {
      Position* neighbor = malloc(sizeof(Position));
      neighbor->y = curr->y + dy[i];
      neighbor->x = curr->x + dx[i];

      // boundary check
      if (isValid(neighbor, row, col)) {
        if (!nodes[neighbor->y][neighbor->x]->expanded && maze[neighbor->y][neighbor->x])
          push(&frontier, neighbor);

        int local = nodes[curr->y][curr->x]->local + 1;
        if (local < nodes[neighbor->y][neighbor->x]->local) {
          nodes[neighbor->y][neighbor->x]->parent = curr;
          nodes[neighbor->y][neighbor->x]->local = local;
          nodes[neighbor->y][neighbor->x]->global = local + heuristic(neighbor, dest);
        }
      }
    }
  }

  // reconstruct path and draw it if there is one
  if (!(nodes[dest->y][dest->x]->expanded)) {
    printf("\nJalan tidak ditemukan!\n");
  } else {
    List* path = NULL;
    push(&path, dest);
    int x = dest->x, y = dest->y;
    while (!(nodes[y][x]->parent->x == x && nodes[y][x]->parent->y == y)) {
      push(&path, nodes[y][x]->parent);
      int tx = nodes[y][x]->parent->x;
      int ty = nodes[y][x]->parent->y;
      x = tx;
      y = ty;
    }
    printf("Jalan optimal:\n");
    // printList(path);
    drawPath(maze, row, col, path, start, dest);
  }
}

// BACKTRACKING -- ADAPTED
void printSolution(int** maze, int** sol, int row, int col, int len) {
  char map[row][col];
  printf("Jumlah langkah: %d\n", len);
  for (int y = 0; y < row; y++) {
      for (int x = 0; x < col; x++) {
          if (sol[y][x]) map[y][x] = ' ';
          else if (maze[y][x]) map[y][x] = '.';
          else map[y][x] = '#';
      }
  }

  for (int y = 0; y < row; y++) {
    for (int x = 0; x < col; x++) {
      printf("%c ", map[y][x]);
    }
    printf("\n");
  }
  printf("\n");
}

void findAllPathsInner(int** maze, int row, int col, Position* curr, Position* dest, int** sol, int* pNum, int len, int* maxLen, int** longest) {
  if (curr->x == dest->x && curr->y == dest->y) {
    sol[curr->y][curr->x] = 1;
    (*pNum)++;
    if ((*pNum) < 5) {
      printf("Jalan ke-%d:\n", (*pNum));
      printSolution(maze, sol, row, col, len);
    }
    if (len > *maxLen) {
      *maxLen = len;
      for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
          longest[y][x] = sol[y][x];
        }
      }
    }
    sol[curr->y][curr->x] = 0;
    return;
  }

  if (isValid(curr, row, col) && maze[curr->y][curr->x] == 1 && sol[curr->y][curr->x] == 0) {
    sol[curr->y][curr->x] = 1;
    int dy[4] = {-1, 0, 1, 0};
    int dx[4] = {0, -1, 0, 1};
    for (int i = 0; i < 4; i++) {
      Position* neighbor = malloc(sizeof(Position));
      neighbor->y = curr->y + dy[i];
      neighbor->x = curr->x + dx[i];
      findAllPathsInner(maze, row, col, neighbor, dest, sol, pNum, len + 1, maxLen, longest);
    }
    sol[curr->y][curr->x] = 0;
  }
}

void findAllPaths(int** maze, int row, int col, Position* start, Position* dest) {
  int** sol = malloc(sizeof(int*) * row);
  int** longest = malloc(sizeof(int*) * row);
  for (int y = 0; y < row; y++) {
    sol[y] = malloc(sizeof(int) * col);
    longest[y] = malloc(sizeof(int) * col);
    for (int x = 0; x < col; x++) {
      sol[y][x] = 0;
      longest[y][x] = 0;
    }
  }

  Position* curr = malloc(sizeof(Position));
  curr->x = start->x;
  curr->y = start->y;
  int num = 0, maxLen = 0;
  findAllPathsInner(maze, row, col, curr, dest, sol, &num, 0, &maxLen, longest);
  printf("Jumlah jalan: %d\n", num);

  printf("\nJalan terpanjang:\n");
  printSolution(maze, longest, row, col, maxLen);
}