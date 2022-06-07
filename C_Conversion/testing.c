#include <math.h>
#include <stdio.h>
#include "library/vec.h"

int main(){
    int arrayA[10]= {0,1,2,3,4,5,6,7,8,9};
    int arrayB[10] ={1,1,1,1,1,1,1,1,1,1};
    printf("%d", dotProduct(arrayA,arrayB,10)); 
}