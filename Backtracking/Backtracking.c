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

// Backtracking function to find paths from S to E
void findPaths(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols, int x, int y, int path[][2], int pathLen, int *pathCount, int shortestPath[MAX_SIZE][MAX_SIZE][2], int *shortestPathLength, int longestPath[MAX_SIZE][MAX_SIZE][2], int *longestPathLength) {
    // If the current position is 'E', we've found a path
    if (maze[x][y] == 'E') {
        path[pathLen][0] = x;
        path[pathLen][1] = y;
        (*pathCount)++;

        // Store the path if it's the shortest found so far
        if (pathLen + 1 < *shortestPathLength) {
            *shortestPathLength = pathLen + 1;
            for (int i = 0; i <= pathLen; i++) {
                shortestPath[0][i][0] = path[i][0];
                shortestPath[0][i][1] = path[i][1];
            }
        }

         // Store the path if it's the longest found so far
        if (pathLen + 1 > *longestPathLength) {
            *longestPathLength = pathLen + 1;
            for (int i = 0; i <= pathLen; i++) {
                longestPath[0][i][0] = path[i][0];
                longestPath[0][i][1] = path[i][1];
            }
        }

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
            findPaths(maze, rows, cols, newX, newY, path, pathLen, pathCount, shortestPath, shortestPathLength,longestPath,longestPathLength);
        }
    }

    // Unmark the current position
    maze[x][y] = temp;
}

int main() {
    char maze[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    struct timeval start, end;

    char filename[100];
    printf("Enter the maze file name: ");
    scanf("%s", filename);

    // Replace "maze.txt" with the name of the file you want to read
    readMazeFromFile(filename, maze, &rows, &cols);

    int startRow, startCol;
    findStart(maze, rows, cols, &startRow, &startCol);

    int path[MAX_SIZE * MAX_SIZE][2];
    int pathLen = 0;
    int pathCount = 0;

    // Store the shortest path found
    int shortestPath[MAX_SIZE][MAX_SIZE][2];
    int shortestPathLength = MAX_SIZE * MAX_SIZE;

     // Store the longest path found
    int longestPath[MAX_SIZE][MAX_SIZE][2];
    int longestPathLength = 0;

    gettimeofday(&start, NULL);

    findPaths(maze, rows, cols, startRow, startCol, path, pathLen, &pathCount, shortestPath, &shortestPathLength,longestPath,&longestPathLength);

    char mazeTemp1[MAX_SIZE][MAX_SIZE];
    char mazeTemp2[MAX_SIZE][MAX_SIZE];
    readMazeFromFile(filename, mazeTemp1, &rows, &cols);
    readMazeFromFile(filename, mazeTemp2, &rows, &cols);
    if (pathCount == 0) {
        printf("No paths found from Start to End.\n");

    } else {
        printf("Number of paths found: %d\n", pathCount);
        printf("Shortest path found:\n");
        for (int i = 1; i < shortestPathLength-1; i++) {
            mazeTemp1[shortestPath[0][i][0]][shortestPath[0][i][1]] = '*';
        }
        printMaze(mazeTemp1, rows, cols);

        printf("longest path found:\n");
        for (int i = 1; i < longestPathLength-1; i++) {
            mazeTemp2[longestPath[0][i][0]][longestPath[0][i][1]] = '*';
        }
        printMaze(mazeTemp2, rows, cols);
    }

       gettimeofday(&end, NULL);
        long seconds = end.tv_sec - start.tv_sec;
        long micros = end.tv_usec - start.tv_usec;
        double elapsed = seconds * 1e3 + micros / 1e3;

        printf("\nWaktu yang dibutuhkan : %.6f ms\n", elapsed);
        return 0;

}
