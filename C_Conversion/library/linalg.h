#ifndef linalg_h
#define linalg_h

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//basic vector operations

float dotProduct(float vect_A[], float vect_B[], int len) {
    float product = 0;
    int i;
    for (i = 0; i < len; i++) {
        product = product + vect_A[i] * vect_B[i];
    }
    return product;
}

void crossProduct(float vect_A[], float vect_B[], float cross_P[]) {
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

float det_2x2(float matrix[][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

float det_3x3(float matrix[][3]) {
    return matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] +
           matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][0] * matrix[1][2] * matrix[2][1] -
           matrix[0][1] * matrix[1][0] * matrix[2][2] - matrix[0][2] * matrix[1][1] * matrix[2][0];
}

void solve2x2(float matrix[][2], float vector[], float dst[]) {
    //using cramer's rule

    float det = det_2x2(matrix);
    if (det == 0) {
        fprintf(stderr, "Determinant is 0!\n");
        //exit(-1);
    }
    float x1[2][2] = {{vector[0],    vector[1]},
                      {matrix[0][1], matrix[1][1]}};

    float x2[2][2] = {{matrix[0][0], matrix[1][0]},
                      {vector[0],    vector[1]}};
    dst[0] = det_2x2(x1) / det;
    dst[1] = det_2x2(x2) / det;
    //dst[0] = det_2x2(x1)/det;
    //dst[1] = det_2x2(x2)/det;
}

void multi_scalar_vector(float c, float vector[], size_t len, float dst[]) {
    int i = 0;
    for (i = 0; i < len; i++) {
        dst[i] = vector[i] * c;
    }
}

void divide_scalar_vector(float c, float vector[], size_t len, float dst[]) {
    int i = 0;
    for (i = 0; i < len; i++) {
        dst[i] = vector[i] / c;
    }
}


void add_vectors(float vector1[], float vector2[], size_t len, float dst[]) {
    int i = 0;
    for (i = 0; i < len; i++) {
        dst[i] = vector1[i] + vector2[i];
    }
}

void add_matrix(size_t row, size_t col, float matrix1[row][col], float matrix2[row][col], float dst[row][col]) {
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            dst[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}

void sub_matrix(size_t row, size_t col, float matrix1[row][col], float matrix2[row][col], float dst[row][col]) {
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            dst[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
}


void sub_vectors(float vect_A[], float vect_B[], size_t len, float dst[]) {
    int i = 0;
    for (i = 0; i < len; i++) {
        dst[i] = vect_A[i] - vect_B[i];
    }
}

float neg_scalar(float a) {
    return -a;
}


float norm(float vect_A[], size_t len) {
    int i = 0;
    float sum = 0;
    for (i = 0; i < len; i++) {
        sum = sum + pow(vect_A[i], 2);
    }
    return sqrt(sum);
}

//switch row 1 and row 2
void row_operation(float row1[], float row2[], size_t side) {
    float tmp[side];
    memset(tmp, 0, side * sizeof(float));
    int i = 0;
    //copying row content
    for (i = 0; i < side; i++) {
        tmp[i] = row1[i];
        row1[i] = row2[i];
        row2[i] = tmp[i];
    }
}


void matrix_multi(size_t row_A, size_t col_A, size_t row_B, size_t col_B, float matrix_A[row_A][col_A],
                  float matrix_B[row_B][col_B], float dst[row_A][col_B]) {
    if (col_A != row_B) {
        printf("Wrong Matrix Size - Mult Error");
    }
    int i, j, k = 0;
    for (i = 0; i < row_A; i++) {
        for (j = 0; j < col_B; j++) {
            float sum = 0;
            for (k = 0; k < col_A; k++) {
                sum = sum + matrix_A[i][k] * matrix_B[k][j];
            }
            dst[i][j] = sum;
        }
    }
}


void transpose(size_t row, size_t col, float matrix[row][col], float dst[col][row]) {
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            dst[j][i] = matrix[i][j];
        }
    }
}


//modified Gauss Jordon inverse, get echelon form and bottom up reduce upper trangular.
void gaussian_inverse(size_t side, float matrix_A[][side], float dst[][side]) {
    int i, j, k, l = 0;
    float result[side][side];
    memset(result, 0, side * side * sizeof(float));
    float factor = 0;
    //initialise identity matrix
    for (i = 0; i < side; i++) {
        result[i][i] = 1;
    }
    //combine both matrix
    float matrix_comb[side][2 * side];
    for (i = 0; i < side; i++) {
        for (j = 0; j < 2 * side; j++) {
            if (j >= side) {
                matrix_comb[i][j] = result[i][j - side];
            } else {
                matrix_comb[i][j] = matrix_A[i][j];
            }
        }
    }

    // iterate column until bottom, process lower half trangular
    // 1. if i=j and not eq 1/0, divide by the value, else, subtract by row values.
    // 2. if i=j and eq 0, swap row with one that is not
    // 3. if i!=j, then divide row by its value, then subtract by row with 1
    // iterate upper trangular bottom up, same process. 

    //i is row, j is column {{1,2,3},{4,5,6},{7,8,9}}
    for (j = 0; j < side; j++) {
        for (i = j; i < side; i++) {
            //if on diagonal and not equal to 1
            if (j == i && matrix_comb[i][i] != 1) {

                //if on diagonal and equal to 0, switch rows.
                //Amortized O(1)
                if (matrix_comb[i][i] == 0) {
                    for (k = i; k < side; k++) {
                        if (matrix_comb[k][j] != 0) {
                            row_operation(matrix_comb[i], matrix_comb[k], side * 2);
                            break;
                        }
                        if (k + 1 == side) {
                            fprintf(stderr, "Error, Determanent is 0", 1);
                            //exit(-1);
                        }
                    }
                    // int m=0;
                    // for (m=0; m<side; m++){
                    //     printf("%f", matrix_comb[i][m]);
                    // }

                }

                //else divide so diagonal becomes 1
                if (matrix_comb[i][i] != 1) {
                    factor = 1 / matrix_comb[i][i];
                    multi_scalar_vector(factor, matrix_comb[i], 2 * side, matrix_comb[i]);
                }
            }
            // if it is not 0, then deduct it by row
            if (i != j && matrix_comb[i][j] != 0) {
                if (matrix_comb[i][j] != 1) {
                    factor = 1 / matrix_comb[i][j];

                    multi_scalar_vector(factor, matrix_comb[i], 2 * side, matrix_comb[i]);

                }
                sub_vectors(matrix_comb[i], matrix_comb[j], side * 2, matrix_comb[i]);

            }
        }
    }

    //upper triangle processing
    for (i = side - 2; i >= 0; i--) {
        for (j = side - 1; j > i; j--) {
            if (matrix_comb[i][j] != 0) {
                float mult_sol[2 * side];
                multi_scalar_vector(matrix_comb[i][j], matrix_comb[j], 2 * side, mult_sol);
                sub_vectors(matrix_comb[i], mult_sol, 2 * side, matrix_comb[i]);
            }
        }
    }

    //storing result values into dst
    for (i = 0; i < side; i++) {
        for (j = side; j < side * 2; j++) {
            dst[i][j - side] = matrix_comb[i][j];
        }
    }
    // printing out matrix
    printf("Inverse Matrix: \n");
    for (i=0; i<side; i++){
        for (j=0; j<2*side; j++){
            printf("%f ",matrix_comb[i][j]);
            if (j==(2*side-1)){
                printf("\n");
            }
        }
    }


}


#endif