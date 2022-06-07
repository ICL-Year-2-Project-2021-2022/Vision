#ifndef vec_h
#define vec_h

//basic vector operations

int dotProduct(int vect_A[], int vect_B[], int len){
    int product =0;
    int i;
    for (i=0; i<len; i++){
        product = product + vect_A[i]*vect_B[i];
    }
    return product;
}

void crossProduct(int vect_A[], int vect_B[], int cross_P[]){
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

int neg_scalar(int a){
    return -a;
}

void subtract(int vect_A[], int vect_B[],int len,int dst[]){
    dst[0] = vect_A[0] - vect_B[0];
    dst[1] = vect_A[1] - vect_B[1];
    dst[2] = vect_A[2] - vect_B[2];
}





#endif