#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX_SIZE 100

// Directions for movement: right, down, left, up
int rowDir[] = {0, 1, 0, -1};
int colDir[] = {1, 0, -1, 0};

// Function to read the maze from a file
void readMazeFromFile(char *filename, char maze[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_SIZE];
    int row = 0;
    int col = 0;

    while (fgets(line, sizeof(line), file)) {
        col = 0;
        while (line[col] != '\0' && line[col] != '\n') {
            maze[row][col] = line[col];
            col++;
        }
        row++;
    }

    *rows = row;
    *cols = col;

    fclose(file);
}

// Function to print the maze
void printMaze(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}

// Function to find the position of 'S' (Start)
void findStart(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols, int *startRow, int *startCol) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') {
                *startRow = i;
                *startCol = j;
                return;
            }
        }
    }
}

// Function to print a path on the maze
void printPathOnMaze(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols, int path[][2], int pathLen) {
    char tempMaze[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            tempMaze[i][j] = maze[i][j];
        }
    }

    for (int i = 0; i < pathLen; i++) {
        if (tempMaze[path[i][0]][path[i][1]] == '.') {
            tempMaze[path[i][0]][path[i][1]] = ' ';
        }
    }

    printMaze(tempMaze, rows, cols);
    printf("\n");
}

int validContent(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(maze[i][j]!='.' && maze[i][j]!='#' && maze[i][j]!='S' && maze[i][j]!='E'){
                return 0;
            }
        }
    }
    return 1;
}

// Backtracking function to find paths from S to E
void findPaths(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols, int x, int y, int path[][2], int pathLen, int *pathCount, int allPaths[MAX_SIZE][MAX_SIZE][2], int *allPathsLength) {
    // If the current position is 'E', we've found a path
    if (maze[x][y] == 'E') {
        path[pathLen][0] = x;
        path[pathLen][1] = y;
        (*pathCount)++;

        // Store the path
        for (int i = 0; i <= pathLen; i++) {
            allPaths[*pathCount - 1][i][0] = path[i][0];
            allPaths[*pathCount - 1][i][1] = path[i][1];
        }
        allPathsLength[*pathCount - 1] = pathLen + 1;

        return;
    }

    // Mark the current position as part of the path
    char temp = maze[x][y];
    maze[x][y] = 'x';
    path[pathLen][0] = x;
    path[pathLen][1] = y;
    pathLen++;

    // Try all four directions
    for (int i = 0; i < 4; i++) {
        int newX = x + rowDir[i];
        int newY = y + colDir[i];

        if (newX >= 0 && newX < rows && newY >= 0 && newY < cols &&
            (maze[newX][newY] == '.' || maze[newX][newY] == 'E')) {
            findPaths(maze, rows, cols, newX, newY, path, pathLen, pathCount, allPaths, allPathsLength);
        }
    }

    // Unmark the current position
    maze[x][y] = temp;
}

int main() {
    char maze[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    char filename[100];
    printf("Enter the maze file name: ");
    scanf("%s", filename);
    readMazeFromFile(filename, maze, &rows, &cols);
     if (!validContent(maze, rows, cols)) {
        printf("Invalid Maze\n");
        return 1;
    }

    int startRow, startCol;
    findStart(maze, rows, cols, &startRow, &startCol);

    int path[MAX_SIZE * MAX_SIZE][2];
    int pathLen = 0;
    int pathCount = 0;

    // Store all paths
    int allPaths[MAX_SIZE][MAX_SIZE][2];
    int allPathsLength[MAX_SIZE];

    findPaths(maze, rows, cols, startRow, startCol, path, pathLen, &pathCount, allPaths, allPathsLength);

    if (pathCount == 0) {
        printf("No paths found from Start to End.\n");
    } else {
        printf("Number of paths found: %d\n", pathCount);
        printf("All paths:\n");
        for (int i = 0; i < 4; i++) {
            printf("Path %d:\n", i + 1);
            printPathOnMaze(maze, rows, cols, allPaths[i], allPathsLength[i]);
        }
        printf("Shortest path found:\n");
        int shortestPathIndex = 0;
        int shortestPathLength = allPathsLength[0];
        for (int i = 1; i < pathCount; i++) {
            if (allPathsLength[i] < shortestPathLength) {
                shortestPathIndex = i;
                shortestPathLength = allPathsLength[i];
            }
        }
        int longestPathIndex = 0;
        int longestPathLength = allPathsLength[0];
        for (int i = 1; i < pathCount; i++) {
            if (allPathsLength[i] > longestPathLength) {
                longestPathIndex = i;
                longestPathLength = allPathsLength[i];
            }
        }
        printPathOnMaze(maze, rows, cols, allPaths[shortestPathIndex], allPathsLength[shortestPathIndex]);
        printf("longest path found:\n");
        printPathOnMaze(maze, rows, cols, allPaths[longestPathIndex], allPathsLength[longestPathIndex]);
    }
        gettimeofday(&end, NULL);

        long seconds = end.tv_sec - start.tv_sec;
        long micros = end.tv_usec - start.tv_usec;
        double elapsed = seconds * 1e3 + micros / 1e3;

        printf("\nWaktu yang dibutuhkan : %.6f ms\n", elapsed);
        return 0;
}
