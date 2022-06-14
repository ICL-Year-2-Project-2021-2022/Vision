#include "kalman.h"

struct Kal_Res kalman_filter(size_t state_size, float old_state[state_size][1], float var[state_size][state_size],
        float displacement[3][1], struct Landmarks land_list, struct Seen_Land_List seen_list) { //missing R(procc noise) and Q(meas noise) (uncertainty, needs to be defined)
    //only using the first 3 array for x, y, theta, others are for the mappings
    struct Kal_Res predictionResult = predictionStep(state_size, old_state, var, displacement);
    return correctionStep(state_size, predictionResult.new_state, predictionResult.new_var, land_list, seen_list);
}

struct Kal_Res predictionStep(size_t state_size, float old_state[state_size][1], float var[state_size][state_size], float displacement[3][1]) {
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
        int c = 0;
        float x_coor, y_coor; //x and y coordinate of landmark, used to compute delta, see below.
        for (j = 0; j < seen_list.size; j++) {

            if (strcmp(land_list.item[i].color, seen_list.item[j].color)) {
                seen = true;
                c = j;
            }

        }
        // if the landmark was not observed, then we calculate the position of it

        if (!seen) {
            x_coor =
                    pred_state[0][0] + land_list.item[i].land_dist * cos(land_list.item[i].land_ang + pred_state[2][0]);
            y_coor =
                    pred_state[1][0] + land_list.item[i].land_dist * sin(land_list.item[i].land_ang + pred_state[2][0]);
            //suppose we map a specific color into a specified location in matrix
            struct Seen_Land new_land;
            new_land.x_coor = x_coor;
            new_land.y_coor = y_coor;
            new_land.color = land_list.item[i].color;
            seen_list.item[seen_list.size] = new_land;
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
        } else {
            x_coor = seen_list.item[c].x_coor;
            y_coor = seen_list.item[c].y_coor;
            //obtain expected observation
            float delta[2];//distance of landmark to robot
            delta[0] = x_coor - pred_state[0][0];
            delta[1] = y_coor - pred_state[1][0];
            float q = dotProduct(delta, delta, 2);
            float exp_dis_ang[2]; //z
            exp_dis_ang[0] = sqrt(q);
            exp_dis_ang[1] = atan2(delta[1], delta[0]) - pred_state[2][0];

            //Compute jacobian matrix H
            float F_matrix[5][state_size];//used to scale up the jacobian matrix
            for (j = 0; j < 5; j++) {
                for (k = 0; k < state_size; k++) {
                    if (j < 3 && j == k) {
                        F_matrix[j][j] = 1;
                    } else if (j == 3 && k == 2 * c + 3) {
                        F_matrix[j][k] = 1;
                    } else if (j == 4 && k == 2 * c + 4) {
                        F_matrix[j][k] = 1;
                    } else {
                        F_matrix[j][k] = 0;
                    }
                }
            }
            //should optimise this line, too many divisions.
            float jacobian_low[2][5] = {{-delta[0] / sqrt(q), -delta[1] / sqrt(q), 0,  delta[0] / sqrt(q),
                                                                                                      delta[1] /
                                                                                                      sqrt(q)},
                                        {delta[1] / q,        -delta[0] / q,       -1, -delta[1] / q, delta[0] / q}};
            float jacobian[2][state_size];
            matrix_multi(2, 5, 5, state_size, jacobian_low, F_matrix, jacobian);

            //Compute Kalman Gain
            float kalman_gain[state_size][2];
            float trans_jacobian[state_size][2];
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

            //get new predicted state
            float z_diff[2][1];
            z_diff[0][0] = land_list.item[c].land_dist - exp_dis_ang[0];
            z_diff[1][0] = land_list.item[c].land_ang - exp_dis_ang[1];
            float z_tmp[state_size][1];
            matrix_multi(state_size, 2, 2, 2, kalman_gain, z_diff, z_tmp);
            add_matrix(state_size, 1, pred_state, z_tmp, pred_state);

            //get new predicted angle
            float iden_matrix_sxs[state_size][state_size];
            memset(iden_matrix_sxs, 0, state_size * state_size * sizeof(float));
            for (j = 0; j < state_size; j++) {
                iden_matrix_sxs[j][j] = 1;
            }
            float ang_tmp[state_size][state_size], ang_tmp2[state_size][state_size];
            matrix_multi(state_size, 2, 2, state_size, kalman_gain, jacobian, ang_tmp);
            sub_matrix(state_size, state_size, iden_matrix_sxs, ang_tmp, ang_tmp2);
            float new_pred_var[state_size][state_size];
            matrix_multi(state_size, state_size, state_size, state_size, ang_tmp2, pred_var, new_pred_var);
            for (j = 0; j < state_size; j++) {
                for (k = 0; k < state_size; k++) {
                    pred_var[j][k] = new_pred_var[j][k];
                }
            }
        }
    }
}

void calculatePredictedState(size_t state_size, float old_state[state_size][1], float displacement[3][1],
                             float pred_state[state_size][1]) {
    int i, j = 0;
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

void calculatePredictedVar(size_t state_size, float displacement[3][1], float var[state_size][state_size], float pred_var[state_size][state_size]) {
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

int main() {
    float state[5][1] = {{1},
                         {2},
                         {0.5},
                         {20},
                         {30}};
    int state_size = sizeof(state) / sizeof(float);
    float var[5][5] = {{1, 2, 1, 1, 3},
                       {2, 5, 1, 0, 5},
                       {1, 2, 0, 1, 2},
                       {1, 2, 4, 5, 3},
                       {1, 0, 0, 2, 1}};
    float displacement[3][1] = {{5},
                                {20},
                                {0.62f}};
    struct Observations landmark1 = {.land_dist = 40, .land_ang =30, .color="red"};
    struct Landmarks land_list;
    land_list.size = 0;
    land_list.item[0] = landmark1;
    land_list.size++;
    int seen_len;
    struct Seen_Land seen_land;
    struct Seen_Land_List seen_land_list;
    struct Kal_Res results;
    results = kalman_filter(state_size, state, var, displacement, land_list, seen_land_list);

}