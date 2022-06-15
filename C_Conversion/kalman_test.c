#include "kalman_test.h"

#define state_size 15

int compareMatrices(size_t row, size_t col, float matrixA[row][col], float matrixB[row][col]) {
    int result = 0;
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (matrixA[i][j] != matrixB[i][j]) {
                result = -1;
                printf("diff at [i = %d, j = %d] -> A is %f and B is %f\n", i, j, matrixA[i][j], matrixB[i][j]);
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

int setPositionOfTheNeverSeenRedBallFromOriginPosition() {
    float pred_state[state_size][1] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Observations redBall;
    redBall.color = "red";
    redBall.land_dist = 10;
    redBall.land_ang = 0;
    setPositionOfNeverSeenLandmark(state_size, pred_state, seen_land_list, redBall);
    float expected_state[state_size][1] = {{0}, {0}, {0}, {10}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    return compareMatrices(state_size, 1, pred_state, expected_state);
}

int setPositionOfTheNeverSeenRedBallFromX1AndY1Position() {
    float pred_state[15][1] = {{1}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Observations redBall;
    redBall.color = "red";
    redBall.land_dist = 10;
    redBall.land_ang = 0;
    setPositionOfNeverSeenLandmark(state_size, pred_state, seen_land_list, redBall);
    float expected_state[state_size][1] = {{1}, {1}, {0}, {11}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    return compareMatrices(state_size, 1, pred_state, expected_state);
}

int setPositionOfTheNeverSeenRedBallFromX1AndY1PositionWithAngle() {
    float pred_state[15][1] = {{1}, {1}, {0.5}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    struct Seen_Land_List seen_land_list;
    seen_land_list.size = 0;
    struct Observations redBall;
    redBall.color = "red";
    redBall.land_dist = 10;
    redBall.land_ang = -0.25;
    setPositionOfNeverSeenLandmark(state_size, pred_state, seen_land_list, redBall);
    float expected_state[state_size][1] = {{1}, {1}, {0.5}, {10.68912422f}, {3.474039593}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    return compareMatrices(state_size, 1, pred_state, expected_state);
}

int obtainExpectedPositionOfTheLandmarkBasedOnXAndY() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{1}, {1}, {0}, {11}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 11, 1, delta, exp_dis_ang, &q);
    float expected_delta[2] = {10, 0};
    float expected_dis_ang[2] = {10, 0};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 100) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithIncludedAngles() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{2}, {2}, {0.5f}, {11}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 11, 0, delta, exp_dis_ang, &q);
    float expected_delta[2] = {9, -2};
    float expected_dis_ang[2] = {9.21954441f, -0.718668938f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 85) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithResultantNegativeAngle() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{2}, {2}, {0.5f}, {10}, {1}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 10, 1, delta, exp_dis_ang, &q);
    float expected_delta[2] = {8, -1};
    float expected_dis_ang[2] = {8.062257748f, -0.6243549945f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 65) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkNegativeGivingPositiveAngle() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{1}, {1}, {-0.5f}, {8}, {0.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 8, 0.5f, delta, exp_dis_ang, &q);
    float expected_delta[2] = {7, -0.5f};
    float expected_dis_ang[2] = {7.017834424f, 0.42869252f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 49.25f) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkPositiveGivingPositiveAngle() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{1}, {1}, {-0.5f}, {8}, {2.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 8, 2.5f, delta, exp_dis_ang, &q);
    float expected_delta[2] = {7, 1.5f};
    float expected_dis_ang[2] = {7.158910532f, 0.7110933332f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 51.25f) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingPositiveAngle() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{1}, {1}, {0.25f}, {8}, {2.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 8, 3, delta, exp_dis_ang, &q);
    float expected_delta[2] = {7, 2};
    float expected_dis_ang[2] = {7.28010988f, 0.02829965901f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 53) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingNegativeAngle() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{1}, {1}, {0.25f}, {8}, {1.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 8, 1.5f, delta, exp_dis_ang, &q);
    float expected_delta[2] = {7, 0.5f};
    float expected_dis_ang[2] = {7.017834424f, -0.1786925352f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 49.25) ? 0 : -1;
}

int obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndNegativeLandmarkGivingPositiveAngle() {
    float delta[2];
    float exp_dis_ang[2];
    float q;
    float pred_state[state_size][1] = {{1}, {1}, {-0.5f}, {8}, {0.5f}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    obtainExpectedObservation(state_size, pred_state, 8, 0.5f, delta, exp_dis_ang, &q);
    float expected_delta[2] = {7, -0.5f};
    float expected_dis_ang[2] = {7.017834424f, 0.42869252f};
    int matricesComparison = compareMatrices(2, 1, delta, expected_delta) == 0 && compareMatrices(2, 1, exp_dis_ang, expected_dis_ang) == 0 ? 0 : -1;
    return (matricesComparison == 0 && q == 49.25) ? 0 : -1;
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

    if (setPositionOfTheNeverSeenRedBallFromOriginPosition() == 0) {
        printf("setPositionOfTheNeverSeenRedBallFromOriginPosition - PASS\n");
        successTestCounter++;
    } else {
        printf("setPositionOfTheNeverSeenRedBallFromOriginPosition - FAIL\n");
    }
    testCounter++;

    if (setPositionOfTheNeverSeenRedBallFromX1AndY1Position() == 0) {
        printf("setPositionOfTheNeverSeenRedBallFromX1AndY1Position - PASS\n");
        successTestCounter++;
    } else {
        printf("setPositionOfTheNeverSeenRedBallFromX1AndY1Position - FAIL\n");
    }
    testCounter++;

    if (setPositionOfTheNeverSeenRedBallFromX1AndY1PositionWithAngle() == 0) {
        printf("setPositionOfTheNeverSeenRedBallFromX1AndY1PositionWithAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("setPositionOfTheNeverSeenRedBallFromX1AndY1PositionWithAngle - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkBasedOnXAndY() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkBasedOnXAndY - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkBasedOnXAndY - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithIncludedAngles() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithIncludedAngles - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithIncludedAngles - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithResultantNegativeAngle() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithResultantNegativeAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithResultantNegativeAngle - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkNegativeGivingPositiveAngle() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkNegativeGivingPositiveAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkNegativeGivingPositiveAngle - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkPositiveGivingPositiveAngle() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkPositiveGivingPositiveAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndLandmarkPositiveGivingPositiveAngle - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingPositiveAngle() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingPositiveAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingPositiveAngle - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingNegativeAngle() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingNegativeAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndPositiveLandmarkGivingNegativeAngle - FAIL\n");
    }
    testCounter++;

    if (obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndNegativeLandmarkGivingPositiveAngle() == 0) {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndNegativeLandmarkGivingPositiveAngle - PASS\n");
        successTestCounter++;
    } else {
        printf("obtainExpectedPositionOfTheLandmarkWithNegativeRobotAngleAndNegativeLandmarkGivingPositiveAngle - FAIL\n");
    }
    testCounter++;

    printf("Total tests: %d, passed: %d", testCounter, successTestCounter);
}
