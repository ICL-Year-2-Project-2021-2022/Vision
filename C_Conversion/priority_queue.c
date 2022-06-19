
#include <stdio.h>
#include <stdlib.h>
// priority Node
typedef struct node {
    int x_coor;
    int y_coor;
    float distance_start;
    float priority; //lower priority, first served
    struct node* parent;
    struct node* next;
} Node;

Node* newNode(int x_coor, int y_coor, float distance_start, float priority, struct node* parent) {
   Node* temp = (Node*)malloc(sizeof(Node));
   temp->x_coor = x_coor;
   temp->y_coor = y_coor;
   temp->distance_start = distance_start;
   temp->priority = priority;
   temp->parent = parent;
   temp->next = NULL;
   return temp;
}
float peek_x(Node** head) {
   return (*head)->x_coor;
}

float peek_y(Node** head) {
   return (*head)->y_coor;
}

float peek_priority(Node** head) {
   return (*head)->priority;
}

void peek_parent(Node** head){
    printf("%f %f", (*head)->parent->x_coor, (*head)->parent->y_coor);
}

Node* popGet(Node** head) {
   Node* temp = *head;
   (*head) = (*head)->next;
   return temp;
}


int isInClose(Node** head, Node* currentNode){
    Node* start = (*head);
    if (start == NULL){
        
        if (start->x_coor== currentNode->x_coor && start->y_coor == currentNode-> y_coor){
            return 1;
        }
    }
    
    while (start->next != NULL) {
        if (start->x_coor== currentNode->x_coor && start->y_coor == currentNode-> y_coor){
            return 1;
        }
        
        start = start->next;
    }
    return 0;
}




void push(Node** head, float x_coor, float y_coor, float distance_start, float priority, struct node* parent) {
    Node* start = (*head);
    Node* temp = newNode(x_coor,y_coor,distance_start,priority,parent);
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

void pushNode(Node** head, Node* new_node) {
    Node* start = (*head);

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

// Function to check the queue is empty
int isEmpty(Node** head) {
   return (*head) == NULL;
}


void changeCostParent(Node** head, Node* neighbourNode, Node* currentNode){
    Node* start = (*head);
    while (start->next != NULL) {
       
        if (start->x_coor== neighbourNode->x_coor && start->y_coor == neighbourNode-> y_coor){
             printf("reached");
            start->priority = neighbourNode->priority;
            start->parent = currentNode;
        }
        start = start->next;
    }
}

int main() {
    Node* pq = newNode(0, 0, 5,2,  NULL);
    push(&pq, 1, 2 , 4 , 5, pq );
    push(&pq, 3, 3, 9, 10, pq);
    Node* p1 = newNode(2, 0, 2, 7,  NULL);
    pushNode(&pq, p1);
    Node* p2 = newNode(2, 0, 2, 9,  pq);

    Node* p3 = newNode(2,0,2, 19, pq2);
    changeCostParent(&pq, p2, pq);
    Node* result;
    if (isInClose(&pq,p3 )){
        printf("reached");
    }
    while (!isEmpty(&pq)){
        printf("%f\n", peek_priority(&pq));
        //peek_parent(&pq);
        result = popGet(&pq);
    }
    return 0;
}







