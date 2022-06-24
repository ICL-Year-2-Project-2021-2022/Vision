#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include <stdbool.h>
#include <math.h>

typedef struct command {
    float distance;
    float initialAngle;
    struct command *next;
} Command;

typedef struct commandQueue {
    struct command *start;
    struct command *end;
} CommandQueue;

void insertCommand(CommandQueue *queue, Command *command);

Command* getNextCommand(CommandQueue* queue);

//void initializeCommandQueue(struct CommandQueue *queue);

bool isQueueEmpty(CommandQueue *queue);

#endif
