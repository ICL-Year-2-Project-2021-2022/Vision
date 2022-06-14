#include <math.h>
#include "library/linalg.h"
#include <stdio.h>


struct Object{
    float com_x;
    float com_y;
    float mass;
    char color[20];
};

struct Angles_Param{
    float horizontal_angle;
    float vertical_angle;
    float est_distance;
};

//ray[0] = dir
//ray[1] = pos
//ray[2] = color
struct Ray{
    float dir[3];
    float pos[3];
    char *color;
};

struct Mid_Err{
    float midpoint[3];
    float error;
};

// image is not needed, only objects and dimension
struct Angles_Param map_pixel_to_angle(struct Object ball_param , int rows, int cols){
    struct Angles_Param results;
    float position[2];
    position[0] = ball_param.com_x;
    position[1] = ball_param.com_y; 
    float HORIZONTAL_FOV = (65.0/180.0*M_PI); //Rad 
    //int row = sizeof(images)/ sizeof(images[0]);
    //int column = sizeof(images[0][])/sizeof(images[0][0]);
    float VERTICAL_FOV = (float) rows/ (float)cols * HORIZONTAL_FOV;
    float STAND_HEIGHT = 30; //mm
    float CAMERA_HEIGHT = 110; //mm
    float BALL_RADIUS = 20; //mm
    //relative horizontal and veritcal angel
    results.horizontal_angle = (position[0]/(float)cols-0.5)*HORIZONTAL_FOV;
    //size estimation
    results.vertical_angle = -(position[1]/(float)rows-0.5)*VERTICAL_FOV;

    //first way size estimation based on veritical angle
    //Only works for points below horizon
    float est_distance_angle = tan(M_PI/2 + results.vertical_angle) * (CAMERA_HEIGHT-STAND_HEIGHT);

    // second way to estimate distance. 
    //Estimates distance based on mask size
    float est_pixel_diameter = sqrt(4 * (float)ball_param.mass/ M_PI);
    float est_occupied_angle = ((float)est_pixel_diameter/(float)cols*HORIZONTAL_FOV);
    float est_distance_size = (float)BALL_RADIUS/(tan((float)est_occupied_angle/2));

    //Average heuristics
    // results {horizontal_angle, vertical_angle, est_distance}
    results.est_distance = (est_distance_size + est_distance_angle)/2;
    return results;

}

int generate_rays(int objects[],int obj_len, int positions[], int pos_len, int image[]){
    int CAMERA_HEIGHT = 110; //mm
    int *rays;
    int i, j =0;
    for ( i =0; i< pos_len; i++ ){
        for (j=0; j<obj_len;j++){
             

        }
    }
}






//verified works
struct Mid_Err find_mid(struct Ray ray1,struct Ray ray2, size_t len){
    
    struct Mid_Err results;
    float matrix[2][2];
    matrix[0][0]= dotProduct(ray1.dir, ray1.dir,len);
    matrix[0][1]= neg_scalar(dotProduct(ray2.dir,ray1.dir,len));
    matrix[1][0]= dotProduct(ray2.dir, ray1.dir, len);
    matrix[1][1]= neg_scalar(dotProduct(ray2.dir, ray2.dir, len));
    //printf("matrix, %f, %f,%f,%f\n", matrix[0][0],matrix[0][1],matrix[1][0],matrix[1][1]);
    float point_diff[3];
    sub_vectors(ray1.pos, ray2.pos, 3, point_diff);
    //printf("point_diff, %f, %f,%f", point_diff[0], point_diff[1], point_diff[2]);
    float vector[2];
    vector[0]= neg_scalar(dotProduct(point_diff, ray1.dir, len));
    vector[1]= neg_scalar(dotProduct(point_diff, ray2.dir, len));
    //printf("vector: %f, %f", vector[0], vector[1]);
    float sol[2];
    solve2x2(matrix, vector, sol);
    float r = sol[0];
    float s = sol[1];
    //printf("r,s,%f,%f", r,s);
    
    //point1 = np.add(ray1[1],np.multiply( r,  ray1[0]))
    float mult_sol1[3];
    multi_scalar_vector(r, ray1.dir, len, mult_sol1);
    float point1[3];
    add_vectors(ray1.pos, mult_sol1, len, point1);
    //printf("point1:%f, %f,%f\n", point1[0], point1[1], point1[2]);
    float mult_sol2[3];
    //point2 = np.add(ray2[1], np.multiply( s,  ray2[0]))
    multi_scalar_vector(s,ray2.dir, len, mult_sol2);
    float point2[3];
    add_vectors(ray2.pos, mult_sol2, len, point2);
    
    //printf("point2:%f, %f,%f\n", point2[0], point2[1], point2[2]);
    
    //  midpoint = np.add(point1, point2)/2
    float p1p2[3];
    add_vectors(point1, point2, len, p1p2);
    //printf("added: %f, %f, %f", p1p2[0],p1p2[1], p1p2[2]);
    float divide_sol[3];
    divide_scalar_vector(2, p1p2, len, divide_sol);
    memcpy(results.midpoint,divide_sol, sizeof(results.midpoint));
    //printf("midpt: %f,%f,%f", results.midpoint[0], results.midpoint[1], results.midpoint[2]);

    float sub_sol[3];
    sub_vectors(point1, point2, len, sub_sol);
    results.error = norm(sub_sol, 3);
    //printf("err: %f", results.error);
    return results;
}

int test_map_pixel_to_angle(){
    float precision= 0.0001;
    int row = 480;
    int col = 640;
    struct Object ball= { .com_x = 167, .com_y =324,.mass= 1596.0,.color ="yellow"};
    struct Angles_Param result;
    result  = map_pixel_to_angle(ball, row, col);
    if(result.horizontal_angle-precision<-0.2712078032 && result.horizontal_angle+precision>-0.2712078032){
        if (result.vertical_angle-precision<-0.14889840181076 && result.vertical_angle+precision>-0.14889840181076){
            if (result.est_distance-precision<516.8105540083 && result.est_distance+precision>516.8105540083){
                printf("%s", "Map Pixel to Angle Passed\n");
                return 0;
            }
        }
    }
    printf("%s", "Map Pixel to Angle Failed\n");
    return -1;



}
int test_find_mid(){
    struct Ray r1 = {.dir = {0.965287862173886, -0.06683821269429235, -0.252491577015158}, .pos = {0.0, -125.0, 110.0}, .color = "red"};
    struct Ray r2 ={.dir = {0.8576753190942117, -0.44499217565015714, -0.2576334811836026}, .pos = {0, 0, 110}, .color = "red"};
    struct Mid_Err results;
    results = find_mid(r1,r2,3);
    float precision = 1;
    if (results.midpoint[0]-precision<277 && results.midpoint[0]+precision>277){
        if (results.midpoint[1]-precision<-143 && results.midpoint[1]+precision>-143){
            if (results.midpoint[2]-precision<32 && results.midpoint[2]+precision>32){
                if (results.error-precision<10 && results.error+precision>10){
                    printf("Find_Mid Passed\n");
                    return 0;
                }
            }
        } 
    }
    printf("Find_Mid Failed\n");
    return -1;
}

//converting the 2d array into struct

int main(){
    
    test_find_mid();
    test_map_pixel_to_angle();
    
}