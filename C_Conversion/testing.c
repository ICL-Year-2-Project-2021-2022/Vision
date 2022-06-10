#include<stdio.h>
#include "library/vec.h"
#include <stdlib.h>


int test_row_switch(){
    float matrix[3][3] = {{1.0,2.0, 5.0},{3.0,4.0 , -1.0},{2.5, 1.0, 1.5}};
    row_operation(matrix[0], matrix[2],3);
    if (matrix[0][0]==2.5 && matrix[0][1]== 1.0 && matrix[0][2] ==1.5){
        if(matrix[2][0]==1.0 && matrix[2][1]==2.0 && matrix[2][2]==5.0){
            printf("%s", "Row Switch Passed");
            return 0;
        }
    }
    printf("%s", "Row Switch Failed");
    return -1;
}

int test_inverse1(){
    float matrix[3][3] = {{1.0,2.0, 5.0},{3.0,4.0 , -1.0},{2.0, 1.0, 1.0}};
    
    gaussian_inverse(3, matrix,matrix);
}

int test_inverse2(){
    float matrix[5][5] = {{1.0,2.0, 5.0, 3.5, 6},{2.0,4.0 , 2.7, 5.9,2 },{100, 1.0, 1.0,3.8,9.2},{1.5,9.5,2,4,18},{3,8,9.7,10,2}};
    
    gaussian_inverse(sizeof(matrix[0])/sizeof(matrix[0][0]), matrix,matrix); 
    
}

int test_matrix_multi(){
    //to find row of a matrix: (sizeof(a)/sizeof(a[0]))
    //to find column of a matrix: (sizeof(a)/ sizeof(a[0][0]))/row
    float matrixA[1][3] = {{1,2,2}};
    float matrixB[3][3] = {{2,6,0},{3,1,9},{4,8,2}};
    int row_A = sizeof(matrixA)/sizeof(matrixA[0]);
    int col_A = sizeof(matrixA)/sizeof(matrixA[0][0])/row_A;
    int row_B = sizeof(matrixB)/sizeof(matrixB[0]);
    int col_B = sizeof(matrixB)/sizeof(matrixB[0][0])/row_B;

    float matrixC[row_A][col_B];
    matrix_multi(row_A, col_A, row_B, col_B,matrixA, matrixB, matrixC);
    int i,j=0;
    for (i=0; i<row_A; i++){
        for (j=0; j<col_B; j++){
            printf("%f ", matrixC[i][j]);
        }
        printf("\n");
    }
}


int test_transpose(){
    float matrixA[3][3] = {{2,6,0},{3,1,9},{4,8,2}};
    int row = sizeof(matrixA)/sizeof(matrixA[0]);
    int col = sizeof(matrixA)/sizeof(matrixA[0][0])/row;
    float transposed[col][row];
    transpose(row, col, matrixA, transposed);
    int i,j=0;
    for (i=0; i<row; i++){
        for (j=0; j<col; j++){
            printf("%f ", transposed[i][j]);
        }
        printf("\n");
    }


}



int main(){
    //test_row_switch();
    //test_inverse1();
    //test_inverse2();
    //test_matrix_multi();
    //test_transpose();
    // float matrix[2][2] = {{1.0,2.0, 5.0},{3.0,4.0 , -1.0},{2.5, 1.0, 1.5}};
    // float iden[2][2] = {{1.0,0.0},{0.0,1.0}};
    // printf("%f,%f", iden[0][0], iden[0][1]);
    // multi_scalar_vector(2,matrix[0],2, iden[0]);
    // printf("%f,%f", iden[0][0], iden[0][1]);
    // int vector1[2] = {3, 6};
    // int vector2[2] = {4,6}; 
    // printf("norm %d", norm(vector1,2));
    // int sol[2] = {};
    // add_vectors(vector1, vector2, sizeof(vector1)/sizeof(vector1[0]),sol);
    // printf("det,%d\n",det_2x2(matrix));
    // printf("sol, %d, %d\n",sol[0], sol[1]);
    
     
    // int matrix3[3][3] = {{1,2,3},{4,5,6},{2,3,8}};
    // printf("det3,%d\n",det_3x3(matrix3));
    // int result[2];
    // solve2x2(matrix, vector1, result);
    // printf("%d, %d",result[0], result[1]);
    return 0;

}
