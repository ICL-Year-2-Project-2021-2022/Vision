#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// priority Node
typedef struct node {
    int x_coor;
    int y_coor;
    float distance_start;
    float priority; //lower priority, first served
    struct node *parent;
    struct node *next;
} Node;

Node *newNode(int x_coor, int y_coor, float distance_start, float priority, struct node *parent) {
    Node *temp = (Node *) malloc(sizeof(Node));
    temp->x_coor = x_coor;
    temp->y_coor = y_coor;
    temp->distance_start = distance_start;
    temp->priority = priority;
    temp->parent = parent;
    temp->next = NULL;
    return temp;
}

Node *copyNode(Node *old) {
    Node *temp = (Node *) malloc(sizeof(Node));
    temp->x_coor = old->x_coor;
    temp->y_coor = old->y_coor;
    temp->distance_start = old->distance_start;
    temp->priority = old->priority;
    temp->parent = old->parent;
    temp->next = NULL;
    return temp;

}

float peek_x(Node **head) {
    return (*head)->x_coor;
}


float peek_y(Node **head) {
    return (*head)->y_coor;
}

float peek_priority(Node **head) {
    return (*head)->priority;
}

Node *popGet(Node **head) {
    Node *temp = *head;
    (*head) = (*head)->next;
    return temp;
}


void push(Node **head, float x_coor, float y_coor, float distance_start, float priority, struct node *parent) {
    Node *start = (*head);
    Node *temp = newNode(x_coor, y_coor, distance_start, priority, parent);
    if ((*head)->priority > priority) {
        temp->next = *head;
        (*head) = temp;
    } else {
        while (start->next != NULL && start->next->priority < priority) {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
}

void pushNode(Node **head, Node *new_node) {
    Node *start = (*head);
    if (start == NULL) {
        start = new_node;
    } else {
        if ((*head)->priority > new_node->priority) {
            new_node->next = *head;
            (*head) = new_node;
        } else {
            while (start->next != NULL && start->next->priority < new_node->priority) {
                start = start->next;
            }
            new_node->next = start->next;
            start->next = new_node;
        }
    }

}

// Function to check the queue is empty
int isEmpty(Node **head) {
    return (*head) == NULL;
}


int distanceToGoal(int x_current, int y_current, int x_goal, int y_goal) {
    int x_diff = abs(x_goal - x_current);
    int y_diff = abs(y_goal - y_current);
    if (x_diff > y_diff) {
        return y_diff * 14 + (x_diff - y_diff) * 10;
    } else {
        return x_diff * 14 + (y_diff - x_diff) * 10;
    }
}

int isReachedObj(int x_current, int y_current, int x_goal, int y_goal) {
    return (x_current == x_goal) && (y_current == y_goal);
}

int isInClose(Node **head, Node *currentNode) {
    Node *start = (*head);
    if (start == NULL) {
        return 0;
    }

    while (start != NULL) {
        if (start->x_coor == currentNode->x_coor && start->y_coor == currentNode->y_coor) {
            return 1;
        }

        start = start->next;
    }
    return 0;
}

int isInOpen(Node **head, Node *currentNode) {
    Node *start = (*head);
    if (start == NULL) {
        return 0;
    }
    while (start != NULL) {
        if (start->x_coor == currentNode->x_coor && start->y_coor == currentNode->y_coor) {

            return 1;
        }
        start = start->next;
    }
    return 0;
}

float getDistanceStart(Node **head, Node *currentNode) {
    Node *start = (*head);
    while (start != NULL) {
        if (start->x_coor == currentNode->x_coor && start->y_coor == currentNode->y_coor) {
            return start->distance_start;
        }
        start = start->next;
    }
    return 1000;
}

void changeCostParent(Node **head, Node *neighbourNode, Node *currentNode) {
    Node *start = (*head);
    while (start != NULL) {
        if (start->x_coor == neighbourNode->x_coor && start->y_coor == neighbourNode->y_coor) {
            start->priority = neighbourNode->priority;
            start->parent = currentNode;
        }
        start = start->next;
    }
}

void printPathToGoal(Node *currentNode) {
    Node *start = (currentNode);
    while (start != NULL) {
        start = start->parent;
    }
}

void printQueue(Node **head) {
    Node *start = (*head);
    while (start != NULL) {
        start = start->next;
    }
}


//IMPORTANT: Keep in mind that Distance to Start and goal is according to path, not euclidian.

Node *A_star(size_t row, size_t col, int start_x, int start_y, int goal_x, int goal_y, int grid[row][col]) {
    Node *open = newNode(start_x, start_y, 0, distanceToGoal(start_x, start_y, goal_x, goal_y), NULL);//start at (0,0)
    Node *close = newNode(start_x, start_y, 0, distanceToGoal(start_x, start_y, goal_x, goal_y), NULL);
    int neighbour_x, neighbour_y;
    float priority;
    float distanceStart;
    Node *current = NULL;
    Node *neighbour_node = NULL;
    int initial = 1;
    current = copyNode(open);

    while (1) {
        if (initial) {
            current = copyNode(open);
            initial = 0;
        } else {
            current = popGet(&open);
            pushNode(&close, current);
        }

        if (isReachedObj(current->x_coor, current->y_coor, goal_x, goal_y)) {
            printPathToGoal(current);
            break;
        }
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == j && i == 0) { //check if it is itself
                    continue;
                }
                neighbour_x = current->x_coor + i;
                neighbour_y = current->y_coor + j;

                if (neighbour_x < 0 || neighbour_y < 0) {
                    continue;
                }
                if (neighbour_x >= col || neighbour_y >= row) {
                    continue;
                }
                if (abs(i) == 1 && abs(j) == 1) { //go diagonally
                    distanceStart = current->distance_start + 14;
                } else {
                    distanceStart = current->distance_start + 10;
                }

                priority = distanceStart + distanceToGoal(neighbour_x, neighbour_y, goal_x, goal_y);
                neighbour_node = newNode(neighbour_x, neighbour_y, distanceStart, priority, current);
                if (grid[neighbour_y][neighbour_x] == 1 ||
                    isInClose(&close, neighbour_node)) { //assume 1 means occupied
                    continue;
                }

                if (!isInOpen(&open, neighbour_node)) {
                    pushNode(&open, neighbour_node);
                } else if (distanceStart < getDistanceStart(&open, neighbour_node)) {
                    changeCostParent(&open, neighbour_node, current);
                }
            }
        }
    }
    return current;
}

Node *createPathFromArray(size_t pathLength, int arrayPath[pathLength][2]) {
    Node *result = newNode(arrayPath[0][0], arrayPath[0][1], 0, 1, NULL);
    for (int i = 1; i < pathLength; i++) {
        Node *tmp = newNode(arrayPath[i][0], arrayPath[i][1], i, 1, result);
        result = tmp;
    }
    return result;
}

int comparePathsBasedOnCoords(Node *pathA, Node *pathB) {
    if (pathA == NULL && pathB == NULL) {
        return 0;
    } else if (pathA != NULL && pathB != NULL) {
        if (pathA->x_coor == pathB->x_coor && pathA->y_coor == pathB->y_coor &&
            comparePathsBasedOnCoords(pathA->parent, pathB->parent) == 0) {
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

int testGrid2x2WithStraightLine() {
    int grid[2][2] = {{0, 0},
                      {0, 0}};
    Node *start = newNode(0, 0, 0, 1, NULL);
    Node *expectedPath = newNode(1, 0, 1, 1, start);
    Node *resultPath = A_star(2, 2, 0, 0, 1, 0, grid);
    return comparePathsBasedOnCoords(expectedPath, resultPath);
}

int testGrid6x11WithLShapeObstacle() {
    int grid[6][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int start_x = 7;
    int start_y = 1;
    int goal_x = 4;
    int goal_y = 4;

    int expectedPathArray[7][2] = {{7, 1},
                                   {7, 2},
                                   {8, 3},
                                   {7, 4},
                                   {6, 4},
                                   {5, 4},
                                   {4, 4}};
    Node *expectedPath = createPathFromArray(7, expectedPathArray);
    Node *pathFromTheAlgorithm = A_star(6, 11, start_x, start_y, goal_x, goal_y, grid);
    return comparePathsBasedOnCoords(expectedPath, pathFromTheAlgorithm);
}

int main() {
    int totalTestCases = 0;
    int passedTestCases = 0;

    if (testGrid2x2WithStraightLine() == 0) {
        printf("testGrid2x2WithStraightLine - PASS\n");
        passedTestCases++;
    } else {
        printf("testGrid2x2WithStraightLine - FAIL\n");
    }
    totalTestCases++;

    if (testGrid6x11WithLShapeObstacle() == 0) {
        printf("testGrid6x11WithLShapeObstacle - PASS\n");
        passedTestCases++;
    } else {
        printf("testGrid6x11WithLShapeObstacle - FAIL\n");
    }
    totalTestCases++;

    printf("Total test cases: %d, success: %d", totalTestCases, passedTestCases);

    //int grid[5][5]= {{0,0,0,0,0},{0,0,0,0,0},{0,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0}};
}