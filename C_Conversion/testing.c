#include<stdio.h>
#include "library/linalg.h"
#include <stdlib.h>


int test_row_switch() {
    float matrix[3][3] = {{1.0, 2.0, 5.0},
                          {3.0, 4.0, -1.0},
                          {2.5, 1.0, 1.5}};
    row_operation(matrix[0], matrix[2], 3);
    if (matrix[0][0] == 2.5 && matrix[0][1] == 1.0 && matrix[0][2] == 1.5) {
        if (matrix[2][0] == 1.0 && matrix[2][1] == 2.0 && matrix[2][2] == 5.0) {
            printf("%s", "Row Switch Passed");
            return 0;
        }
    }
    printf("%s", "Row Switch Failed");
    return -1;
}

int test_inverse1() {
    float matrix[3][3] = {{1.0, 2.0, 5.0},
                          {3.0, 4.0, -1.0},
                          {2.0, 1.0, 1.0}};

    gaussian_inverse(3, matrix, matrix);
}

int test_inverse2() {
    float matrix[5][5] = {{1.0, 2.0, 5.0, 3.5, 6},
                          {2.0, 4.0, 2.7, 5.9, 2},
                          {100, 1.0, 1.0, 3.8, 9.2},
                          {1.5, 9.5, 2,   4,   18},
                          {3,   8,   9.7, 10,  2}};

    gaussian_inverse(sizeof(matrix[0]) / sizeof(matrix[0][0]), matrix, matrix);

}

int test_matrix_multi() {
    //to find row of a matrix: (sizeof(a)/sizeof(a[0]))
    //to find column of a matrix: (sizeof(a)/ sizeof(a[0][0]))/row
    float matrixA[1][3] = {{1, 2, 2}};
    float matrixB[3][3] = {{2, 6, 0},
                           {3, 1, 9},
                           {4, 8, 2}};
    int row_A = sizeof(matrixA) / sizeof(matrixA[0]);
    int col_A = sizeof(matrixA) / sizeof(matrixA[0][0]) / row_A;
    int row_B = sizeof(matrixB) / sizeof(matrixB[0]);
    int col_B = sizeof(matrixB) / sizeof(matrixB[0][0]) / row_B;

    float matrixC[row_A][col_B];
    matrix_multi(row_A, col_A, row_B, col_B, matrixA, matrixB, matrixC);
    int i, j = 0;
    for (i = 0; i < row_A; i++) {
        for (j = 0; j < col_B; j++) {
            printf("%f ", matrixC[i][j]);
        }
        printf("\n");
    }
}


int test_transpose() {
    float matrixA[3][3] = {{2, 6, 0},
                           {3, 1, 9},
                           {4, 8, 2}};
    int row = sizeof(matrixA) / sizeof(matrixA[0]);
    int col = sizeof(matrixA) / sizeof(matrixA[0][0]) / row;
    float transposed[col][row];
    transpose(row, col, matrixA, transposed);
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%f ", transposed[i][j]);
        }
        printf("\n");
    }


}


struct Observations {
    float land_dist;
    float land_ang;
    char *color;
};

struct Land { //diff from kalman
    float x;
    float y;
    char *color;
};

struct Landmarks {
    size_t size;
    struct Land item[6];
    struct Observations items[6];
};

struct Seen_Land_List {
    size_t size;
    struct Land item[6];
    //red 0, blue 1, green 2, yellow 3, black 4, pink 5  
};

void find_color(struct Seen_Land_List seen_list, struct Landmarks land_list) {
    int i, j = 0;
    for (i = 0; i < land_list.size; i++) {
        for (j = 0; j < seen_list.size; j++) {
            if (land_list.item[i].color == seen_list.item[j].color) {
                printf("%s", land_list.item[i].color);
            }
        }
    }
}

void test_find_color() {
    struct Land l1, l2;
    l1.x = 1;
    l1.y = 2;
    l1.color = "red";
    l2.x = 2;
    l2.y = 4;
    l2.color = "blue";
    struct Landmarks list1;
    list1.size = 0;
    list1.item[list1.size] = l1;
    list1.size++;
    list1.item[list1.size] = l2;
    list1.size++;

    struct Seen_Land_List seen_list;
    seen_list.size = 0;
    seen_list.item[seen_list.size] = l2;
    seen_list.size++;
    find_color(seen_list, list1);
    //should display blue
}

enum colorcode {
    red, blue, green, yellow, black, pink
};

// void test_enum(){
//     enum colorcode color_num;
//     color_num = str2enum("red"); //doesnt work, can use hash map but lazy to impelment
//     //just use if statements
//     printf("%d", color_num);
// }



void get_exp(float pred_state[5][1], struct Seen_Land_List seen_list) {
    float x_coor, y_coor;
    x_coor = seen_list.item[0].x;
    y_coor = seen_list.item[0].y;
    //obtain expected observation
    float delta[2];//distance of landmark to robot
    delta[0] = x_coor - pred_state[0][0];
    delta[1] = y_coor - pred_state[1][0];
    float q = dotProduct(delta, delta, 2);
    float exp_dis_ang[2]; //z
    exp_dis_ang[0] = sqrt(q);
    exp_dis_ang[1] = atan2(delta[1], delta[0]) - pred_state[2][0];
    printf("q: %f\nexp: %f %f", q, exp_dis_ang[0], exp_dis_ang[1]);
}

void test_get_exp() {
    struct Land l1;
    l1.x = 1;
    l1.y = 2;
    l1.color = "red";
    struct Seen_Land_List seen_list;
    seen_list.size = 0;
    seen_list.item[seen_list.size] = l1;
    float pred_state[5][1] = {{2},
                              {4},
                              {0.39},
                              {3},
                              {4.5}};
    get_exp(pred_state, seen_list);

}


void prediction(size_t state_size, float old_state[state_size][1], float var[state_size][state_size],
                float displacement[3][1],
                struct Landmarks land_list) { //missing R(procc noise) and Q(meas noise) (uncertainty, needs to be defined)
    //only using the first 3 array for x, y, theta, others are for the mappings

    //prediction steps
    //1. Create a N x 3 identity matrix,then multiply the change in x y and thetre to map it into 2N+3 dimensional space
    //2. obtain prediction of x,y,thetre by adding displacements to current state
    //3. Compute matrix Gt
    //4. Compute predicted var matrix with Gt and var
    int i, j, k = 0;
    float iden_mat[state_size][3];
    memset(iden_mat, 0, state_size * 3 * sizeof(int));
    for (i = 0; i < 3; i++) {
        iden_mat[i][i] = 1;
    }
    float displace_vector[state_size][1];
    matrix_multi(state_size, 3, 3, 1, iden_mat, displacement, displace_vector);

    float pred_state[state_size][1]; //predicted state
    add_matrix(state_size, 1, old_state, displace_vector, pred_state);

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
    float pred_var[state_size][state_size];
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

    printf("Pred State\n");
    for (i = 0; i < state_size; i++) {
        printf("%f ", pred_state[i][0]);
    }
    printf("\n");
    printf("Pred Var\n");
    for (i = 0; i < state_size; i++) {
        for (j = 0; j < state_size; j++) {
            printf("%f ", pred_var[i][j]);
        }
        printf("\n");
    }
}

void test_prediction() {
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
                                {0.62}};
    struct Observations landmark1 = {.land_dist = 40, .land_ang =30, .color="red"};
    struct Landmarks land_list;
    land_list.size = 0;
    land_list.items[0] = landmark1;
    land_list.size++;
    prediction(state_size, state, var, displacement, land_list);
}

int main() {
    //test_row_switch();
    //test_inverse1();
    //test_inverse2();
    //test_matrix_multi();
    //test_find_color();
    //test_get_exp();
    test_prediction();


    return 0;
}
