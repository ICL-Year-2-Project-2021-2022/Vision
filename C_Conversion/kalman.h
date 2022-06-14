#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "library/linalg.h"

#ifndef KALMAN_KALMAN_H
#define KALMAN_KALMAN_H


struct Observations{
    float land_dist;
    float land_ang;
    char *color;
};

struct Landmarks{
    size_t size;
    struct Observations item[6];
};

struct Seen_Land{
    float x_coor;
    float y_coor;
    char *color;
};

struct Seen_Land_List{
    size_t size;
    struct Seen_Land item[6];
};

//return new state and new variance
struct Kal_Res{
    float new_state[15][1];
    float new_var[15][15];
};

struct Kal_Res kalman_filter(size_t state_size, float old_state[state_size][1], float var[state_size][state_size], float displacement[3][1], struct Landmarks land_list, struct Seen_Land_List seen_list);

struct Kal_Res predictionStep(size_t state_size, float old_state[state_size][1], float var[state_size][state_size], float displacement[3][1]);

struct Kal_Res correctionStep(size_t state_size, float pred_state[state_size][1], float pred_var[state_size][state_size], struct Landmarks land_list, struct Seen_Land_List seen_list);

void calculatePredictedState(size_t state_size, float old_state[state_size][1], float displacement[3][1], float pred_state[state_size][1]);

void calculatePredictedVar(size_t state_size, float displacement[3][1], float var[state_size][state_size], float pred_var[state_size][state_size]);

void setPositionOfNeverSeenLandmark(size_t state_size, float pred_state[state_size][1], struct Seen_Land_List seen_list, struct Observations observation);

#endif //KALMAN_KALMAN_H
