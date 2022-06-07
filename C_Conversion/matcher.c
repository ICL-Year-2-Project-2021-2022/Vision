#include <math.h>
#include "library/vec.h"

int * generate_rays(int objects[],int obj_len, int positions[], int pos_len, int image[]){
    int CAMERA_HEIGHT = 110; //mm
    int *rays;
    int i, j =0;
    for ( i =0; i< pos_len; i++ ){
        for (j=0; j<obj_len;j++){
             

        }
    }
}

struct angles_param{
    int horizontal_angle;
    int vertical_angle;
    int est_distance;
};

struct angles_param map_pixel_to_angle(int objects[], int rows, int cols, int images[rows][cols]){
    struct angles_param results;
    int position[0] = objects[0];
    int position[1] = objects[1]; 
    int HORIZONTAL_FOV = (int)(65.0/180.0*M_PI); //Rad 
    //int row = sizeof(images)/ sizeof(images[0]);
    //int column = sizeof(images[0][])/sizeof(images[0][0]);
    int VERTICAL_FOV = (int)((float) rows/ (float)cols * HORIZONTAL_FOV);
    int STAND_HEIGHT = 30; //mm
    int CAMERA_HEIGHT = 110; //mm
    int BALL_RADIUS = 20; //mm
    //relative horizontal and veritcal angel
    results.horizontal_angle = (int)((float)position[0]/(float)cols-0.5)*HORIZONTAL_FOV;
    //size estimation
    results.vertical_angle = -(int)((float)position[1]/(float)rows-0.5)*VERTICAL_FOV;

    //first way size estimation based on veritical angle
    //Only works for points below horizon
    int est_distance_angle = tan(M_PI/2 + results.vertical_angle) * (CAMERA_HEIGHT-STAND_HEIGHT);

    // second way to estimate distance. 
    //Estimates distance based on mask size
    int est_pixel_diameter = sqrt(4 * (float)objects[2]/ M_PI);
    int est_occupied_angle = (int)((float)est_pixel_diameter/(float)cols*HORIZONTAL_FOV);
    int est_distance_size = (int)(float)BALL_RADIUS/(tan((float)est_occupied_angle/2));

    //Average heuristics
    // results {horizontal_angle, vertical_angle, est_distance}
    results.est_distance = (est_distance_size + est_distance_angle)/2;
    return results;

}








//ray[0] = dir
//ray[1] = pos
//ray[2] = color
struct Ray{
    int dir[3];
    int pos[3];
    char color[16];
};

struct Mid_Err{
    int midpoint[3];
    int error;
};

struct Mid_Err find_mid(struct Ray ray1,struct Ray ray2, int len){
    struct Mid_Err results;
    int matrix[2][2];
    matrix[0][0]= dotProduct(ray1.dir, ray1.dir,len);
    matrix[0][1]= neg_scalar(dotProduct(ray2.dir,ray1.dir,len));
    matrix[1][0]= dotProduct(ray2.dir, ray1.dir, len);
    matrix[1][1]= neg_scalar(dotProduct(ray2.dir, ray2.dir, len));
    int point_diff[3];
    subtract(ray1.pos, ray2.pos, 3, point_diff);
    

}

//converting the 2d array into struct

int main(){
    struct Ray r1,r2;
    r1.dir[0] = 1;
    r1.dir[1] = 2;
    r1.dir[2] = 3;
    r2.dir[0] = 1;
    r2.dir[1] = 1;
    r2.dir[2] = 1;
    
}