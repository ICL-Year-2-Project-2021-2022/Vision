#ifndef vec_h
#define vec_h

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//basic vector operations

float dotProduct(float vect_A[], float vect_B[], int len){
    float product =0;
    int i;
    for (i=0; i<len; i++){
        product = product + vect_A[i]*vect_B[i];
    }
    return product;
}

void crossProduct(float vect_A[], float vect_B[], float cross_P[]){
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

float det_2x2(float matrix[][2]){
    return matrix[0][0]*matrix[1][1]- matrix[0][1]*matrix[1][0];
}

float det_3x3(float matrix[][3]){
    return matrix[0][0]* matrix[1][1]*matrix[2][2] + matrix[0][1]*matrix[1][2]*matrix[2][0] + matrix[0][2]*matrix[1][0]*matrix[2][1] - matrix[0][0]* matrix[1][2]*matrix[2][1]- matrix[0][1]*matrix[1][0]*matrix[2][2]- matrix[0][2]*matrix[1][1]*matrix[2][0];
}

void solve2x2(float matrix[][2], float vector[], float dst[]){
    //using cramer's rule
    
    float det = det_2x2(matrix);
    if (det==0){
        fprintf(stderr, "Determinant is 0!\n");
        exit(-1);
    }
    float x1[2][2] = { {vector[0],vector[1]}, {matrix[0][1], matrix[1][1]}};
    
    float x2[2][2] = { {matrix[0][0], matrix[1][0]}, {vector[0], vector[1]}};
    dst[0] = det_2x2(x1)/det ;
    dst[1] = det_2x2(x2)/det ;
    //dst[0] = det_2x2(x1)/det;
    //dst[1] = det_2x2(x2)/det;
}

void multi_scalar_vector(int c, float vector[], size_t len, float dst[]){
    int i=0;
    for (i=0; i<len; i++){
        dst[i]=vector[i]*c;
        printf("res:%d", dst[i]);
    }    
}

void divide_scalar_vector(int c, float vector[], size_t len, float dst[]){
    int i=0;
    for (i=0; i<len; i++){
        dst[i]= vector[i]/c;
    }
}

void add_vectors(float vector1[], float vector2[], size_t len, float dst[]){
    int i=0;
    for(i=0; i<len; i++){
        dst[i] = vector1[i]+vector2[i];
    }
}

void sub_vectors(float vect_A[], float vect_B[],size_t len,float dst[]){
    int i=0;
    for(i=0; i<len; i++){
        dst[i] = vect_A[i]-vect_B[i];
    }
}

float neg_scalar(float a){
    return -a;
}


float norm(float vect_A[], size_t len){
    int i=0;
    float sum=0;
    for (i=0; i<len; i++){
        sum=sum+pow(vect_A[i],2);
    }
    return sqrt(sum);
}


// void gaussian_inverse(size_t side, float vect_A[][side]){
//     int i,j,k =0;
//     //int result[][side] = {0};
//     int factor=0;
//     //initialise identity matrix
//     for( i=0; i<side; i++){
//         result[i][i]=1;
//     }

//     for (i=0; i<side; i++){
//         for (j=i; j<side; j++){
//             if (j==i && vect_A[i][i]!=1){
//                 factor = 1/vect_A[i][i];
//                 multi_scalar_vector(factor, result);
//             } 
//         }
//     }

// }




// iterate column until bottom, if i=j, divide by the value, else, subtract by row values.
// iterate from top down, right to left







#endif