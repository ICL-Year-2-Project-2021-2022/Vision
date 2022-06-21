#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int compareMatrices(int rows, int columns, int **matrixA, int **matrixB) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (matrixA[i][j] != matrixB[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int **generateArrayWithOnesAtIndexes(int rowsCount, int colsCount, int length, int **onesIndices) {
    int **grid = (int **) malloc(sizeof(int *) * rowsCount);
    for (int i = 0; i < rowsCount; i++) {
        grid[i] = (int *) malloc(sizeof(int *) * colsCount);
    }
    for (int i = 0; i < rowsCount; i++) {
        for (int j = 0; j < colsCount; j++) {
            grid[i][j] = 0;
        }
    }
    for (int i = 0; i < length; i++) {
        int row = onesIndices[i][0];
        int col = onesIndices[i][1];
        grid[row][col] = 1;
    }
    return grid;
}

int **getGrid(size_t state_size, float **state, float **var, int cellWidth, int cellHeight, int rowCount,
              int columnCount) {
    int **grid = (int **) malloc(sizeof(int *) * rowCount);
    for (int i = 0; i < rowCount; i++) {
        grid[i] = (int *) malloc(sizeof(int *) * columnCount);
    }

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < columnCount; j++) {
            grid[i][j] = 0;
        }
    }

    for (int i = 0; i < (state_size - 3) / 2; i++) {
        float radius_x, radius_y;
        int numSquaresOccupiedRadius_x, numSquaresOccupiedRadius_y;
        int offset_x;
        int offset_y;


        radius_x = sqrt(var[2 * i + 3][2 * i + 3]);
        numSquaresOccupiedRadius_x = 1.5 * (int) ceil(radius_x / cellWidth);
        offset_x = state[2 * i + 3][0] / cellWidth;
        offset_y = state[2 * i + 4][0] / cellHeight;

        radius_y = ceil(sqrt(var[2 * i + 4][2 * i + 4]));
        numSquaresOccupiedRadius_y = 1.5 * (int) ceil(radius_y / cellHeight);

        for (int i = -numSquaresOccupiedRadius_y; i < numSquaresOccupiedRadius_y; i++) {
            for (int j = -numSquaresOccupiedRadius_x; j < numSquaresOccupiedRadius_x; j++) {
                if (i + offset_y < 0 || j + offset_x < 0) {
                    continue;
                }
                if (i + offset_y > rowCount || j + offset_x > columnCount) {
                    continue;
                }
                grid[i + offset_y][j + offset_x] = 1;
            }
        }
    }

    return grid;
}

void freeMatrix(size_t row, size_t col, int **grid) {
    for (int i = 0; i < row; i++) {
        free(grid[i]);
    }
    free(grid);
}

/*int basicTestCaseFromMarco() {
    int state_size = 7;
    float state[7][1] = {{492.619354},
                         {601.678772},
                         {0.909130},
                         {738.320618},
                         {500.450195},
                         {2012.723022},
                         {1321.840942}};

    float var[7][7] = {{1815.608887,  -1191.323120, -1.592419, 2621.306641,   -1510.704346, 2367.586182,  -3275.495605},
                       {-1191.323486, 1497.972168,  1.198089,  -1986.013428,  1302.457886,  -1739.131226, 2847.540283},
                       {-1.592419,    1.198089,     0.006891,  -6.588072,     3.556423,     -5.555139,    9.689505},
                       {2621.305420,  -1986.012329, -6.588071, 11743.818359,  -4950.315918, 7051.758789,  -11535.416992},
                       {-1510.707275, 1302.459961,  3.556431,  -4950.331543,  8081.208496,  -3964.069336, 6565.253418},
                       {2367.584229,  -1739.127686, -5.555127, 7051.759277,   -3964.061768, 17174.513672, -9432.500000},
                       {-3275.499268, 2847.541748,  9.689510,  -11535.420898, 6565.240723,  -9432.562500, 27102.423828}};
    float **stateDynamic = (float **) malloc(state_size * sizeof(float *));
    for (int i = 0; i < state_size; i++) {
        stateDynamic[i] = (float *) malloc(sizeof(float *));
    }

    for (int i = 0; i < state_size; i++) {
        stateDynamic[i][0] = state[i][0];
    }

    float **varDynamic = (float **) malloc(state_size * sizeof(float *));
    for (int i = 0; i < state_size; i++) {
        varDynamic[i] = (float *) malloc(state_size * sizeof(float *));
    }

    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            varDynamic[i][j] = var[i][j];
        }
    }

    int **grid = getGrid(state_size, stateDynamic, varDynamic, 50, 50, 40, 60);

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 60; j++) {
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }

    int rows = 40;
    int cols = 60;

    int onesIndicesStatic[32][2] = {{9,  12},
                                    {9,  13},
                                    {9,  14},
                                    {9,  15},
                                    {10, 12},
                                    {10, 13},
                                    {10, 14},
                                    {10, 15},
                                    {23, 38},
                                    {23, 39},
                                    {23, 40},
                                    {23, 41},
                                    {24, 38},
                                    {24, 39},
                                    {24, 40},
                                    {24, 41},
                                    {25, 38},
                                    {25, 39},
                                    {25, 40},
                                    {25, 41},
                                    {26, 38},
                                    {26, 39},
                                    {26, 40},
                                    {26, 41},
                                    {27, 38},
                                    {27, 39},
                                    {27, 40},
                                    {27, 41},
                                    {28, 38},
                                    {28, 39},
                                    {28, 40},
                                    {28, 41}};

    int** expectedGrid = generateArrayWithOnesAtIndexes(rows, cols, 32, onesIndicesStatic);

    freeMatrix(rows, cols, grid);
}*/

int test_case1() {
    int state_size = 7;
    float state[7][1] = {{492.619354},
                         {601.678772},
                         {0.909130},
                         {738.320618},
                         {500.450195},
                         {2012.723022},
                         {1321.840942}};

    float var[7][7] = {{1815.608887,  -1191.323120, -1.592419, 2621.306641,   -1510.704346, 2367.586182,  -3275.495605},
                       {-1191.323486, 1497.972168,  1.198089,  -1986.013428,  1302.457886,  -1739.131226, 2847.540283},
                       {-1.592419,    1.198089,     0.006891,  -6.588072,     3.556423,     -5.555139,    9.689505},
                       {2621.305420,  -1986.012329, -6.588071, 11743.818359,  -4950.315918, 7051.758789,  -11535.416992},
                       {-1510.707275, 1302.459961,  3.556431,  -4950.331543,  8081.208496,  -3964.069336, 6565.253418},
                       {2367.584229,  -1739.127686, -5.555127, 7051.759277,   -3964.061768, 17174.513672, -9432.500000},
                       {-3275.499268, 2847.541748,  9.689510,  -11535.420898, 6565.240723,  -9432.562500, 27102.423828}};
    float **stateDynamic = (float **) malloc(state_size * sizeof(float *));
    for (int i = 0; i < state_size; i++) {
        stateDynamic[i] = (float *) malloc(sizeof(float *));
    }

    for (int i = 0; i < state_size; i++) {
        stateDynamic[i][0] = state[i][0];
    }

    float **varDynamic = (float **) malloc(state_size * sizeof(float *));
    for (int i = 0; i < state_size; i++) {
        varDynamic[i] = (float *) malloc(state_size * sizeof(float *));
    }

    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            varDynamic[i][j] = var[i][j];
        }
    }

    int **grid = getGrid(7, stateDynamic, varDynamic, 50, 50, 40, 60);

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 60; j++) {
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }

    freeMatrix(40, 60, grid);
}


int test_case2() {
    int state_size = 7;
    float state[7][1] = {{397.438538},
                         {302.258850},
                         {0.663889},
                         {735.144836},
                         {1633.343628},
                         {1979.211426},
                         {1282.706177}};

    float var[7][7] = {{1010.129150,  -863.828186,  -1.645340,  2262.239258,   -1440.822632, 2425.620605,   -3253.914795},
                       {-863.828491,  1484.675903,  2.481320,   -3011.501953,  2060.429932,  -2999.974121,  5088.693359},
                       {-1.645341,    2.481320,     0.009742,   -10.027654,    6.224670,     -10.035745,    16.638166},
                       {2262.238525,  -3011.499023, -10.027648, 18156.433594,  -8944.758789, 10897.314453,  -17582.300781},
                       {-1440.826050, 2060.433838,  6.224683,   -8944.790039,  12291.801758, -6801.008301,  11104.898438},
                       {2425.621094,  -2999.974121, -10.035745, 10897.320313,  -6800.993164, 32886.863281,  -16521.056641},
                       {-3253.915527, 5088.693359,  16.638166,  -17582.310547, 11104.874023, -16521.312500, 49922.464844}};
    float **stateDynamic = (float **) malloc(state_size * sizeof(float *));
    for (int i = 0; i < state_size; i++) {
        stateDynamic[i] = (float *) malloc(sizeof(float *));
    }

    for (int i = 0; i < state_size; i++) {
        stateDynamic[i][0] = state[i][0];
    }

    float **varDynamic = (float **) malloc(state_size * sizeof(float *));
    for (int i = 0; i < state_size; i++) {
        varDynamic[i] = (float *) malloc(state_size * sizeof(float *));
    }

    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            varDynamic[i][j] = var[i][j];
        }
    }

    int **grid = getGrid(7, stateDynamic, varDynamic, 50, 50, 40, 60);

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 60; j++) {
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }

    freeMatrix(40, 60, grid);
}


int main() {
    /*if (basicTestCaseFromMarco() == 0) {
        printf("basicTestCaseFromMarco() - PASS");
    } else {
        printf("basicTestCaseFromMarco() - FAIL");
    }*/
    if (test_case1() == 0) {
        printf("test_case1() - PASS\n");
    } else {
        printf("test_case1() - FAIL\n");
    }
    if (test_case2() == 0) {
        printf("test_case2() - PASS\n");
    } else {
        printf("test_case2() - FAIL\n");
    }
}
