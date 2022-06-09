#include<stdio.h>
#include "library/vec.h"


int main(){
    float matrix[2][2] = {{1.0,2.0},{3.0,4.0}};
    float iden[2][2] = {{1.0,0.0},{0.0,1.0}};
    printf("%f,%f", iden[0][0], iden[0][1]);
    multi_scalar_vector(2,matrix[0],2, iden[0]);
    printf("%f,%f", iden[0][0], iden[0][1]);
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
