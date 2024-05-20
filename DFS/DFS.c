#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h> // Include the sys/time.h library

#define MAX 1000 // Define maximum size for the maze

// DATA STRUCTURES DEFINITIONS
// Structure to represent a pair of indices
typedef struct {
    int row;
    int col;
} Pair;

// Stack data structure to store pairs of indices
typedef struct {
    Pair stack[MAX];
    int top;
} Stack;

// Initialize the stack
void initStack(Stack *s) {
    s->top = -1;
}

// Check if the stack is empty
bool isEmpty(Stack *s) {
    return s->top == -1;
}

// Push an element onto the stack
void push(Stack *s, Pair p) {
    if (s->top < MAX - 1) {
        s->stack[++(s->top)] = p;
    } else {
        printf("Stack overflow\n");
    }
}

// Pop an element from the stack
Pair pop(Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[(s->top)--];
    } else {
        printf("Stack underflow\n");
        Pair invalid = {-1, -1};
        return invalid;
    }
}

// Peek the top element of the stack
Pair top(Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[s->top];
    } else {
        Pair invalid = {-1, -1};
        return invalid;
    }
}

// Maze representation
char maze[MAX][MAX];
bool isVisited[MAX][MAX];

// Directions for moving up, down, left, right
int dirRow[] = {-1, 1, 0, 0};
int dirCol[] = {0, 0, -1, 1};

// Function prototypes
void findAllPaths(Pair current, Pair end, int rows, int cols, Stack *path_stack, Stack *shortest_stack, Stack *longest_stack, int *path_count, int *shortest, int *longest, int path_length);
Pair find_start_index(char maze[MAX][MAX], int rows, int cols);
Pair find_end_index(char maze[MAX][MAX], int rows, int cols);
bool readMazeFromFile(const char *filename, char maze[MAX][MAX], int *rows, int *cols);
void printStack(Stack *s);

// HELPER FUNCTIONS
// Find the starting index in the maze
Pair find_start_index(char maze[MAX][MAX], int rows, int cols) {
    Pair start;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') {
                start.row = i;
                start.col = j;
                return start;
            }
        }
    }
    start.row = -1;
    start.col = -1;
    return start; // Return an invalid index if not found
}

// Find the end index in the maze
Pair find_end_index(char maze[MAX][MAX], int rows, int cols) {
    Pair end;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'E') {
                end.row = i;
                end.col = j;
                return end;
            }
        }
    }
    end.row = -1;
    end.col = -1;
    return end; // Return an invalid index if not found
}

// Function to check if the current cell is valid
bool isValidMove(Pair p, int rows, int cols) {
    return p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols &&
           maze[p.row][p.col] != '#' && !isVisited[p.row][p.col];
}

// Copy one stack to another
void copyStack(Stack *src, Stack *dest) {
    dest->top = src->top;
    for (int i = 0; i <= src->top; i++) {
        dest->stack[i] = src->stack[i];
    }
}

// RECURSIVE FUNCTION TO FIND ALL PATHS
void findAllPaths(Pair current, Pair end, int rows, int cols, Stack *path_stack, Stack *shortest_stack, Stack *longest_stack, int *path_count, int *shortest, int *longest, int path_length) {
    if (current.row == end.row && current.col == end.col) {
        // Path found
        (*path_count)++;
        if (path_length < *shortest) {
            *shortest = path_length;
            copyStack(path_stack, shortest_stack);
        }
        if (path_length > *longest) {
            *longest = path_length;
            copyStack(path_stack, longest_stack);
        }
        return;
    }

    // Mark the current cell as visited
    isVisited[current.row][current.col] = true;
    push(path_stack, current);

    for (int i = 0; i < 4; i++) {
        Pair next = {current.row + dirRow[i], current.col + dirCol[i]};
        if (isValidMove(next, rows, cols)) {
            findAllPaths(next, end, rows, cols, path_stack, shortest_stack, longest_stack, path_count, shortest, longest, path_length + 1);
        }
    }

    pop(path_stack); // Backtrack
    isVisited[current.row][current.col] = false; // Unmark the current cell
}

// FUNCTION TO READ MAZE FROM FILE
bool readMazeFromFile(const char *filename, char maze[MAX][MAX], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return false;
    }

    *rows = 0;
    *cols = 0;
    char line[MAX];

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') line[--len] = '\0'; // Remove newline character
        if (*cols == 0) *cols = len;
        for (int j = 0; j < len; j++) {
            maze[*rows][j] = line[j];
        }
        (*rows)++;
    }

    fclose(file);
    return true;
}

// FUNCTION TO PRINT THE STACK
void printStack(Stack *s) {
    for (int i = 0; i <= s->top; i++) {
        printf("(%d, %d)", s->stack[i].row, s->stack[i].col);
        if (i < s->top) printf(" -> ");
    }
    printf("\n");
}

// MAIN FUNCTION
int main() {
    int rows, cols;
    char filename[100];

    // Prompt the user for the filename
    printf("Enter the filename of the maze: ");
    scanf("%s", filename);

    // Read the maze from the provided file
    if (!readMazeFromFile(filename, maze, &rows, &cols)) {
        return 1;
    }

    Pair start = find_start_index(maze, rows, cols);
    Pair end = find_end_index(maze, rows, cols);

    if (start.row == -1 || start.col == -1) {
        printf("Start point 'S' not found in the maze.\n");
        return 1;
    }

    if (end.row == -1 || end.col == -1) {
        printf("End point 'E' not found in the maze.\n");
        return 1;
    }

    // Initialize the visited array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            isVisited[i][j] = false;
        }
    }

    Stack path_stack, shortest_stack, longest_stack;
    initStack(&path_stack);
    initStack(&shortest_stack);
    initStack(&longest_stack);

    int path_count = 0;
    int shortest = MAX;
    int longest = 0;

    struct timeval start_time, end_time; // Declare timeval structs

    gettimeofday(&start_time, NULL); // Start time

    findAllPaths(start, end, rows, cols, &path_stack, &shortest_stack, &longest_stack, &path_count, &shortest, &longest, 0);

    gettimeofday(&end_time, NULL); // End time

    // Calculate time in milliseconds
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    double elapsed = seconds * 1000.0 + microseconds / 1000.0;

    printf("Number of paths: %d\n", path_count);
    printf("Length of shortest path: %d\n", shortest);
    printf("Shortest path from start to end:\nPath: ");
    printStack(&shortest_stack);
    printf("Length of longest path: %d\n", longest);
    printf("Longest path from start to end:\nPath: ");
    printStack(&longest_stack);
    printf("Time taken to solve the maze: %f milliseconds\n", elapsed); // Print the time taken in milliseconds

    return 0;
}
