#ifndef KALMAN_KALMAN_TEST_H
#define KALMAN_KALMAN_TEST_H

#include "kalman.h"

int compareMatrices(size_t row, size_t col, float matrixA[row][col], float matrixB[row][col]);

int dontChangeNextStateForZeroDisplacement();

#endif //KALMAN_KALMAN_TEST_H
