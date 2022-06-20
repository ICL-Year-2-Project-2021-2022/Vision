#ifndef priority_queue_h
#define priority_queue_h

typedef struct node {
    float x_coor;
    float y_coor;
    float distance_start;
    float priority; //larger priority, see later
    struct node* parent;
    struct node* next;
} Node;

Node* newNode(float x_coor, float y_coor, float distance_start, float priority, struct node* parent);
float peek_x(Node** head) 

float peek_y(Node** head);

float peek_priority(Node** head);

Node popGet(Node** head);

void push(Node** head, float x_coor, float y_coor, float distance_start, float priority, struct node* parent);

int isEmpty(Node** head);

#endif





