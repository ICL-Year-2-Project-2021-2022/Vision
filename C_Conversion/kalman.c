#include "kalman.h"

struct Kal_Res kalman_filter(size_t state_size, float old_state[state_size][1], float var[state_size][state_size],
                             float displacement[3][1], struct Landmarks land_list,
                             struct Seen_Land_List seen_list) { //missing R(procc noise) and Q(meas noise) (uncertainty, needs to be defined)
    //only using the first 3 array for x, y, theta, others are for the mappings
    struct Kal_Res predictionResult = predictionStep(state_size, old_state, var, displacement);
    return correctionStep(state_size, predictionResult.new_state, predictionResult.new_var, land_list, seen_list);
}

struct Kal_Res predictionStep(size_t state_size, float old_state[state_size][1], float var[state_size][state_size],
                              float displacement[3][1]) {
    //prediction steps
    //1. Create a N x 3 identity matrix,then multiply the change in x y and thetre to map it into 2N+3 dimensional space
    //2. obtain prediction of x,y,thetre by adding displacements to current state
    //3. Compute matrix Gt
    //4. Compute predicted var matrix with Gt and var
    struct Kal_Res result;
    //state_size = 15; //assume 6 aliens
    calculatePredictedState(state_size, old_state, displacement, result.new_state);
    calculatePredictedVar(state_size, displacement, var, result.new_var);
    return result;
}

struct Kal_Res correctionStep(size_t state_size, float pred_state[state_size][1], float pred_var[state_size][state_size],
               struct Landmarks land_list, struct Seen_Land_List seen_list) {
    // Correction steps in Kalman Filter
    //1. Identify i-th measurement at time t observes th4e landmark with index j (c =j)
    //2. Initialize landmark if unobserved
    //3. Compute the expected observation
    //4. Comute the jacobian of j
    //5. Compute kalman gain
    //6. Sub Kalman gain to obtain new state and variance matrix
    int i, j, k;
    for (i = 0; i < land_list.size; i++) {
        bool seen = false;
        int seenLandmarkIndex = 0;
        for (j = 0; j < seen_list.size; j++) {
            if (strcmp(land_list.item[i].color, seen_list.item[j].color)==0) {
                seen = true;
                seenLandmarkIndex = j;
            }
        }
        // if the landmark was not observed, then we calculate the position of it
        if (!seen) {
            //size_t state_size, float pred_state[state_size][1], struct Seen_Land_List seen_list, struct Observations observation, int itemIndex
            setPositionOfNeverSeenLandmark(state_size, pred_state, seen_list, land_list.item[i]);
        } else {
            //obtain expected observation
            float delta[2], exp_dis_ang[2], q;
            obtainExpectedObservation(state_size, pred_state, seen_list.item[seenLandmarkIndex].x_coor,
                                      seen_list.item[seenLandmarkIndex].y_coor, delta, exp_dis_ang, &q);

            float jacobian[2][state_size];
            computeJacobianHMatrix(state_size, jacobian, seenLandmarkIndex, q, delta);

            float kalman_gain[state_size][2];
            computeKalmanGain(state_size, pred_var, jacobian, kalman_gain);
            
            getPredictedStateFromKalmanGain(state_size, land_list.item[seenLandmarkIndex].land_dist,
                                            land_list.item[seenLandmarkIndex].land_ang, kalman_gain, exp_dis_ang,
                                            pred_state);

            getPredictedVarFromKalmanGain(state_size, kalman_gain, jacobian, pred_var);
             
        }
    }
}

void calculatePredictedState(size_t state_size, float old_state[state_size][1], float displacement[3][1],
                             float pred_state[state_size][1]) {
    int i = 0;
    //state_size = 15; //assume 6 aliens
    float iden_mat[state_size][3];
    memset(iden_mat, 0, state_size * 3 * sizeof(int));
    for (i = 0; i < 3; i++) {
        iden_mat[i][i] = 1;
    }
    float displace_vector[state_size][1];
    matrix_multi(state_size, 3, 3, 1, iden_mat, displacement, displace_vector);

    add_matrix(state_size, 1, old_state, displace_vector, pred_state);
}

void calculatePredictedVar(size_t state_size, float displacement[3][1], float var[state_size][state_size],
                           float pred_var[state_size][state_size]) {
    int i, j = 0;
//get jacobian
    float jacob3x3[3][3] = {{1, 0, -displacement[1][0]},
                            {0, 1, displacement[0][0]},
                            {0, 0, 1}};

    float jacob3x3_trans[3][3];
    transpose(3, 3, jacob3x3, jacob3x3_trans);

    float varxx[3][3];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            varxx[i][j] = var[i][j];
        }
    }
    float tmp[3][3];
    float pre_varxx[3][3]; //only for varxx 3x3 pre, not final pre.
    matrix_multi(3, 3, 3, 3, jacob3x3, varxx, tmp);
    matrix_multi(3, 3, 3, 3, tmp, jacob3x3_trans, pre_varxx);

    float varxm[3][state_size - 3];
    for (i = 0; i < 3; i++) {
        for (j = 3; j < state_size; j++) {
            varxm[i][j - 3] = var[i][j];
        }
    }
    float pre_varvm[3][state_size - 3];
    matrix_multi(3, 3, 3, state_size - 3, jacob3x3, varxm, pre_varvm);
    float pre_varvm_trans[state_size - 3][3];
    transpose(3, state_size - 3, pre_varvm, pre_varvm_trans);

// save memory by not computing state_size x state_size x state_size
    for (i = 0; i < state_size; i++) {
        for (j = 0; j < state_size; j++) {
            if (i < 3 && j < 3) {
                pred_var[i][j] = pre_varxx[i][j];
            } else if (i < 3 && j >= 3) {
                pred_var[i][j] = pre_varvm[i][j - 3];
            }
//not sure if this is correct. The lecture slides might be wrong here
            else if (j < 3 && i >= 3) {
                pred_var[i][j] = pre_varvm_trans[i - 3][j];
            } else {
                pred_var[i][j] = var[i][j];
            }
        }
    }
}

void setPositionOfNeverSeenLandmark(size_t state_size, float pred_state[state_size][1], struct Seen_Land_List seen_list,
                                    struct Observations observation) {
    float x_coor =
            pred_state[0][0] + observation.land_dist * cos(observation.land_ang + pred_state[2][0]);
    float y_coor =
            pred_state[1][0] + observation.land_dist * sin(observation.land_ang + pred_state[2][0]);
//suppose we map a specific color into a specified location in matrix
    struct Seen_Land new_land;
    new_land.x_coor = x_coor;
    new_land.y_coor = y_coor;
    new_land.color = observation.color;
    seen_list.item[seen_list.size] = new_land;
    seen_list.size++;
    int color_num = 10;
    if (strcmp(new_land.color, "red") == 0) {
        color_num = 0;
    } else if (strcmp(new_land.color, "blue") == 0) {
        color_num = 1;
    } else if (strcmp(new_land.color, "green") == 0) {
        color_num = 2;
    } else if (strcmp(new_land.color, "dark_green") == 0) {
        color_num = 3;
    } else if (strcmp(new_land.color, "pink") == 0) {
        color_num = 4;
    } else if (strcmp(new_land.color, "yellow") == 0) {
        color_num = 5;
    }
    if (color_num == 10) {
        fprintf(stderr, "Wrong Color Code");
    }
    pred_state[2 * color_num + 3][0] = x_coor;
    pred_state[2 * color_num + 4][0] = y_coor;
}

void obtainExpectedObservation(size_t state_size, float pred_state[state_size][1], float x_coor, float y_coor, float delta[2],
                          float exp_dis_ang[2], float *q) {
    delta[0] = x_coor - pred_state[0][0];
    delta[1] = y_coor - pred_state[1][0];
    *q = dotProduct(delta, delta, 2);
    exp_dis_ang[0] = sqrt(*q);
    exp_dis_ang[1] = atan2(delta[1], delta[0]) - pred_state[2][0];
}

void computeJacobianHMatrix(size_t state_size, float jacobian[2][state_size], int seenLandmarkIndex, float q,
                            float delta[2]) {
    int i, j;
    float F_matrix[5][state_size];//used to scale up the jacobian matrix
    for (i = 0; i < 5; i++) {
        for (j = 0; j < state_size; j++) {
            if (i < 3 && i == j) {
                F_matrix[i][i] = 1;
            } else if (i == 3 && j == 2 * seenLandmarkIndex + 3) {
                F_matrix[i][j] = 1;
            } else if (i == 4 && j == 2 * seenLandmarkIndex + 4) {
                F_matrix[i][j] = 1;
            } else {
                F_matrix[i][j] = 0;
            }
        }
    }
    //should optimise this line, too many divisions.
    float jacobian_low[2][5] = {{-delta[0] / sqrt(q), -delta[1] / sqrt(q), 0,  delta[0] / sqrt(q),
                                                                                              delta[1] /
                                                                                              sqrt(q)},
                                {delta[1] / q,        -delta[0] / q,       -1, -delta[1] / q, delta[0] / q}};
    matrix_multi(2, 5, 5, state_size, jacobian_low, F_matrix, jacobian);
}

void computeKalmanGain(size_t state_size, float pred_var[state_size][state_size], float jacobian[2][state_size],
                       float kalman_gain[state_size][2]) {
    float trans_jacobian[state_size][2];
    //Compute Kalman Gain
    transpose(2, state_size, jacobian, trans_jacobian);
    float k_tmp1[2][state_size];
    matrix_multi(2, state_size, state_size, state_size, jacobian, pred_var, k_tmp1);
    float k_tmp2[2][2], k_tmp3[2][2];
    matrix_multi(2, state_size, state_size, 2, k_tmp1, trans_jacobian, k_tmp2);
    //(To-Do) need to add Qvar
    gaussian_inverse(2, k_tmp2, k_tmp3);
    float k_tmp4[state_size][2];
    matrix_multi(state_size, state_size, state_size, 2, pred_var, trans_jacobian, k_tmp4);
    //final kalman gain computed
    matrix_multi(state_size, 2, 2, 2, k_tmp4, k_tmp3, kalman_gain);
}

void getPredictedStateFromKalmanGain(size_t state_size, float land_dist, float land_ang, float kalman_gain[state_size][2],
                                     float exp_dis_ang[2], float pred_state[state_size][1]) {
    float z_diff[2][1];
    z_diff[0][0] = land_dist - exp_dis_ang[0];
    z_diff[1][0] = land_ang - exp_dis_ang[1];
    float z_tmp[state_size][1];
    matrix_multi(state_size, 2, 2, 1, kalman_gain, z_diff, z_tmp);
    add_matrix(state_size, 1, pred_state, z_tmp, pred_state);
    int i=0;
    printf("Pred State");
    for (i=0; i<state_size; i++){
        printf("%f ", pred_state[i][0]);
    }
    
}

void getPredictedVarFromKalmanGain(size_t state_size, float kalman_gain[state_size][2], float jacobian[2][state_size],
                                   float pred_var[state_size][state_size]) {
    int i, j;
    float iden_matrix_sxs[state_size][state_size];
    memset(iden_matrix_sxs, 0, state_size * state_size * sizeof(float));
    for (i = 0; i < state_size; i++) {
        iden_matrix_sxs[i][i] = 1;
    }
    float ang_tmp[state_size][state_size], ang_tmp2[state_size][state_size];
    matrix_multi(state_size, 2, 2, state_size, kalman_gain, jacobian, ang_tmp);
    sub_matrix(state_size, state_size, iden_matrix_sxs, ang_tmp, ang_tmp2);
    float new_pred_var[state_size][state_size];
    matrix_multi(state_size, state_size, state_size, state_size, ang_tmp2, pred_var, new_pred_var);    
    for (i = 0; i < state_size; i++) {
        for (j = 0; j < state_size; j++) {
            pred_var[i][j] = new_pred_var[i][j];
        }
    }
}
