#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int **getGrid(size_t state_size,float state[7][1], float var[7][7]){
    int grid[40][60]= {0};
    for (int i=0;i<(state_size-3)/2; i++ ){
        int radius_x, radius_y;
        int numSquaresOccupiedRadius_x,numSquaresOccupiedRadius_y ;
        int offset_x;
        int offset_y;
        
        
        radius_x = (int)sqrt(var[2*i+3][2*i+3]);
        numSquaresOccupiedRadius_x = radius_x/50;
        offset_x =   state[2*i+3][0]/50;
        offset_y =   state[2*i+4][0]/50;
    
        radius_y = (int)sqrt(var[2*i+4][2*i+4]);
        numSquaresOccupiedRadius_y = radius_y/50;

        

        
        for (int i = -numSquaresOccupiedRadius_y; i<numSquaresOccupiedRadius_y; i++){
            for (int j=  -numSquaresOccupiedRadius_x; j<numSquaresOccupiedRadius_x; j++){
                if (i+offset_y<0 || j+offset_x<0 ){
                    continue;
                }
                if (i+offset_y>40 || j+offset_x>60){
                    continue;
                }
                grid[i+offset_y][j+offset_x]=1;
            }
        }

       
    }
    for (int i=0; i<40; i++){
        for (int j=0; j<60; j++){
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }

}


int main(){
    float state[7][1] = {{492.619354}, {601.678772}, {0.909130}, {738.320618}, {500.450195}, {2012.723022}, {1321.840942} };

    float var[7][7] =  {{1815.608887, -1191.323120 ,-1.592419 ,2621.306641 ,-1510.704346, 2367.586182, -3275.495605},
                        {-1191.323486, 1497.972168 ,1.198089 ,-1986.013428, 1302.457886, -1739.131226 ,2847.540283},
                        {-1.592419, 1.198089 ,0.006891 ,-6.588072, 3.556423 ,-5.555139, 9.689505 },
                        {2621.305420, -1986.012329, -6.588071, 11743.818359, -4950.315918, 7051.758789, -11535.416992},
                        {-1510.707275, 1302.459961, 3.556431, -4950.331543, 8081.208496, -3964.069336, 6565.253418},
                        {2367.584229, -1739.127686, -5.555127, 7051.759277, -3964.061768, 17174.513672, -9432.500000 },
                        {-3275.499268, 2847.541748, 9.689510, -11535.420898, 6565.240723, -9432.562500, 27102.423828}};
    int **grid = getGrid(7, state, var);
}