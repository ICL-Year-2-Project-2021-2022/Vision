
#include "kalman.h"
#include "float.h"

struct Kal_Res kalman_filter(size_t state_size, float old_state[state_size][1], float var[state_size][state_size],
                             float displacement[3][1], struct Landmarks land_list,
                             struct Seen_Land_List *seen_list , float control_noise[3][3], float measure_noise[2][2]) { //missing R(procc noise) and Q(meas noise) (uncertainty, needs to be defined)
    //only using the first 3 array for x, y, theta, others are for the mappings
    normalise_angles(state_size, old_state, displacement);
    
    struct Kal_Res predictionResult = predictionStep(state_size, old_state, var, displacement, control_noise);
    
    correctionStep(state_size, old_state, predictionResult.new_state, predictionResult.new_var, land_list, seen_list, measure_noise);
     
    return predictionResult;
}

void normalise_angles(size_t state_size, float old_state[state_size][1],float displacement[3][1]){
    while(displacement[2][0]>M_PI*2){
        displacement[2][0] = displacement[2][0] - M_PI*2;
    }
    while (displacement[2][0]<-M_PI*2)
    {
        displacement[2][0] = displacement[2][0] + M_PI*2;
    }
    if (old_state[2][0]+displacement[2][0]>M_PI*2){
        old_state[2][0] = old_state[2][0] - M_PI*2;
    }
    if (old_state[2][0]+displacement[2][0]<-M_PI*2){
        old_state[2][0] = old_state[2][0] + M_PI*2;
    }
    
}

struct Kal_Res predictionStep(size_t state_size, float old_state[state_size][1], float var[state_size][state_size],
                              float displacement[3][1], float control_noise[3][3]) {
    //prediction steps
    //1. Create a N x 3 identity matrix,then multiply the change in x y and thetre to map it into 2N+3 dimensional space
    //2. obtain prediction of x,y,thetre by adding displacements to current state
    //3. Compute matrix Gt
    //4. Compute predicted var matrix with Gt and var
    struct Kal_Res result;
    //state_size = 15; //assume 6 aliens
    calculatePredictedState(state_size, old_state, displacement, result.new_state);
    calculatePredictedVar(state_size, displacement, var, result.new_var, control_noise);
    return result;
}

void correctionStep(size_t state_size, float old_state[state_size][1], float pred_state[state_size][1], float pred_var[state_size][state_size],
               struct Landmarks land_list, struct Seen_Land_List *seen_list, float measure_noise[2][2]) {
    // Correction steps in Kalman Filter
    //1. Identify i-th measurement at time t observes th4e landmark with index j (c =j)
    //2. Initialize landmark if unobserved
    //3. Compute the expected observation
    //4. Comute the jacobian of j
    //5. Compute kalman gain
    //6. Sub Kalman gain to obtain new state and variance matrix
    int i, j, k;
    for (i = 0; i < land_list.size; i++) {
        printf("\nLand %d\n", i);
        if (land_list.item[i].land_dist>2000){
            continue;
        }
        bool seen = false;
        int seenLandmarkIndex = 0;
        for (j = 0; j < seen_list->size; j++) {
            if (strcmp(land_list.item[i].color, seen_list->item[j].color) == 0) {
                printf("Happens here %d\n", j);
                seen = true;
                seenLandmarkIndex = j;
            }
        }
        float delta[2], exp_dis_ang[2], q;

        int color_num = get_colornum(land_list.item[i].color);
        // if the landmark was not observed, then we calculate the position of it
        if (!seen) {
            //size_t state_size, float pred_state[state_size][1], struct Seen_Land_List seen_list, struct Observations observation, int itemIndex
            setPositionOfNeverSeenLandmark(state_size, pred_state, seen_list, land_list.item[i]);
            obtainExpectedObservation(state_size, pred_state, pred_state[2*color_num+3][0], pred_state[2*color_num+4][0], delta, exp_dis_ang, &q);
        } else {
            //obtain expected observation
            
            obtainExpectedObservation(state_size, pred_state, old_state[2*color_num+3][0],old_state[2*color_num+4][0] , delta, exp_dis_ang, &q);
        }
    
        float jacobian[2][state_size];
        computeJacobianHMatrix(state_size, jacobian, color_num, q, delta);
        float kalman_gain[state_size][2];
        computeKalmanGain(state_size, pred_var, jacobian, kalman_gain, measure_noise);
        //printf("\n land list %f %f\n",land_list.item[seenLandmarkIndex].land_dist,
                                        //land_list.item[seenLandmarkIndex].land_ang);
        if (seen){
            getPredictedStateFromKalmanGain(state_size, land_list.item[i].land_dist,
                                        land_list.item[i].land_ang, kalman_gain, exp_dis_ang,
                                        pred_state);
        }
        getPredictedVarFromKalmanGain(state_size, kalman_gain, jacobian, pred_var);

    }
}

void calculatePredictedState(size_t state_size, float old_state[state_size][1], float displacement[3][1],
                             float pred_state[state_size][1]) {
    int i = 0;
    //state_size = 15; //assume 6 aliens
    float iden_mat[state_size][3];
    memset(iden_mat, 0, state_size * 3 * sizeof(float));
    for (i = 0; i < 3; i++) {
        iden_mat[i][i] = 1;
    }
    float displace_vector[state_size][1];
    matrix_multi(state_size, 3, 3, 1, iden_mat, displacement, displace_vector);

    add_matrix(state_size, 1, old_state, displace_vector, pred_state);
    printf("Predicted State\n");
    for (i=0; i<state_size; i++){
        printf("%f ", pred_state[i][0]);
    }
    printf("\n");
}

void calculatePredictedVar(size_t state_size, float displacement[3][1], float var[state_size][state_size],
                           float pred_var[state_size][state_size], float control_noise[3][3]) {
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
    float pre_varvm[3][state_size - 3];
    matrix_multi(3, 3, 3, state_size - 3, jacob3x3, varxm, pre_varvm);
    float pre_varvm_trans[state_size - 3][3];
    transpose(3, state_size - 3, pre_varvm, pre_varvm_trans);

// save memory by not computing state_size x state_size x state_size
    for (i = 0; i < state_size; i++) {
        for (j = 0; j < state_size; j++) {
            if (i < 3 && j < 3) {
                pred_var[i][j] = pre_varxx[i][j]+ control_noise[i][j]; //don't need to scal it up by just adding noise directly
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

void setPositionOfNeverSeenLandmark(size_t state_size, float pred_state[state_size][1], struct Seen_Land_List *seen_list,
                                    struct Observations observation) {
    float x_coor =
            pred_state[0][0] + observation.land_dist * cos(observation.land_ang + pred_state[2][0]);
    float y_coor =
            pred_state[1][0] + observation.land_dist * sin(observation.land_ang + pred_state[2][0]);
//suppose we map a specific color into a specified location in matrix
    struct Seen_Land new_land;
    new_land.color = observation.color;
    seen_list->item[seen_list->size] = new_land;
    seen_list->size+=1;
    int color_num = 10;
    color_num = get_colornum(new_land.color);

    pred_state[2 * color_num + 3][0] = x_coor;
    pred_state[2 * color_num + 4][0] = y_coor;
}

int get_colornum(char *color){
    int color_num = 10;
    if (strcmp(color, "red") == 0) {
        color_num = 0;
    } else if (strcmp(color, "blue") == 0) {
        color_num = 1;
    } else if (strcmp(color, "green") == 0) {
        color_num = 2;
    } else if (strcmp(color, "dark_green") == 0) {
        color_num = 3;
    } else if (strcmp(color, "pink") == 0) {
        color_num = 4;
    } else if (strcmp(color, "yellow") == 0) {
        color_num = 5;
    }
    if (color_num == 10) {
        fprintf(stderr, "Wrong Color Code\n");
    }
    return color_num;
}

void obtainExpectedObservation(size_t state_size, float pred_state[state_size][1], float x_coor, float y_coor,
                               float delta[2], float exp_dis_ang[2], float *q) {
    delta[0] = x_coor - pred_state[0][0];
    delta[1] = y_coor - pred_state[1][0];
    *q = dotProduct(delta, delta, 2); 
    exp_dis_ang[0] = sqrt(*q);
    float angleDueToDifference = atan2(delta[1], delta[0]);
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
    printf("F_matrix\n");
    for (int i=0; i<5; i++){
        for (j=0; j<state_size; j++){
            printf("%f ", F_matrix[i][j]);
        }
        printf("\n");
    }
    //should optimise this line, too many divisions.
    float jacobian_low[2][5] = {{-delta[0] / sqrt(q), -delta[1] / sqrt(q), 0,  delta[0] / sqrt(q),
                                                                                              delta[1] /
                                                                                              sqrt(q)},
                                {delta[1] / q,        -delta[0] / q,       -1, -delta[1] / q, delta[0] / q}};
    matrix_multi(2, 5, 5, state_size, jacobian_low, F_matrix, jacobian);
    printf("Jacobian\n");
    for (int i=0; i<2; i++){
        for (j=0; j<state_size; j++){
            printf("%f ", jacobian[i][j]);
        }
        printf("\n");
    }
}

//verfied
void computeKalmanGain(size_t state_size, float pred_var[state_size][state_size], float jacobian[2][state_size],
                       float kalman_gain[state_size][2], float measure_noise[2][2]) {
    float trans_jacobian[state_size][2];
    //Compute Kalman Gain
    
    transpose(2, state_size, jacobian, trans_jacobian);
    
    float k_tmp1[2][state_size]; 
    matrix_multi(2, state_size, state_size, state_size, jacobian, pred_var, k_tmp1);
    float k_tmp2[2][2];
    matrix_multi(2, state_size, state_size, 2, k_tmp1, trans_jacobian, k_tmp2);
    float k_tmp5[2][2];
    float k_tmp3[2][2];
    add_matrix(2,2,k_tmp2, measure_noise, k_tmp5);
    gaussian_inverse(2, k_tmp5, k_tmp3);
    float k_tmp4[state_size][2];
    matrix_multi(state_size, state_size, state_size, 2, pred_var, trans_jacobian, k_tmp4);
    
    //final kalman gain computed
    matrix_multi(state_size, 2, 2, 2, k_tmp4, k_tmp3, kalman_gain);
    
}

void
getPredictedStateFromKalmanGain(size_t state_size, float land_dist, float land_ang, float kalman_gain[state_size][2],
                                float exp_dis_ang[2], float pred_state[state_size][1]) {
    float z_diff[2][1];
    z_diff[0][0] = land_dist - exp_dis_ang[0];
    z_diff[1][0] = land_ang - exp_dis_ang[1];

    float z_tmp[state_size][1];
    matrix_multi(state_size, 2, 2, 1, kalman_gain, z_diff, z_tmp);
    add_matrix(state_size, 1, pred_state, z_tmp, pred_state);
    int i = 0;
    printf("New Pred State:\n ");
    for (i = 0; i < state_size; i++) {
        printf("%f ", pred_state[i][0]);
    }
    printf("\n");

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
    printf("New Pred Var:\n");
    for (int i=0; i<state_size; i++){
        for (int j=0; j<state_size; j++){
            printf("%f ", pred_var[i][j]);
        }
        printf("\n");
    }
}

int test_combined_3Steps_2Balls() { //to test this, neee
    float control_noise[3][3] = {{100, 0, 0,},{0,100,0},{0,0,0.005}};
    float measure_noise[2][2] = {{250000,0},{0, 0.007}}; //around 50cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.03, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
    float displacement1[3][1] = {{0},
                                {0},
                                {0}}; 
    struct Observations landmark1 = {.land_dist = 1920, .land_ang =-0.464, .color="red"};//should be 1677
    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {-0.4636}};
    struct Observations landmark2 = {.land_dist = 1239, .land_ang = 0.01, .color="red"}; //should be 1453, 0
    struct Observations landmark3 = {.land_dist = 2300, .land_ang = -0.60, .color="blue"}; //shoudl be 2170, -0.6
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark2;
    land_list2.size++;
    land_list2.item[1] = landmark3;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.4636}};
    struct Observations landmark4 = {.land_dist = 1610, .land_ang = 0.64, .color="red"}; //should be 1250 0.64
    struct Observations landmark5 = {.land_dist = 1909, .land_ang = -0.107, .color="blue"}; //should be 1860, 0.4636
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark4;
    land_list3.size++;
    land_list3.item[1] = landmark5;
    land_list3.size++;

    results = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<750*(1+precision) && results.new_state[3][0]>750*(1-precision) ){
        if (results.new_state[4][0]<1500*(1+precision) && results.new_state[4][0]>1500*(1-precision) ){
            return 0;
        }
    }
    return -1;
}



int test_combined_4Steps_2Balls() { //to test this, neee
    float control_noise[3][3] = {{100, 0, 0,},{0,100,0},{0,0,0.005}};
    float measure_noise[2][2] = {{250000,0},{0, 0.007}}; //around 50cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.03, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {0}}; 
    struct Observations landmark1 = {.land_dist = 1920, .land_ang =-0.464, .color="red"};//should be 1677
    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {-0.4636}};
    struct Observations landmark2 = {.land_dist = 1239, .land_ang = 0.01, .color="red"}; //should be 1453, 0
    struct Observations landmark3 = {.land_dist = 2000, .land_ang = -0.60, .color="blue"}; //shoudl be 2170, -0.6
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark2;
    land_list2.size++;
    land_list2.item[1] = landmark3;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.4636}};
    struct Observations landmark4 = {.land_dist = 1610, .land_ang = 0.64, .color="red"}; //should be 1250 0.64
    struct Observations landmark5 = {.land_dist = 1861, .land_ang = -0.107, .color="blue"}; //should be 1700, 0.4636
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark4;
    land_list3.size++;
    land_list3.item[1] = landmark5;
    land_list3.size++;
    struct Kal_Res results3;
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{100}, {300}, {0.233}};
    struct Observations landmark6 = {.land_dist = 1200, .land_ang = 0.424, .color="red"}; //should be 934 0.423
    struct Observations landmark7 = {.land_dist = 1730, .land_ang = -0.467, .color="blue"}; //should be 1700, 0.4636
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark6;
    land_list4.size++;
    land_list4.item[1] = landmark7;
    land_list4.size++;

    results = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<750*(1+precision) && results.new_state[3][0]>750*(1-precision) ){
        if (results.new_state[4][0]<1500*(1+precision) && results.new_state[4][0]>1500*(1-precision) ){
            return 0;
        }
    }
    return -1;



}



int test_combined_5Steps_2Balls() { //to test this, neee
    float control_noise[3][3] = {{100, 0, 0,},{0,100,0},{0,0,0.005}};
    float measure_noise[2][2] = {{12500,0},{0, 0.007}}; //around 50cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.03, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {-0.785398}}; 
    struct Observations landmark1 = {.land_dist = 1420, .land_ang =0.17467, .color="red"};//should be 1220
    struct Observations landmark2 = {.land_dist = 1729, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {0.32175}};
    struct Observations landmark3 = {.land_dist = 1100, .land_ang = -0.1798, .color="red"}; //should be 1000, 0
    struct Observations landmark4 = {.land_dist = 1503, .land_ang = -1.036, .color="blue"}; //shoudl be 1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] = landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.785448}};
    struct Observations landmark5 = {.land_dist = 800, .land_ang = 0.8442, .color="red"}; //should be 1250 0.64
    struct Observations landmark6 = {.land_dist = 1200, .land_ang = -0.-0.3217, .color="blue"}; //should be 1700, 0.4636
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark5;
    land_list3.size++;
    land_list3.item[1] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{100}, {300}, {0.927}};
    struct Observations landmark7 = {.land_dist = 450, .land_ang = -0.14185, .color="red"}; //should be 761
    struct Observations landmark8 = {.land_dist = 1150, .land_ang = -1.5404, .color="blue"}; //should be 1100
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    land_list4.item[1] = landmark8;
    land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {0}, {-0.549}};
    struct Observations landmark9 = {.land_dist = 435, .land_ang = 0.407148, .color="red"}; //should be 761
    struct Observations landmark10 = {.land_dist = 1052, .land_ang = -0.9914568, .color="blue"}; //should be 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;

    results = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}



int test_combined_5Steps_2Balls_inaccurate() { //to test this, neee
    float control_noise[3][3] = {{100, 0, 0,},{0,100,0},{0,0,0.005}};
    float measure_noise[2][2] = {{200000,0},{0, 0.007}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.03, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {-0.785398}}; 
    struct Observations landmark1 = {.land_dist = 1520, .land_ang =0.17467, .color="red"};//should be 1220
    struct Observations landmark2 = {.land_dist = 1929, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {0.32175}};
    struct Observations landmark3 = {.land_dist = 1300, .land_ang = -0.1798, .color="red"}; // 1000, 0
    struct Observations landmark4 = {.land_dist = 1503, .land_ang = -1.036, .color="blue"}; //1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.785448}};
    struct Observations landmark5 = {.land_dist = 800, .land_ang = 0.8442, .color="red"}; // 761
    struct Observations landmark6 = {.land_dist = 1000, .land_ang = -0.-0.3217, .color="blue"}; // 1100
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark5;
    land_list3.size++;
    land_list3.item[1] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{100}, {300}, {0.927}};
    struct Observations landmark7 = {.land_dist = 550, .land_ang = -0.14185, .color="red"}; // 447
    struct Observations landmark8 = {.land_dist = 1350, .land_ang = -1.5404, .color="blue"}; // 1044
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    land_list4.item[1] = landmark8;
    land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {0}, {-0.549}};
    struct Observations landmark9 = {.land_dist = 325, .land_ang = 0.407148, .color="red"}; // 447
    struct Observations landmark10 = {.land_dist = 822, .land_ang = -0.9914568, .color="blue"}; // 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;

    results = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}


int test_combined_6Steps_2Balls () { //to test this, neee
    float control_noise[3][3] = {{100, 0, 0,},{0,100,0},{0,0,0.005}};
    float measure_noise[2][2] = {{200000,0},{0, 0.007}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.03, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {-0.785398}}; 
    struct Observations landmark1 = {.land_dist = 1520, .land_ang =0.17467, .color="red"};//should be 1220
    struct Observations landmark2 = {.land_dist = 1929, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {0.32175}};
    struct Observations landmark3 = {.land_dist = 1300, .land_ang = -0.1798, .color="red"}; // 1000, 0
    struct Observations landmark4 = {.land_dist = 1503, .land_ang = -1.036, .color="blue"}; //1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.785448}};
    struct Observations landmark5 = {.land_dist = 800, .land_ang = 0.8442, .color="red"}; // 761
    struct Observations landmark6 = {.land_dist = 1000, .land_ang = -0.-0.3217, .color="blue"}; // 1100
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark5;
    land_list3.size++;
    land_list3.item[1] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{100}, {300}, {0.927}};
    struct Observations landmark7 = {.land_dist = 550, .land_ang = -0.14185, .color="red"}; // 447
    struct Observations landmark8 = {.land_dist = 1350, .land_ang = -1.5404, .color="blue"}; // 1044
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    land_list4.item[1] = landmark8;
    land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {0}, {-0.549}};
    struct Observations landmark9 = {.land_dist = 325, .land_ang = 0.407148, .color="red"}; // 447
    struct Observations landmark10 = {.land_dist = 822, .land_ang = -0.9914568, .color="blue"}; // 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;
    struct Kal_Res results5;
    results5 = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);

    float displacement6[3][1] = {{50}, {100}, {0.407}};
    struct Observations landmark11 = {.land_dist = 335, .land_ang = 0.0, .color="red"}; // 447
    struct Observations landmark12 = {.land_dist = 1020, .land_ang = -1.5057, .color="blue"}; // 1100
    struct Landmarks land_list6;
    land_list6.size = 0;
    land_list6.item[0] = landmark11;
    land_list6.size++;
    land_list6.item[1] = landmark12;
    land_list6.size++;

    results = kalman_filter(state_size, results5.new_state, results5.new_var, displacement6, land_list6, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}



int test_combined_7Steps_2Balls () { //to test this, neee
    float control_noise[3][3] = {{100, 0, 0},{100,0,0},{0,0,0}};
    float measure_noise[2][2] = {{200000,0},{0, 0.007}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {-0.785398}}; 
    struct Observations landmark1 = {.land_dist = 1520, .land_ang =0.17467, .color="red"};//should be 1220
    struct Observations landmark2 = {.land_dist = 1929, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {0.32175}};
    struct Observations landmark3 = {.land_dist = 1300, .land_ang = -0.1798, .color="red"}; // 1000, 0
    struct Observations landmark4 = {.land_dist = 1503, .land_ang = -1.036, .color="blue"}; //1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.785448}};
    struct Observations landmark5 = {.land_dist = 800, .land_ang = 0.8442, .color="red"}; // 761
    struct Observations landmark6 = {.land_dist = 1000, .land_ang = -0.-0.3217, .color="blue"}; // 1100
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark5;
    land_list3.size++;
    land_list3.item[1] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{100}, {300}, {0.927}};
    struct Observations landmark7 = {.land_dist = 550, .land_ang = -0.14185, .color="red"}; // 447
    struct Observations landmark8 = {.land_dist = 1350, .land_ang = -1.5404, .color="blue"}; // 1044
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    land_list4.item[1] = landmark8;
    land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {0}, {-0.549}};
    struct Observations landmark9 = {.land_dist = 325, .land_ang = 0.407148, .color="red"}; // 447
    struct Observations landmark10 = {.land_dist = 822, .land_ang = -0.9914568, .color="blue"}; // 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;
    struct Kal_Res results5;
    results5 = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);

    float displacement6[3][1] = {{50}, {100}, {0.407}};
    struct Observations landmark11 = {.land_dist = 335, .land_ang = 0.0, .color="red"}; // 335
    struct Observations landmark12 = {.land_dist = 1020, .land_ang = -1.5057, .color="blue"}; // 1030
    struct Landmarks land_list6;
    land_list6.size = 0;
    land_list6.item[0] = landmark11;
    land_list6.size++;
    land_list6.item[1] = landmark12;
    land_list6.size++;
    struct Kal_Res results6;
    results6 = kalman_filter(state_size, results5.new_state, results5.new_var, displacement6, land_list6, &seen_land_list, control_noise, measure_noise);

    float displacement7[3][1] = {{0}, {0}, {-0.70714}};
    struct Observations landmark13 = {.land_dist = 335, .land_ang = 0.707, .color="red"}; // 335
    struct Observations landmark14 = {.land_dist = 1010, .land_ang = -0.798, .color="blue"}; // 1030
    struct Landmarks land_list7;
    land_list7.size = 0;
    land_list7.item[0] = landmark13;
    land_list7.size++;
    land_list7.item[1] = landmark14;
    land_list7.size++;

    results = kalman_filter(state_size, results6.new_state, results6.new_var, displacement7, land_list7, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}




int test_combined_7Steps_2Balls_FuckedDistance () { //to test this, neee
    float control_noise[3][3] = {{10, 0, 0},{10,0,0},{0,0,0.0005}};
    float measure_noise[2][2] = {{5000000,0},{0, 0.001}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0, 0.002, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {-0.785398}}; 
    struct Observations landmark1 = {.land_dist = 2000, .land_ang =0.17467, .color="red"};//should be 1220
    struct Observations landmark2 = {.land_dist = 2800, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{100}, {200}, {0.32175}};
    struct Observations landmark3 = {.land_dist = 2000, .land_ang = -0.1798, .color="red"}; // 1000, 0
    struct Observations landmark4 = {.land_dist = 1900, .land_ang = -1.036, .color="blue"}; //1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{300}, {100}, {-0.785448}};
    struct Observations landmark5 = {.land_dist = 1400, .land_ang = 0.8442, .color="red"}; // 761
    struct Observations landmark6 = {.land_dist = 1800, .land_ang = -0.3217, .color="blue"}; // 1100
    struct Landmarks land_list3;
    land_list3.size = 0;
    land_list3.item[0] = landmark5;
    land_list3.size++;
    land_list3.item[1] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{100}, {300}, {0.927}};
    struct Observations landmark7 = {.land_dist = 1000, .land_ang = -0.14185, .color="red"}; // 447
    struct Observations landmark8 = {.land_dist = 1500, .land_ang = -1.5404, .color="blue"}; // 1044
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    land_list4.item[1] = landmark8;
    land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {0}, {-0.549}};
    struct Observations landmark9 = {.land_dist = 900, .land_ang = 0.407148, .color="red"}; // 447
    struct Observations landmark10 = {.land_dist = 1800, .land_ang = -0.9914568, .color="blue"}; // 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;
    struct Kal_Res results5;
    results5 = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);

    float displacement6[3][1] = {{50}, {100}, {0.407}};
    struct Observations landmark11 = {.land_dist = 480, .land_ang = 0.0, .color="red"}; // 335
    struct Observations landmark12 = {.land_dist = 1700, .land_ang = -1.5057, .color="blue"}; // 1030
    struct Landmarks land_list6;
    land_list6.size = 0;
    land_list6.item[0] = landmark11;
    land_list6.size++;
    land_list6.item[1] = landmark12;
    land_list6.size++;
    struct Kal_Res results6;
    results6 = kalman_filter(state_size, results5.new_state, results5.new_var, displacement6, land_list6, &seen_land_list, control_noise, measure_noise);

    float displacement7[3][1] = {{0}, {0}, {-0.70714}};
    struct Observations landmark13 = {.land_dist = 400, .land_ang = 0.707, .color="red"}; // 335
    struct Observations landmark14 = {.land_dist = 1510, .land_ang = -0.798, .color="blue"}; // 1030
    struct Landmarks land_list7;
    land_list7.size = 0;
    land_list7.item[0] = landmark13;
    land_list7.size++;
    land_list7.item[1] = landmark14;
    land_list7.size++;

    results = kalman_filter(state_size, results6.new_state, results6.new_var, displacement7, land_list7, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}



int test_combined_8Steps_2Balls_FuckedDistance () { //to test this, neee
    float control_noise[3][3] = {{10, 0, 0},{10,0,0},{0,0,0.0001}};
    float measure_noise[2][2] = {{1000000,0},{0, 0.0001}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.0001, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {-0.785398}}; 
    struct Observations landmark1 = {.land_dist = 1800, .land_ang =0.17467, .color="red"};//should be 1220
    struct Observations landmark2 = {.land_dist = 1990, .land_ang = -0.588, .color="blue"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    land_list1.item[land_list1.size] = landmark2;
    land_list1.size++;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{50}, {50}, {0}};
    struct Observations landmark3 = {.land_dist = 1700, .land_ang = 0.18534795, .color="red"}; // 1000, 0
    struct Observations landmark4 = {.land_dist = 1800, .land_ang = -0.614662952, .color="blue"}; //1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{50}, {0}, {-0.785448}};
    //struct Observations landmark5 = {.land_dist = 1400, .land_ang = 1.007480065, .color="red"}; // 761
    struct Observations landmark6 = {.land_dist = 1800, .land_ang =0.176708856, .color="blue"}; // 1100
    struct Landmarks land_list3;
    land_list3.size = 0;
    // land_list3.item[0] = landmark5;
    // land_list3.size++;
    land_list3.item[land_list3.size] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    printf("\n\n\nthis loop ");
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{50}, {50}, {0.785398163}};
    struct Observations landmark7 = {.land_dist = 1500, .land_ang = 0.236848761, .color="red"}; // 447
    struct Observations landmark8 = {.land_dist = 1700, .land_ang = -0.638319808, .color="blue"}; // 1044
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    land_list4.item[1] = landmark8;
    land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {50}, {0.785398163}};
    struct Observations landmark9 = {.land_dist = 1520, .land_ang = -0.57430483, .color="red"}; // 447
    struct Observations landmark10 = {.land_dist = 1800, .land_ang = -1.460139106, .color="blue"}; // 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;
    struct Kal_Res results5;
    results5 = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);

    float displacement6[3][1] = {{50}, {50}, {-0.785398}};
    struct Observations landmark11 = {.land_dist = 1480, .land_ang = 0.226798848, .color="red"}; // 335
    struct Observations landmark12 = {.land_dist = 1700, .land_ang = -0.708626272, .color="blue"}; // 1030
    struct Landmarks land_list6;
    land_list6.size = 0;
    land_list6.item[0] = landmark11;
    land_list6.size++;
    land_list6.item[1] = landmark12;
    land_list6.size++;
    struct Kal_Res results6;
    results6 = kalman_filter(state_size, results5.new_state, results5.new_var, displacement6, land_list6, &seen_land_list, control_noise, measure_noise);

    float displacement7[3][1] = {{50}, {0}, {-0.785398163}};
    struct Observations landmark13 = {.land_dist = 1300, .land_ang = 1.058406866, .color="red"}; // 335
    struct Observations landmark14 = {.land_dist = 1510, .land_ang = 0.079829986, .color="blue"}; // 1030
    struct Landmarks land_list7;
    land_list7.size = 0;
    land_list7.item[0] = landmark13;
    land_list7.size++;
    land_list7.item[1] = landmark14;
    land_list7.size++;

    results = kalman_filter(state_size, results6.new_state, results6.new_var, displacement7, land_list7, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}




int test_combined_10Steps_2Balls_FuckedDistance () { //to test this, neee
    float control_noise[3][3] = {{10, 0, 0},{10,0,0},{0,0,0.003}};
    float measure_noise[2][2] = {{1000000,0},{0, 0.009}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.002, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {0}}; 
    struct Observations landmark1 = {.land_dist = 1800, .land_ang =-0.610725964, .color="red"};//should be 1220
    //struct Observations landmark2 = {.land_dist = 1990, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    //land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark3 = {.land_dist = 1800, .land_ang = -0.087127189, .color="red"}; // 1000, 0
    struct Observations landmark4 = {.land_dist = 1800, .land_ang = -0.849801991, .color="blue"}; //1403
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark5 = {.land_dist = 1600, .land_ang = 0.436471587, .color="red"}; // 761
    struct Observations landmark6 = {.land_dist = 2000, .land_ang =-0.326203216, .color="blue"}; // 1100
    struct Landmarks land_list3;
    land_list3.size = 0;
    // land_list3.item[0] = landmark5;
    // land_list3.size++;
    land_list3.item[land_list3.size] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    printf("\n\n\nthis loop ");
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{0}, {100}, {1.047197551}};
    struct Observations landmark7 = {.land_dist = 1500, .land_ang = -0.661043169, .color="red"}; // 447
    //struct Observations landmark8 = {.land_dist = 1700, .land_ang = -0.638319808, .color="blue"}; // 1044
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    // land_list4.item[1] = landmark8;
    // land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {100}, {-1.570796327}};
    struct Observations landmark9 = {.land_dist = 1305, .land_ang = 0.851966327, .color="red"}; // 447
    struct Observations landmark10 = {.land_dist = 1800, .land_ang = 0.066568164, .color="blue"}; // 1100
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;
    struct Kal_Res results5;
    results5 = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);

    float displacement6[3][1] = {{0}, {100}, {1.570796327}};
    struct Observations landmark11 = {.land_dist = 1480, .land_ang = -0.785398163, .color="red"}; // 335
    //struct Observations landmark12 = {.land_dist = 1700, .land_ang = -0.708626272, .color="blue"}; // 1030
    struct Landmarks land_list6;
    land_list6.size = 0;
    land_list6.item[0] = landmark11;
    land_list6.size++;
    // land_list6.item[1] = landmark12;
    // land_list6.size++;
    struct Kal_Res results6;
    results6 = kalman_filter(state_size, results5.new_state, results5.new_var, displacement6, land_list6, &seen_land_list, control_noise, measure_noise);

    float displacement7[3][1] = {{0}, {100}, {0}};
    struct Observations landmark13 = {.land_dist = 1250, .land_ang = -0.862170055, .color="red"}; // 335
    //struct Observations landmark14 = {.land_dist = 1510, .land_ang = 0.079829986, .color="blue"}; // 1030
    struct Landmarks land_list7;
    land_list7.size = 0;
    land_list7.item[0] = landmark13;
    land_list7.size++;
    // land_list7.item[1] = landmark14;
    // land_list7.size++;
    struct Kal_Res results7;
    results7 = kalman_filter(state_size, results6.new_state, results6.new_var, displacement7, land_list7, &seen_land_list, control_noise, measure_noise);

    float displacement8[3][1] = {{0}, {100}, {0}};
    struct Observations landmark15 = {.land_dist = 1200, .land_ang = -0.950546841, .color="red"}; // 335
    //struct Observations landmark16 = {.land_dist = 1510, .land_ang = 0.079829986, .color="blue"}; // 1030
    struct Landmarks land_list8;
    land_list8.size = 0;
    land_list8.item[0] = landmark15;
    // land_list8.size++;
    // land_list8.item[1] = landmark16;
    land_list8.size++;
    struct Kal_Res results8;
    results8 = kalman_filter(state_size, results7.new_state, results7.new_var, displacement8, land_list8, &seen_land_list, control_noise, measure_noise);

  float displacement9[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark17 = {.land_dist = 1100, .land_ang = -0.426948065, .color="red"}; // 335
    struct Observations landmark18 = {.land_dist = 1510, .land_ang = -1.179749083, .color="blue"}; // 1030
    struct Landmarks land_list9;
    land_list9.size = 0;
    land_list9.item[0] = landmark17;
    land_list9.size++;
    land_list9.item[1] = landmark18;
    land_list9.size++;
    struct Kal_Res results9;
    results9 = kalman_filter(state_size, results8.new_state, results8.new_var, displacement9, land_list9, &seen_land_list, control_noise, measure_noise);

  float displacement10[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark19 = {.land_dist = 1100, .land_ang = 0.09665071, .color="red"}; // 335
    struct Observations landmark20 = {.land_dist = 1610, .land_ang = -0.656150308, .color="blue"}; // 1030
    struct Landmarks land_list10;
    land_list10.size = 0;
    land_list10.item[0] = landmark19;
    land_list10.size++;
    land_list10.item[1] = landmark20;
    land_list10.size++;

    results = kalman_filter(state_size, results9.new_state, results9.new_var, displacement10, land_list10, &seen_land_list, control_noise, measure_noise);


    float precision= 0.2f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
           
           
                    return 0;
                }
            }
        }
    }
    return -1;
}



int test_combined_12Steps_2Balls_FuckedDistance () { //to test this, neee
    float control_noise[3][3] = {{50, 0, 0},{50,0,0},{0,0,0.01}};
    float measure_noise[2][2] = {{1000000,0},{0, 0.001}}; //around 450cm 5 degree error
    float state[7][1] = {{0},
                         {0},
                         {M_PI_2},
                         {0},
                         {0},
                         {0},
                         {0}};

    int state_size = sizeof(state) / sizeof(state[0]);
    float var[7][7] = {{10, 0, 0, 0, 0 , 0, 0},
                       {0, 10, 0, 0, 0 , 0, 0},
                       {0, 0, 0.002, 0, 0 ,0 ,0},
                       {0, 0, 0, 10000000, 0 ,0,0},
                       {0, 0, 0, 0, 10000000 ,0,0},
                       {0,0 , 0, 0, 0, 10000000 , 0},
                       {0,0, 0,0,0,0,10000000}};
                        
   float displacement1[3][1] = {{0},
                                {0},
                                {0}}; 
    struct Observations landmark1 = {.land_dist = 1800, .land_ang =-0.610725964, .color="red"};//should be 1220
    //struct Observations landmark2 = {.land_dist = 1990, .land_ang = -0.588, .color="red"}; //should be 1529

    struct Landmarks land_list1;
    land_list1.size = 0;
    land_list1.item[0] = landmark1;
    land_list1.size++;
    //land_list1.item[land_list1.size] = landmark2;
    int seen_len;
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Kal_Res results, results1;
    results1 = kalman_filter(state_size, state, var, displacement1, land_list1, &seen_land_list, control_noise, measure_noise);


    float displacement2[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark3 = {.land_dist = 2000, .land_ang = -0.087127189, .color="red"}; // 1220
    struct Observations landmark4 = {.land_dist = 1800, .land_ang = -0.849801991, .color="blue"}; //1529
    struct Landmarks land_list2;
    land_list2.size = 0;
    land_list2.item[0] = landmark3;
    land_list2.size++;
    land_list2.item[1] =  landmark4;
    land_list2.size++;
    struct Kal_Res results2;
    results2 = kalman_filter(state_size, results1.new_state, results1.new_var, displacement2, land_list2, &seen_land_list, control_noise, measure_noise);

    float displacement3[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark5 = {.land_dist = 1600, .land_ang = 0.436471587, .color="red"}; // 1220
    struct Observations landmark6 = {.land_dist = 2000, .land_ang =-0.326203216, .color="blue"}; // 1529
    struct Landmarks land_list3;
    land_list3.size = 0;
    // land_list3.item[0] = landmark5;
    // land_list3.size++;
    land_list3.item[land_list3.size] = landmark6;
    land_list3.size++;
    struct Kal_Res results3;
    printf("\n\n\nthis loop ");
    results3 = kalman_filter(state_size, results2.new_state, results2.new_var, displacement3, land_list3, &seen_land_list, control_noise, measure_noise);

    float displacement4[3][1] = {{0}, {100}, {1.047197551}};
    struct Observations landmark7 = {.land_dist = 1500, .land_ang = -0.661043169, .color="red"}; // 1140
    //struct Observations landmark8 = {.land_dist = 1700, .land_ang = -0.638319808, .color="blue"}; // 1513
    struct Landmarks land_list4;
    land_list4.size = 0;
    land_list4.item[0] = landmark7;
    land_list4.size++;
    // land_list4.item[1] = landmark8;
    // land_list4.size++;
    struct Kal_Res results4;
    results4 = kalman_filter(state_size, results3.new_state, results3.new_var, displacement4, land_list4, &seen_land_list, control_noise, measure_noise);


    float displacement5[3][1] = {{0}, {100}, {-1.570796327}};
    struct Observations landmark9 = {.land_dist = 1305, .land_ang = 0.851966327, .color="red"}; // 1063
    struct Observations landmark10 = {.land_dist = 1800, .land_ang = 0.066568164, .color="blue"}; // 1503
    struct Landmarks land_list5;
    land_list5.size = 0;
    land_list5.item[0] = landmark9;
    land_list5.size++;
    land_list5.item[1] = landmark10;
    land_list5.size++;
    struct Kal_Res results5;
    results5 = kalman_filter(state_size, results4.new_state, results4.new_var, displacement5, land_list5, &seen_land_list, control_noise, measure_noise);

    float displacement6[3][1] = {{0}, {100}, {1.570796327}};
    struct Observations landmark11 = {.land_dist = 1480, .land_ang = -0.785398163, .color="red"}; // 990
    //struct Observations landmark12 = {.land_dist = 1700, .land_ang = -0.708626272, .color="blue"}; // 1500
    struct Landmarks land_list6;
    land_list6.size = 0;
    land_list6.item[0] = landmark11;
    land_list6.size++;
    // land_list6.item[1] = landmark12;
    // land_list6.size++;
    struct Kal_Res results6;
    results6 = kalman_filter(state_size, results5.new_state, results5.new_var, displacement6, land_list6, &seen_land_list, control_noise, measure_noise);

    float displacement7[3][1] = {{0}, {100}, {0}};
    struct Observations landmark13 = {.land_dist = 1250, .land_ang = -0.862170055, .color="red"}; // 921
    //struct Observations landmark14 = {.land_dist = 1510, .land_ang = 0.079829986, .color="blue"}; // 1503
    struct Landmarks land_list7;
    land_list7.size = 0;
    land_list7.item[0] = landmark13;
    land_list7.size++;
    // land_list7.item[1] = landmark14;
    // land_list7.size++;
    struct Kal_Res results7;
    results7 = kalman_filter(state_size, results6.new_state, results6.new_var, displacement7, land_list7, &seen_land_list, control_noise, measure_noise);

    float displacement8[3][1] = {{0}, {100}, {0}};
    struct Observations landmark15 = {.land_dist = 1200, .land_ang = -0.950546841, .color="red"}; // 860
    //struct Observations landmark16 = {.land_dist = 1510, .land_ang = 0.079829986, .color="blue"}; // 1513
    struct Landmarks land_list8;
    land_list8.size = 0;
    land_list8.item[0] = landmark15;
    // land_list8.size++;
    // land_list8.item[1] = landmark16;
    land_list8.size++;
    struct Kal_Res results8;
    results8 = kalman_filter(state_size, results7.new_state, results7.new_var, displacement8, land_list8, &seen_land_list, control_noise, measure_noise);

  float displacement9[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark17 = {.land_dist = 1100, .land_ang = -0.426948065, .color="red"}; // 860
    struct Observations landmark18 = {.land_dist = 1410, .land_ang = -1.179749083, .color="blue"}; // 1513
    struct Landmarks land_list9;
    land_list9.size = 0;
    land_list9.item[0] = landmark17;
    land_list9.size++;
    land_list9.item[1] = landmark18;
    land_list9.size++;
    struct Kal_Res results9;
    results9 = kalman_filter(state_size, results8.new_state, results8.new_var, displacement9, land_list9, &seen_land_list, control_noise, measure_noise);

    float displacement10[3][1] = {{0}, {0}, {-0.523598776}};
    struct Observations landmark19 = {.land_dist = 1100, .land_ang = 0.09665071, .color="red"}; // 860
    struct Observations landmark20 = {.land_dist = 1210, .land_ang = -0.656150308, .color="blue"}; // 1513
    struct Landmarks land_list10;
    land_list10.size = 0;
    land_list10.item[0] = landmark19;
    land_list10.size++;
    land_list10.item[1] = landmark20;
    land_list10.size++;
    struct Kal_Res results10;
    results10 = kalman_filter(state_size, results9.new_state, results9.new_var, displacement10, land_list10, &seen_land_list, control_noise, measure_noise);

    float displacement11[3][1] = {{500}, {0}, {-0.523598776}};
    //struct Observations landmark21 = {.land_dist = 700, .land_ang = 0.09665071, .color="red"}; // 538
    struct Observations landmark22 = {.land_dist = 1300, .land_ang = -0.19739556, .color="blue"}; // 1019
    struct Landmarks land_list11;
    land_list11.size = 0;
    // land_list11.item[0] = landmark21;
    // land_list11.size++;
    land_list11.item[land_list11.size] = landmark22;
    land_list11.size++;
    struct Kal_Res results11;
    results11 = kalman_filter(state_size, results10.new_state, results10.new_var, displacement11, land_list11, &seen_land_list, control_noise, measure_noise);

    float displacement12[3][1] = {{0}, {0}, {1.047197551}};
    struct Observations landmark23 = {.land_dist = 500, .land_ang = 0.143092398, .color="red"}; // 538
    //struct Observations landmark24 = {.land_dist = 1610, .land_ang = -0.656150308, .color="blue"}; // 1019
    struct Landmarks land_list12;
    land_list12.size = 0;
    land_list12.item[0] = landmark23;
    land_list12.size++;
    // land_list12.item[1] = landmark24;
    // land_list12.size++;

    results = kalman_filter(state_size, results11.new_state, results11.new_var, displacement12, land_list12, &seen_land_list, control_noise, measure_noise);


    float precision= 0.25f; //accept 20% error
    //change into measuring landmark accuracy
    if (results.new_state[3][0]<700*(1+precision) && results.new_state[3][0]>700*(1-precision) ){
        if (results.new_state[4][0]<1000*(1+precision) && results.new_state[4][0]>1000*(1-precision) ){
            if (results.new_state[5][0]<1500*(1+precision) && results.new_state[5][0]>1500*(1-precision) ){
                if (results.new_state[6][0]<300*(1+precision) && results.new_state[6][0]>300*(1-precision) ){
                    return 0;
                }
            }
        }
    }
    return -1;
}

int main(){
    //testVariance();
    //test_combined1();
    int testCounter = 0;
    int successTestCounter = 0;

    /*if (test_combined_3Steps_2Balls() == 0) {
        printf("test_combined_3Steps_2Balls - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_3Steps_2Balls - FAIL\n");
    }
    testCounter++;
    if (test_combined_4Steps_2Balls() == 0) {
        printf("test_combined_4Steps_2Balls - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_4Steps_2Balls - FAIL\n");
    }
    testCounter++;

     if (test_combined_5Steps_2Balls() == 0) {
        printf("test_combined_5Steps_2Balls - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_5Steps_2Balls - FAIL\n");
    }
    testCounter++;

    if (test_combined_5Steps_2Balls_inaccurate() == 0) {
        printf("test_combined_5Steps_2Balls_inaccurate - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_5Steps_2Balls_inaccurate - FAIL\n");
    }
    testCounter++;


    if (test_combined_6Steps_2Balls() == 0) {
        printf("test_combined_6Steps_2Balls - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_6Steps_2Balls - FAIL\n");
    }
    testCounter++; 


    if (test_combined_7Steps_2Balls() == 0) {
        printf("test_combined_7Steps_2Balls - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_7Steps_2Balls - FAIL\n");
    }
    testCounter++;


    if (test_combined_7Steps_2Balls_FuckedDistance() == 0) {
        printf("test_combined_7Steps_2Balls_FuckedDistance - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_7Steps_2Balls - FAIL\n");
    }
    testCounter++;

    if (test_combined_8Steps_2Balls_FuckedDistance() == 0) {
        printf("test_combined_8Steps_2Balls_FuckedDistance - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_8Steps_2Balls - FAIL\n");
    }
    testCounter++;*/


    // if (test_combined_10Steps_2Balls_FuckedDistance() == 0) {
    //     printf("test_combined_10Steps_2Balls_FuckedDistance - PASS\n");
    //     successTestCounter++;
    // } else {
    //     printf("test_combined_10Steps_2Balls - FAIL\n");
    // }
    // testCounter++;

    if (test_combined_12Steps_2Balls_FuckedDistance() == 0) {
        printf("test_combined_12Steps_2Balls_FuckedDistance - PASS\n");
        successTestCounter++;
    } else {
        printf("test_combined_12Steps_2Balls - FAIL\n");
    }
    testCounter++;
    
    
    printf("Total tests: %d, passed: %d", testCounter, successTestCounter);
}
