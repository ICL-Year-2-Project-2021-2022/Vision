#include <stdio.h>
#include <stdlib.h>
#include "CommandQueue.h"

#define COMMANDS_IN_QUEUE 5

void insertCommand(CommandQueue *queue, Command *command) {
    if (!queue->start) {
        queue->start = command;
    } else if (!queue->end) {
        queue->start->next = command;
        queue->end = command;
    } else {
        queue->end->next = command;
        queue->end = queue->end->next;
    }
    command->next = 0;
}

Command* getNextCommand(CommandQueue* queue) {
    Command* result = queue->start;
    if (queue->start != NULL) {
        queue->start = queue->start->next;
    }
    return result;
}

/*void initializeCommandQueue(struct CommandQueue *queue) {
    for (int i = 0; i < COMMANDS_IN_QUEUE; i++) {
        struct Command* newCommand = new Command;
        newCommand->distance = commandsArray[i][0];
        newCommand->initialAngle = commandsArray[i][1];
        newCommand->finalAngle = commandsArray[i][2];
        newCommand->next = nullptr;
        insertCommand(queue, newCommand);
    }
}*/

bool isQueueEmpty(CommandQueue *queue) {
    return !queue->start;
}
