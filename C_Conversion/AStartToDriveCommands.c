#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CommandQueue.h"

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
    if (old == NULL) {
        return NULL;
    } else {
        Node *temp = (Node *) malloc(sizeof(Node));
        temp->x_coor = old->x_coor;
        temp->y_coor = old->y_coor;
        temp->distance_start = old->distance_start;
        temp->priority = old->priority;
        temp->parent = old->parent;
        temp->next = NULL;
        return temp;
    }
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
        *head = new_node;
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

void freeList(Node **head) {
    Node *start;
    while ((*head) != NULL) {
        start = (*head);
        *head = (*head)->next;
        free(start);
    }
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


void reverseList(Node **head) {
    Node *prev = NULL;
    Node *current = *head;
    Node *next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


Node *copyList(Node *head) {
    if ((head) == NULL) {
        return NULL;
    } else {
        Node *newNode = (Node *) malloc(sizeof(Node));
        newNode = copyNode(head);
        newNode->next = copyList((head)->parent);
        return newNode;
    }
}

Node *getPathNodes(Node **head) {
    Node *start = (*head);
    Node *path = NULL;
    path = copyList(start);
    reverseList(&path);
    return path;
}


void printPathToGoal(Node *currentNode) {
    Node *start = (currentNode);
    while (start != NULL) {
        printf("Coordinates: %d %d \n", start->x_coor, start->y_coor);
        start = start->parent;
    }
}

void printQueue(Node **head) {
    Node *start = (*head);
    if (start == NULL) {
        printf("List is Empty\n");
    }
    int i = 1;
    while (start != NULL) {
        printf("Step %d Coordinates: %d %d \n", i, start->x_coor, start->y_coor);
        start = start->next;
        i++;
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
            if (isEmpty(&open)) {
                printf("Cannot reach target\n");
                freeList(&close);
                freeList(&open);
                return NULL;
            }
            current = popGet(&open);
            pushNode(&close, current);
        }

        if (isReachedObj(current->x_coor, current->y_coor, goal_x, goal_y)) {
            Node *result = getPathNodes(&current);
            //printPathToGoal(current);
            freeList(&open);
            freeList(&close);
            return result;
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
                //printf("Neighbour %d %d\n", neighbour_x, neighbour_y );

                if (isInOpen(&open, neighbour_node) == 0) {
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

float floatAbs(float a) {
    if (a < 0) {
        return -a;
    }
    return a;
}

// this needs to be tested with more angles
float angleOfSmallestRotation(float start, float end) {
    float first = floatAbs(end - start);
    float second = floatAbs(end - (start + 2 * M_PI));
    if (first < second) {
        return end - start;
    } else {
        return end - (start + M_2_PI);
    }
}

struct Command* createNewCommand(float distance, float angle) {
    Command *command = (Command *) malloc(sizeof(Command));
    command->initialAngle = angle;
    command->distance = 0;
    command->next = NULL;
    return command;
}

struct CommandQueue *convertGridMovesToDriveCommands(Node *path, int cellWidth, int cellHeight, float initialRotation) {
    CommandQueue *result = (CommandQueue *) malloc(sizeof(CommandQueue));
    result->start = NULL;
    result->end = NULL;
    Node *previous = path;
    float rotation = initialRotation;
    if (path != NULL) {
        path = path->next;
    }
    while (path != NULL) {
        if (path->x_coor != previous->x_coor && path->y_coor != previous->y_coor) {
            if (path->x_coor != previous->x_coor && path->y_coor != previous->y_coor) {
                if (path->x_coor > previous->x_coor && path->y_coor > previous->y_coor && rotation != (float) M_PI_4) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, M_PI_4));
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                } else if (path->x_coor > previous->x_coor && path->y_coor < previous->y_coor && rotation != (float) 7 * M_PI_4) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, 7 * M_PI_4));
                    angleCommand->distance = 0;
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                } else if (path->x_coor < previous->x_coor && path->y_coor < previous->y_coor && rotation != (float) 5 * M_PI_4) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, 5 * M_PI_4));
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                } else if (rotation != (float) 3 * M_PI_4) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, 3 * M_PI_4));
                    angleCommand->distance = 0;
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                }
                Command * driveCommand = createNewCommand(sqrt(cellWidth * cellWidth + cellHeight * cellHeight), 0);
                insertCommand(result, driveCommand);
            }
            //rotate
        } else {
            if (path->x_coor > previous->x_coor && path->y_coor == previous->y_coor) {
                if (rotation != 0) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, 0));
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                }
                Command *distanceCommand = createNewCommand(cellWidth, 0);
                insertCommand(result, distanceCommand);
            } else if (path->x_coor < previous->x_coor && path->y_coor == previous->y_coor) {
                if (rotation != (float) M_PI) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, M_PI));
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                }
                Command * distanceCommand = createNewCommand(cellWidth, 0);
                insertCommand(result, distanceCommand);
            } else if (path->y_coor > previous->y_coor && path->x_coor == previous->x_coor) {
                if (rotation != (float) M_PI_2) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, M_PI_2));
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                }
                Command * distanceCommand = createNewCommand(cellHeight, 0);
                insertCommand(result, distanceCommand);
            } else if (path->y_coor < previous->y_coor && path->x_coor == previous->x_coor) {
                if (rotation != (float) (3 * M_PI_2)) {
                    Command * angleCommand = createNewCommand(0, angleOfSmallestRotation(rotation, 3 * M_PI_2));
                    rotation += angleCommand->initialAngle;
                    insertCommand(result, angleCommand);
                }
                Command * distanceCommand = createNewCommand(cellHeight, 0);
                insertCommand(result, distanceCommand);
            }
        }
        if (path != NULL) {
            previous = path;
            path = path->next;
        }
    }
    return result;
}

int compareCommandQueues(CommandQueue *currentQueue, CommandQueue *expectedQueue) {
    while (!isQueueEmpty(currentQueue) && !isQueueEmpty(expectedQueue)) {
        Command *commandA = getNextCommand(currentQueue);
        Command *commandB = getNextCommand(expectedQueue);
        printf("Command A: dis -> %f, angle -> %f, Command B: dis -> %f, angle -> %f\n", commandA->distance, commandA->initialAngle, commandB->distance, commandB->initialAngle);
        if (commandA->distance != commandB->distance || !(commandA->initialAngle + 0.0001f >= commandB->initialAngle && commandA->initialAngle - 0.0001f <= commandB->initialAngle)) {
            return -1;
        }
    }
    if (isQueueEmpty(currentQueue) && isQueueEmpty(expectedQueue)) {
        return 0;
    } else {
        return -1;
    }
}

int testDrivingOnHorizontalLine() {
    int grid[2][2] = {{0, 0},
                      {0, 0}};
    int start_x = 0;
    int start_y = 0;
    int goal_x = 1;
    int goal_y = 0;
    Node *path = A_star(2, 2, start_x, start_y, goal_x, goal_y, grid);
    printQueue(&path);
    CommandQueue *queue = convertGridMovesToDriveCommands(path, 50, 50, 0);

    Command *command1 = (Command *) malloc(sizeof(Command));
    command1->distance = 50;
    command1->initialAngle = 0;

    CommandQueue *expectedQueue = (CommandQueue *) malloc(sizeof(CommandQueue));
    insertCommand(expectedQueue, command1);

    return compareCommandQueues(queue, expectedQueue);
}

int testDrivingOnVerticalLine() {
    int grid[2][2] = {{0, 0},
                      {0, 0}};
    int start_x = 0;
    int start_y = 0;
    int goal_x = 0;
    int goal_y = 1;
    Node *path = A_star(2, 2, start_x, start_y, goal_x, goal_y, grid);
    printQueue(&path);
    CommandQueue *queue = convertGridMovesToDriveCommands(path, 50, 50, M_PI_2);

    Command *command1 = (Command *) malloc(sizeof(Command));
    command1->distance = 50;
    command1->initialAngle = 0;

    CommandQueue *expectedQueue = (CommandQueue *) malloc(sizeof(CommandQueue));
    insertCommand(expectedQueue, command1);

    return compareCommandQueues(queue, expectedQueue);
}

int testRotateAndDriveVertical() {
    int grid[2][2] = {{0, 0},
                      {0, 0}};
    int start_x = 0;
    int start_y = 0;
    int goal_x = 0;
    int goal_y = 1;
    Node *path = A_star(2, 2, start_x, start_y, goal_x, goal_y, grid);
    printQueue(&path);
    CommandQueue *queue = convertGridMovesToDriveCommands(path, 50, 50, 0);

    Command *command1 = (Command *) malloc(sizeof(Command));
    command1->distance = 0;
    command1->initialAngle = M_PI_2;

    Command *command2 = (Command *) malloc(sizeof(Command));
    command2->distance = 50;
    command2->initialAngle = 0;

    CommandQueue *expectedQueue = (CommandQueue *) malloc(sizeof(CommandQueue));

    insertCommand(expectedQueue, command1);
    insertCommand(expectedQueue, command2);

    return compareCommandQueues(queue, expectedQueue);
}

int testGoDiagonally() {
    int grid[2][2] = {{0, 0},
                      {0, 0}};
    int start_x = 0;
    int start_y = 0;
    int goal_x = 1;
    int goal_y = 1;
    Node *path = A_star(2, 2, start_x, start_y, goal_x, goal_y, grid);
    printQueue(&path);
    CommandQueue *queue = convertGridMovesToDriveCommands(path, 50, 50, 0);

    Command *command1 = (Command *) malloc(sizeof(Command));
    command1->distance = 0;
    command1->initialAngle = M_PI / 4;

    Command *command2 = (Command *) malloc(sizeof(Command));
    command2->distance = sqrt(50 * 50 * 2);
    command2->initialAngle = 0;

    CommandQueue *expectedQueue = (CommandQueue *) malloc(sizeof(CommandQueue));

    insertCommand(expectedQueue, command1);
    insertCommand(expectedQueue, command2);

    return compareCommandQueues(queue, expectedQueue);
}

int testConversionOfCommandsOnStraightLine() {
    int grid[6][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    //int grid[5][5]= {{0,0,0,0,0},{0,0,0,0,0},{0,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    int start_x = 8;
    int start_y = 1;
    int goal_x = 8;
    int goal_y = 4;
    int cellWidth = 20;
    int cellHeight = 20;
    Node *path = A_star(6, 11, start_x, start_y, goal_x, goal_y, grid);
    printQueue(&path);
}

int test_withObstacles() {
    int grid[6][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
                       {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    //int grid[5][5]= {{0,0,0,0,0},{0,0,0,0,0},{0,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    int start_x = 7;
    int start_y = 1;
    int goal_x = 4;
    int goal_y = 4;
    Node *path = A_star(6, 11, start_x, start_y, goal_x, goal_y, grid);
    printQueue(&path);

    CommandQueue *queue = convertGridMovesToDriveCommands(path, 50, 50, 0);
    CommandQueue *expectedQueue = (CommandQueue *) malloc(sizeof(CommandQueue));

    Command *command1 = (Command *) malloc(sizeof(Command));
    command1->distance = 0;
    command1->initialAngle = M_PI_2;
    insertCommand(expectedQueue, command1);

    Command *command2 = (Command *) malloc(sizeof(Command));
    command2->distance = 50;
    command2->initialAngle = 0;
    insertCommand(expectedQueue, command2);

    Command *command3 = (Command *) malloc(sizeof(Command));
    command3->distance = 0;
    command3->initialAngle = - (M_PI / 4);
    insertCommand(expectedQueue, command3);

    Command *command4 = (Command *) malloc(sizeof(Command));
    command4->distance = sqrt(50 * 50 * 2);
    command4->initialAngle = 0;
    insertCommand(expectedQueue, command4);

    Command *command5 = (Command *) malloc(sizeof(Command));
    command5->distance = 0;
    command5->initialAngle = M_PI_2;
    insertCommand(expectedQueue, command5);

    Command *command6 = (Command *) malloc(sizeof(Command));
    command6->distance = sqrt(50 * 50 * 2);
    command6->initialAngle = 0;
    insertCommand(expectedQueue, command6);

    Command *command7 = (Command *) malloc(sizeof(Command));
    command7->distance = 0;
    command7->initialAngle = (M_PI / 4);
    insertCommand(expectedQueue, command7);

    Command *command8 = (Command *) malloc(sizeof(Command));
    command8->distance = 50;
    insertCommand(expectedQueue, command8);

    Command *command9 = (Command *) malloc(sizeof(Command));
    command9->distance = 50;
    insertCommand(expectedQueue, command9);

    Command *command10 = (Command *) malloc(sizeof(Command));
    command10->distance = 50;
    insertCommand(expectedQueue, command10);

    return compareCommandQueues(queue, expectedQueue);
}

int main() {
    printf("Hello!\n");

    if (testDrivingOnHorizontalLine() == 0) {
        printf("testDrivingOnHorizontalLine() - PASS\n");
    } else {
        printf("testDrivingOnHorizontalLine() - FAIL\n");
    }

    if (testDrivingOnVerticalLine() == 0) {
        printf("testDrivingOnVerticalLine() - PASS\n");
    } else {
        printf("testDrivingOnVerticalLine() - FAIL\n");
    }

    if (testRotateAndDriveVertical() == 0) {
        printf("testRotateAndDriveVertical() - PASS\n");
    } else {
        printf("testRotateAndDriveVertical() - FAIL\n");
    }

    if (testGoDiagonally() == 0) {
        printf("testGoDiagonally() - PASS\n");
    } else {
        printf("testGoDiagonally() - FAIL\n");
    }

    if (test_withObstacles() == 0) {
        printf("test_withObstacles() - PASS\n");
    } else {
        printf("test_withObstacles() - FAIL\n");
    }
}