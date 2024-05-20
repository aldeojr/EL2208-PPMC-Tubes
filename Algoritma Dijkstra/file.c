#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void readMaze(char* filename, int*** pMaze, int* pRows, int* pCols, int** pStart, int** pDest) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File tidak ditemukan.\n");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int rowCount = 0;
    int colCount = 0;

    // First pass to determine row and column count
    if (fgets(line, sizeof(line), file)) {
        colCount = strcspn(line, "\n");
        rowCount++;
        while (fgets(line, sizeof(line), file)) {
            if (strcspn(line, "\n") != colCount) {
                printf("Bentuk maze bukan persegi atau persegi panjang!\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            rowCount++;
        }
    } else {
        printf("File kosong!\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for maze, start, and destination positions
    *pRows = rowCount;
    *pCols = colCount;
    *pMaze = malloc(rowCount * sizeof(int*));
    for (int i = 0; i < rowCount; i++) {
        (*pMaze)[i] = malloc(colCount * sizeof(int));
    }

    *pStart = malloc(2 * sizeof(int));
    *pDest = malloc(2 * sizeof(int));

    rewind(file);

    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int j = 0; j < colCount; j++) {
            char ch = line[j];
            if (ch == 'S') {
                (*pStart)[0] = i;
                (*pStart)[1] = j;
                (*pMaze)[i][j] = 1;
            } else if (ch == 'E') {
                (*pDest)[0] = i;
                (*pDest)[1] = j;
                (*pMaze)[i][j] = 1;
            } else if (ch == '.') {
                (*pMaze)[i][j] = 1;
            } else if (ch == '#') {
                (*pMaze)[i][j] = 0;
            } else {
                fprintf(stderr, "Terdapat karakter tidak valid: %c\n", ch);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
        i++;
    }

    fclose(file);
}

void printMatrix(int** maze, int row, int col, int* start, int* dest) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (i == start[0] && j == start[1]) {
        printf("S");
      } else if (i == dest[0] && j == dest[1]) {
        printf("E");
      } else if (maze[i][j] == 1) {
        printf(".");
      } else if (maze[i][j] == 0) {
        printf("#");
      }
    }
    printf("\n");
  }
}

typedef struct Node {
    int x, y;
    int dist;
} Node;

typedef struct MinHeap {
    Node* nodes;
    int size;
    int capacity;
} MinHeap;

typedef struct PathNode {
    int x, y;
    struct PathNode* next;
} PathNode;

typedef struct {
    PathNode** data;
    int rows, cols;
} PrevList;

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->nodes = (Node*)malloc(capacity * sizeof(Node));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

void swapNodes(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->nodes[left].dist < minHeap->nodes[smallest].dist)
        smallest = left;

    if (right < minHeap->size && minHeap->nodes[right].dist < minHeap->nodes[smallest].dist)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->nodes[smallest], &minHeap->nodes[idx]);
        minHeapify(minHeap, smallest);
    }
}

Node extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0)
        return (Node){-1, -1, INT_MAX}; 
    Node root = minHeap->nodes[0];
    if (minHeap->size > 1) {
        minHeap->nodes[0] = minHeap->nodes[minHeap->size - 1];
        minHeapify(minHeap, 0);
    }
    minHeap->size--;
    return root;
}

void decreaseKey(MinHeap* minHeap, int x, int y, int dist) {
    for (int i = 0; i < minHeap->size; i++) {
        if (minHeap->nodes[i].x == x && minHeap->nodes[i].y == y) {
            minHeap->nodes[i].dist = dist;
            while (i && minHeap->nodes[i].dist < minHeap->nodes[(i - 1) / 2].dist) {
                swapNodes(&minHeap->nodes[i], &minHeap->nodes[(i - 1) / 2]);
                i = (i - 1) / 2;    
            }
            break;
        }
    }
}

int isInMinHeap(MinHeap* minHeap, int x, int y) {
    for (int i = 0; i < minHeap->size; i++) {
        if (minHeap->nodes[i].x == x && minHeap->nodes[i].y == y)
            return 1;
    }
    return 0;
}

PathNode* createPathNode(int x, int y) {
    PathNode* newNode = (PathNode*)malloc(sizeof(PathNode));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

PrevList* createPrevList(int rows, int cols) {
    PrevList* list = (PrevList*)malloc(sizeof(PrevList));
    list->data = (PathNode**)malloc(rows * cols * sizeof(PathNode*));
    list->rows = rows;
    list->cols = cols;
    for (int i = 0; i < rows * cols; i++) {
        list->data[i] = NULL;
    }
    return list;
}

void addPredecessor(PrevList* list, int x, int y, int px, int py) {
    int index = x * list->cols + y;
    PathNode* newNode = createPathNode(px, py);
    newNode->next = list->data[index];
    list->data[index] = newNode;
}

void printPaths(PathNode* path, int* counter) {
    (*counter)++;
    PathNode* current = path;
    printf("Path %d: ", *counter);
    while (current->next != NULL) {
        printf("(%d, %d) -> ", current->x, current->y);
        current = current->next;    
    }
    printf("(%d, %d) \n", current->x, current->y);
}

void findAllPaths(PrevList* prev, int rows, int cols, int destX, int destY, PathNode* path, int* counter) {
    int index = destX * cols + destY;
    if (prev->data[index] == NULL) {
        PathNode* current = createPathNode(destX, destY);
        current->next = path;
        printPaths(current, counter);
        free(current);
        
        return;
    }

    PathNode* node = prev->data[index];
    while (node != NULL) {
        PathNode* newPath = createPathNode(destX, destY);
        newPath->next = path;
        findAllPaths(prev, rows, cols, node->x, node->y, newPath, counter);
        node = node->next;
        free(newPath);
    }
}

void dijkstra(int** maze, int rows, int cols, int* start, int* dest) {
    int** dist = (int**)malloc(rows * sizeof(int*));
    PrevList* prev = createPrevList(rows, cols);
    for (int i = 0; i < rows; i++) {
        dist[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            dist[i][j] = INT_MAX;
        }
    }

    MinHeap* minHeap = createMinHeap(rows * cols);
    minHeap->nodes[minHeap->size++] = (Node){start[0], start[1], 0};
    dist[start[0]][start[1]] = 0;

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    while (minHeap->size > 0) {
        Node current = extractMin(minHeap);
        int x = current.x;
        int y = current.y;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] == 1) {
                int alt = dist[x][y] + 1;
                if (alt < dist[nx][ny]) {
                    dist[nx][ny] = alt;
                    prev->data[nx * cols + ny] = NULL; // Clear previous predecessors
                    addPredecessor(prev, nx, ny, x, y);
                    if (isInMinHeap(minHeap, nx, ny)) {
                        decreaseKey(minHeap, nx, ny, alt);
                    } else {
                        minHeap->nodes[minHeap->size++] = (Node){nx, ny, alt};
                    }
                } else if (alt == dist[nx][ny]) {
                    addPredecessor(prev, nx, ny, x, y);
                }
            }
        }
    }

    int destX = dest[0];
    int destY = dest[1];

    if (dist[destX][destY] == INT_MAX) {
        printf("Tidak ada jalur dari start ke end.\n");
    } else {
        int counter = 0; 
        printf("All shortest path from start to end:\n");
        findAllPaths(prev, rows, cols, destX, destY, NULL, &counter);
        printf("\nJumlah jalur terpendek tersedia : %d\n", counter);
    }

    for (int i = 0; i < rows; i++) {
        free(dist[i]);
    }
    free(dist);
    for (int i = 0; i < rows * cols; i++) {
        PathNode* node = prev->data[i];
        while (node != NULL) {
            PathNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(prev->data);
    free(prev);
    free(minHeap->nodes);
    free(minHeap);
}

// referensi : https://github.com/Vishnuparammal/maze_runner/blob/master/src/maze.c //
