#include "kalman.h"

int main() {
    float state[5][1] = {{1},
                         {2},
                         {0.5},
                         {16},
                         {61}};
    int state_size = sizeof(state) / sizeof(float);
    float var[5][5] = {{1, 2, 1, 1, 3},
                       {2, 5, 1, 0, 5},
                       {1, 2, 0, 1, 2},
                       {1, 2, 4, 5, 3},
                       {1, 0, 0, 2, 1}};
    float displacement[3][1] = {{5},
                                {20},
                                {0.62}};
    struct Observations landmark1 = {.land_dist = 40, .land_ang =0.2, .color="red"};
    struct Landmarks land_list;
    land_list.size = 0;
    land_list.item[0] = landmark1;
    land_list.size++;
    int seen_len;
    struct Seen_Land seen_land1 = {.x_coor = 16, .y_coor = 61, .color = "red"};
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    seen_land_list.item[seen_land_list.size] = seen_land1;
    seen_land_list.size++;
    struct Kal_Res results;
    results = kalman_filter(state_size, state, var, displacement, land_list, seen_land_list);
}