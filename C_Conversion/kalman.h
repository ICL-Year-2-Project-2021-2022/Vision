#ifndef KALMAN_KALMAN_H
#define KALMAN_KALMAN_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#ifndef KALMAN_KALMAN_TEST_H
#include "library/linalg.h"
#endif



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
    char *color;
};

struct Seen_Land_List{
    size_t size;
    struct Seen_Land item[6];
};

#define res_size 7 //change this base on your size
//return new state and new variance
struct Kal_Res{
    float new_state[res_size][1];
    float new_var[res_size][res_size];
};

struct Kal_Res kalman_filter(size_t state_size, float old_state[state_size][1], float var[state_size][state_size], float displacement[3][1], struct Landmarks land_list, struct Seen_Land_List *seen_list, float control_noise[3][3], float measure_noise[2][2]);

void normalise_angles(size_t state_size, float old_state[state_size][1],float displacement[3][1]);

struct Kal_Res predictionStep(size_t state_size, float old_state[state_size][1], float var[state_size][state_size], float displacement[3][1], float control_noise[3][3]);

void correctionStep(size_t state_size, float old_state[state_size][1],float pred_state[state_size][1], float pred_var[state_size][state_size], struct Landmarks land_list, struct Seen_Land_List *seen_list,float measure_noise[2][2]);

int get_colornum(char *color);

void calculatePredictedState(size_t state_size, float old_state[state_size][1], float displacement[3][1], float pred_state[state_size][1]);

void calculatePredictedVar(size_t state_size, float displacement[3][1], float var[state_size][state_size], float pred_var[state_size][state_size], float control_noise[3][3]);

void setPositionOfNeverSeenLandmark(size_t state_size, float pred_state[state_size][1], struct Seen_Land_List *seen_list, struct Observations observation);

void obtainExpectedObservation(size_t state_size, float pred_state[state_size][1], float x_coor, float y_coor, float delta[2], float exp_dis_ang[2], float* q);

void computeJacobianHMatrix(size_t state_size, float jacobian[2][state_size], int seenLandmarkIndex, float q, float delta[2]);

void computeKalmanGain(size_t state_size, float pred_var[state_size][state_size], float jacobian[2][state_size], float kalman_gain[state_size][2],float measure_noise[2][2]);

void getPredictedStateFromKalmanGain(size_t state_size, float land_dist, float land_ang, float kalman_gain[state_size][2], float exp_dis_ang[2], float pred_state[state_size][1]);

void getPredictedVarFromKalmanGain(size_t state_size, float kalman_gain[state_size][2], float jacobian[2][state_size], float pred_var[state_size][state_size]);

#endif //KALMAN_KALMAN_H
