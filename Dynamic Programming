#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define INF 1000000

typedef struct {
    int length;
    int path[MAX_ROWS * MAX_COLS][2];
} Path;

Path dp[MAX_ROWS][MAX_COLS];

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

//Fungsi untuk mencari jalur terpendek
void DP(char **Map, int rows, int cols, int startX, int startY, int endX, int endY) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dp[i][j].length = INF;
        }
    }
    dp[startX][startY].length = 0;
    dp[startX][startY].path[0][0] = startX;
    dp[startX][startY].path[0][1] = startY;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    //Mengisi tabel tiap terjadi pergerakan ke titik baru
    for (int steps = 0; steps < rows * cols; steps++) {
        for (int x = 0; x < rows; x++) {
            for (int y = 0; y < cols; y++) {
                for (int dir = 0; dir < 4; dir++) {
                    int newX = x + dx[dir];
                    int newY = y + dy[dir];
                    //Pemeriksa apakah titik selanjutnya dapat dikunjungi
                    if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && (Map[newX][newY] == '.' || Map[newX][newY] == 'E')) {
                        if (dp[newX][newY].length > dp[x][y].length + 1) {
                            dp[newX][newY].length = dp[x][y].length + 1;
                            for (int i = 0; i < dp[x][y].length; i++) {
                                dp[newX][newY].path[i][0] = dp[x][y].path[i][0];
                                dp[newX][newY].path[i][1] = dp[x][y].path[i][1];
                            }
                            dp[newX][newY].path[dp[x][y].length][0] = newX;
                            dp[newX][newY].path[dp[x][y].length][1] = newY;
                        }
                    }
                }
            }
        }
    }
}

//Fungsi untuk menampilkan semua jalur yang ditemukan
void printAllPaths(char **Map, int endX, int endY) {
    printf("Semua jalur yang ditemukan :\n");
    int length = dp[endX][endY].length;
    for (int i = 0; i < length; i++) {
        printf("Jalur ke %d:\n", i + 1);
        for (int j = 0; j <= i; j++) {
            printf("(%d, %d)\n", dp[endX][endY].path[j][0], dp[endX][endY].path[j][1]);
        }
        printf("\n");
    }
}

//Fungsi untuk menampilkan jalur terpendek
void printShortest(int endX, int endY) {
    printf("Jalur terpendek sepanjang : %d\n", dp[endX][endY].length - 1);
    printf("Jalur terpendek yang ditempuh :\n");
    for (int i = 0; i < dp[endX][endY].length; i++) {
        printf("(%d, %d)\n", dp[endX][endY].path[i][0], dp[endX][endY].path[i][1]);
    }
}

//Fungsi untuk menampilkan jalur terpanjang
void printLongest(char **Map, int rows, int cols, int startX, int startY, int endX, int endY) {
    int longestPathLength = -1;
    int longestPath[MAX_ROWS * MAX_COLS][2];
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (dp[x][y].length != INF) {
                for (int dir = 0; dir < 4; dir++) {
                    int newX = x + dx[dir];
                    int newY = y + dy[dir];
                    if (newX == endX && newY == endY && dp[x][y].length + 1 > longestPathLength) {
                        longestPathLength = dp[x][y].length + 1;
                        for (int i = 0; i < dp[x][y].length; i++) {
                            longestPath[i][0] = dp[x][y].path[i][0];
                            longestPath[i][1] = dp[x][y].path[i][1];
                        }
                        longestPath[dp[x][y].length][0] = endX;
                        longestPath[dp[x][y].length][1] = endY;
                    }
                }
            }
        }
    }

    printf("Jalur terpanjang sepanjang : %d\n", longestPathLength);
    printf("Jalur terpanjang yang ditempuh :\n");
    for (int i = 0; i <= longestPathLength; i++) {
        printf("(%d, %d)\n", longestPath[i][0], longestPath[i][1]);
    }
}

//Fungsi untuk membuka file untuk membaca titik start dan end
void readMatrix(char **Map, int rows, int cols, FILE *file, int *startX, int *startY, int *endX, int *endY) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, " %c", &Map[i][j]);
            if (Map[i][j] == 'S') {
                *startX = i;
                *startY = j;
            } else if (Map[i][j] == 'E') {
                *endX = i;
                *endY = j;
            }
        }
    }
}

//Fungsi untuk mencari dimensi matriks
void getMatrixSize(FILE *file, int *rows, int *cols) {
    char ch;
    *rows = *cols = 0;
    int temp_cols = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            (*rows)++;
            if (temp_cols > *cols) {
                *cols = temp_cols;
            }
            temp_cols = 0;
        } else if (ch != ' ' && ch != '\t') { //Abaikan whitespace
            temp_cols++;
        }
    }
    if (temp_cols > 0) {
        (*rows)++;
        if (temp_cols > *cols) {
            *cols = temp_cols;
        }
    }
    fseek(file, 0, SEEK_SET);
}
//Fungsi untuk memastikan bentuk matriks segi empat
int isSegiempat(FILE *file) {
    char ch;
    int temp_cols = -1;
    int current_cols = 0;
    int current_row = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (temp_cols == -1) {
                temp_cols = current_cols;
            } else if (temp_cols != current_cols) { //Jumlah kolom dan baris berbeda
                return 0;
            }
            current_row++;
            current_cols = 0;
        } else if (ch != ' ' && ch != '\t') {
            current_cols++;
        }
    }

    if (temp_cols == -1) {
        temp_cols = current_cols;
    } else if (temp_cols != current_cols) {
        return 0;
    }
    fseek(file, 0, SEEK_SET);
    return current_row > 0;
}


int main() {
    int rows, cols;
    int startX, startY, endX, endY;

    printf("Masukkan nama file : ");
    char filename[100];
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File tidak dapat dibuka.\n");
        return 0;
    }

    if (!isSegiempat(file)) {
        printf("Bentuk maze tidak segi empat.\n");
        fclose(file);
        return 0;
    }

    getMatrixSize(file, &rows, &cols);

    char **Map = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        Map[i] = (char *)malloc(cols * sizeof(char));
    }

    readMatrix(Map, rows, cols, file, &startX, &startY, &endX, &endY);
    fclose(file);
    struct timeval start, end;
    gettimeofday(&start, NULL);

    DP(Map, rows, cols, startX, startY, endX, endY);

    if (dp[endX][endY].length == INF) {
        printf("Tidak ada jalur yang ditemukan.\n");
    } else {
        printAllPaths(Map, endX, endY);
        printShortest(endX, endY);
        printLongest(Map, rows, cols, startX, startY, endX, endY);
    }

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long micros = end.tv_usec - start.tv_usec;
    double elapsed = seconds + micros / 1e6;

    printf("\nWaktu yang dibutuhkan : %.6f detik\n", elapsed);

    for (int i = 0; i < rows; i++) {
        free(Map[i]);
    }
    free(Map);

    return 0;
}
