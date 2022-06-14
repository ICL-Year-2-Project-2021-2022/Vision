#include "kalman_test.h"

#define state_size 15

int compareMatrices(size_t row, size_t col, float matrixA[row][col], float matrixB[row][col]) {
    int result = 0;
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (matrixA[i][j] != matrixB[i][j]) {
                result = -1;
            }
        }
    }
    return result;
}

// calculatePredictedState
int dontChangeNextStateForZeroDisplacement() {
    float old_state[state_size][1] = {{1}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    float pred_state[state_size][1] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    float displacement[3][1] = {{0}, {0}, {0}};
    float expected_state[state_size][1] = {{1}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    calculatePredictedState(state_size, old_state, displacement, pred_state);
    return compareMatrices(state_size, 1, pred_state, expected_state);
}

int addValueToEachComponent() {
    float old_state[state_size][1] = {{1}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    float pred_state[state_size][1] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    float displacement[3][1] = {{1}, {2}, {0.5f}};
    float expected_state[state_size][1] = {{2}, {3}, {0.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    calculatePredictedState(state_size, old_state, displacement, pred_state);
    return compareMatrices(state_size, 1, pred_state, expected_state);
}

int addValueAlsoNegativeValueToEachComponent() {
    float old_state[state_size][1] = {{1}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    float pred_state[state_size][1] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    float displacement[3][1] = {{-1}, {2}, {-0.5f}};
    float expected_state[state_size][1] = {{0}, {3}, {-0.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    calculatePredictedState(state_size, old_state, displacement, pred_state);
    return compareMatrices(state_size, 1, pred_state, expected_state);
}

// calculatePredictedVar
int zeroDisplacementToTheVarPredictionInitialState() {
    float old_var[state_size][state_size];
    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            old_var[j][i] = 0;
        }
    }
    float expected_var[state_size][state_size];
    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            expected_var[j][i] = 0;
        }
    }
    float pred_var[state_size][state_size];
    float displacement[3][1] = {{0}, {0}, {0}};
    calculatePredictedVar(state_size, displacement, old_var, pred_var);
    return compareMatrices(state_size, state_size, pred_var, expected_var);
}

int zeroVarPredictionFromInitialStateWithNonZeroDisplacement() {
    float old_var[state_size][state_size];
    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            old_var[j][i] = 0;
        }
    }
    float expected_var[state_size][state_size];
    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            expected_var[j][i] = 0;
        }
    }
    float pred_var[state_size][state_size];
    float displacement[3][1] = {{2}, {1}, {0.5f}};
    calculatePredictedVar(state_size, displacement, old_var, pred_var);
    return compareMatrices(state_size, state_size, pred_var, expected_var);
}

int zeroVarPredictionFromInitialStateWithNegativeDisplacementValues() {
    float old_var[state_size][state_size];
    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            old_var[j][i] = 0;
        }
    }
    float expected_var[state_size][state_size];
    for (int i = 0; i < state_size; i++) {
        for (int j = 0; j < state_size; j++) {
            expected_var[j][i] = 0;
        }
    }
    float pred_var[state_size][state_size];
    float displacement[3][1] = {{-2}, {-1}, {-0.5f}};
    calculatePredictedVar(state_size, displacement, old_var, pred_var);
    return compareMatrices(state_size, state_size, pred_var, expected_var);
}

int main() {
    int testCounter = 0;
    int successTestCounter = 0;

    if (dontChangeNextStateForZeroDisplacement() == 0) {
        printf("dontChangeNextStateForZeroDisplacement - PASS\n");
        successTestCounter++;
    } else {
        printf("dontChangeNextStateForZeroDisplacement - FAIL\n");
    }
    testCounter++;

    if (addValueToEachComponent() == 0) {
        printf("addValueToEachComponent - PASS\n");
        successTestCounter++;
    } else {
        printf("addValueToEachComponent - FAIL\n");
    }
    testCounter++;

    if (addValueAlsoNegativeValueToEachComponent() == 0) {
        printf("addValueAlsoNegativeValueToEachComponent - PASS\n");
        successTestCounter++;
    } else {
        printf("addValueAlsoNegativeValueToEachComponent - FAIL\n");
    }
    testCounter++;

    if (addValueAlsoNegativeValueToEachComponent() == 0) {
        printf("addValueAlsoNegativeValueToEachComponent - PASS\n");
        successTestCounter++;
    } else {
        printf("addValueAlsoNegativeValueToEachComponent - FAIL\n");
    }
    testCounter++;

    if (zeroDisplacementToTheVarPredictionInitialState() == 0) {
        printf("zeroDisplacementToTheVarPredictionInitialState - PASS\n");
        successTestCounter++;
    } else {
        printf("zeroDisplacementToTheVarPredictionInitialState - FAIL\n");
    }
    testCounter++;

    if (zeroVarPredictionFromInitialStateWithNonZeroDisplacement() == 0) {
        printf("zeroVarPredictionFromInitialStateWithNonZeroDisplacement - PASS\n");
        successTestCounter++;
    } else {
        printf("zeroVarPredictionFromInitialStateWithNonZeroDisplacement - FAIL\n");
    }
    testCounter++;

    if (zeroVarPredictionFromInitialStateWithNegativeDisplacementValues() == 0) {
        printf("zeroVarPredictionFromInitialStateWithNegativeDisplacementValues - PASS\n");
        successTestCounter++;
    } else {
        printf("zeroVarPredictionFromInitialStateWithNegativeDisplacementValues - FAIL\n");
    }
    testCounter++;

    printf("Total tests: %d, passed: %d", testCounter, successTestCounter);
}
