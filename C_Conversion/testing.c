#include<stdio.h>
#include "library/linalg.h"
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


struct Land{
    float x;
    float y;
    char *color;
};

struct Land_List{
    size_t size;
    struct Land items[10];
};

struct Seen_List{
    size_t size;
    struct Land items[10];
};

void find_color(struct Seen_List seen_list, struct Land_List land_list){
    int i,j=0;
    for (i=0; i<land_list.size; i++){
        for(j=0; j<seen_list.size; j++){
            if(land_list.items[i].color == seen_list.items[j].color){
                printf("%s", land_list.items[i].color);
            }
        }
    }
}

void test_find_color(){
    struct Land l1,l2;
    l1.x=1;
    l1.y=2;
    l1.color= "red";
    l2.x=2;
    l2.y=4;
    l2.color= "blue";
    struct Land_List list1;
    list1.size=0;
    list1.items[list1.size]=l1;
    list1.size++;
    list1.items[list1.size]=l2;
    list1.size++;

    struct Seen_List seen_list;
    seen_list.size=0;
    seen_list.items[seen_list.size]=l2;
    seen_list.size++;
    find_color(seen_list,list1);
    //should display blue
}


int main(){
    //test_row_switch();
    //test_inverse1();
    //test_inverse2();
    //test_matrix_multi();
    test_find_color();
    
    
    return 0;
}
