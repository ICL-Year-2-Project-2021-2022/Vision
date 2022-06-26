#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int **getGrid(size_t state_size, size_t row, size_t col, size_t cellSideLength, float state[7][1], float var[7][7]){
    int **grid;
    grid = malloc(sizeof(int*)*row);
    for (int i =0; i<row; i++){
        grid[i] = malloc(sizeof(int*)*col);
    }
    for(int i=0; i<row ;i++){
        for (int j=0; j<col; j++){
            grid[i][j] = 0;
        }
    }

    
    for (int i=0;i<(state_size-3)/2; i++ ){
        float radius_x, radius_y;
        int numSquaresOccupiedRadius_x,numSquaresOccupiedRadius_y ;
        int offset_x;
        int offset_y;
        
        
        radius_x = sqrt(var[2*i+3][2*i+3]);
        numSquaresOccupiedRadius_x =ceil(radius_x/cellSideLength);
        offset_x =   state[2*i+3][0]/cellSideLength;
        offset_y =   state[2*i+4][0]/cellSideLength;
    
        radius_y = ceil(sqrt(var[2*i+4][2*i+4]));
        numSquaresOccupiedRadius_y  =ceil(radius_y/100);

        

        
        for (int i = -numSquaresOccupiedRadius_y; i<=numSquaresOccupiedRadius_y; i++){
            for (int j=  -numSquaresOccupiedRadius_x; j<=numSquaresOccupiedRadius_x; j++){
                if (i+offset_y<0 || j+offset_x<0 ){
                    continue;
                }
                if (i+offset_y>=row || j+offset_x>=col){
                    continue;
                }
                grid[i+offset_y][j+offset_x]=1;
            }
        }

       
    }
    return grid;

}


void freeMatrix(size_t row, size_t col, int **grid){
    for (int i=0;i <row; i++){
        free(grid[i]);
    }
}


// int test_case1(){
//     float state[7][1] = {{492.619354}, {601.678772}, {0.909130}, {738.320618}, {500.450195}, {2012.723022}, {1321.840942} };

//     float var[7][7] =  {{1815.608887, -1191.323120 ,-1.592419 ,2621.306641 ,-1510.704346, 2367.586182, -3275.495605},
//                         {-1191.323486, 1497.972168 ,1.198089 ,-1986.013428, 1302.457886, -1739.131226 ,2847.540283},
//                         {-1.592419, 1.198089 ,0.006891 ,-6.588072, 3.556423 ,-5.555139, 9.689505 },
//                         {2621.305420, -1986.012329, -6.588071, 11743.818359, -4950.315918, 7051.758789, -11535.416992},
//                         {-1510.707275, 1302.459961, 3.556431, -4950.331543, 8081.208496, -3964.069336, 6565.253418},
//                         {2367.584229, -1739.127686, -5.555127, 7051.759277, -3964.061768, 17174.513672, -9432.500000 },
//                         {-3275.499268, 2847.541748, 9.689510, -11535.420898, 6565.240723, -9432.562500, 27102.423828}};
//     int **grid = getGrid(7, state, var);

//     for (int i=0; i<40; i++){
//         for (int j=0; j<60; j++){
//             printf("%d",grid[i][j]);
//         }
//         printf("\n");
//     }

//     freeMatrix(40,60, grid);
// }


// int test_case2(){
//     float state[7][1] = {{397.438538}, {302.258850} ,{0.663889}, {735.144836}, {1633.343628}, {1979.211426}, {1282.706177}};

//     float var[7][7] =  {{1010.129150, -863.828186 ,-1.645340, 2262.239258, -1440.822632 ,2425.620605 ,-3253.914795},
// {-863.828491, 1484.675903, 2.481320, -3011.501953 ,2060.429932, -2999.974121, 5088.693359},
// {-1.645341 ,2.481320, 0.009742 ,-10.027654 ,6.224670 ,-10.035745 ,16.638166},
// {2262.238525, -3011.499023, -10.027648, 18156.433594 ,-8944.758789, 10897.314453, -17582.300781},
// {-1440.826050 ,2060.433838, 6.224683, -8944.790039 ,12291.801758 ,-6801.008301, 11104.898438},
// {2425.621094 ,-2999.974121, -10.035745 ,10897.320313 ,-6800.993164, 32886.863281, -16521.056641},
// {-3253.915527 ,5088.693359 ,16.638166 ,-17582.310547 ,11104.874023 ,-16521.312500, 49922.464844}};
//     int **grid = getGrid(7, state, var);

//     for (int i=0; i<40; i++){
//         for (int j=0; j<60; j++){
//             printf("%d",grid[i][j]);
//         }
//         printf("\n");
//     }

//     freeMatrix(40,60, grid);
// }


int test_case3(){
    float state[7][1] = {{0}, {0} ,{0}, {250}, {650}, {650}, {1050}};

    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.03, 0, 0 ,0 ,0},
                       {0, 0, 0, 22500, 0 ,0,0},
                       {0, 0, 0, 0, 22500 ,0,0},
                       {0,0 , 0, 0, 0, 22500 , 0},
                       {0,0, 0,0,0,0,22500}};
    int **grid = getGrid(7, 20, 20, 100, state, var);

    for (int i=0; i<20; i++){
        for (int j=0; j<20; j++){
            printf("%d",grid[i][j]);
        }
        printf("\n");
    }

    freeMatrix(40,60, grid);
}




int main(){
    test_case3();
}