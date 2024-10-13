#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User structure
typedef struct User {
    char username[50];
    char password[50];
    struct User* next;
} User;

// Message structure
typedef struct Message {
    char sender[50];
    char receiver[50];
    char content[256];
    struct Message* next;
    struct Message* prev;
} Message;

// Stack for admin actions (you can define the required admin actions)
typedef struct Stack {
    char action[256];
    struct Stack* next;
} Stack;

// Queue for message sending/receiving
typedef struct Queue {
    Message* front;
    Message* rear;
} Queue;

// Doubly linked list for storing messages
typedef struct DoublyLinkedList {
    Message* head;
    Message* tail;
} DoublyLinkedList;

// Function prototypes
User* createUser(char* username, char* password);
void addUser(User** head, char* username, char* password);
User* findUser(User* head, char* username, char* password);

Message* createMessage(char* sender, char* receiver, char* content);
void addMessage(DoublyLinkedList* list, char* sender, char* receiver, char* content);
void displayMessages(DoublyLinkedList* list, char* username);

void push(Stack** stack, char* action);
char* pop(Stack** stack);

void enqueue(Queue* queue, Message* message);
Message* dequeue(Queue* queue);

void userLogin(User* userHead, DoublyLinkedList* messageList, Queue* messageQueue);
void adminLogin(Stack** adminStack);

// User Management Functions
User* createUser(char* username, char* password) {
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->next = NULL;
    return newUser;
}

void addUser(User** head, char* username, char* password) {
    User* newUser = createUser(username, password);
    newUser->next = *head;
    *head = newUser;
}

User* findUser(User* head, char* username, char* password) {
    User* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Message Management Functions
Message* createMessage(char* sender, char* receiver, char* content) {
    Message* newMessage = (Message*)malloc(sizeof(Message));
    strcpy(newMessage->sender, sender);
    strcpy(newMessage->receiver, receiver);
    strcpy(newMessage->content, content);
    newMessage->next = NULL;
    newMessage->prev = NULL;
    return newMessage;
}

void addMessage(DoublyLinkedList* list, char* sender, char* receiver, char* content) {
    Message* newMessage = createMessage(sender, receiver, content);
    if (list->head == NULL) {
        list->head = list->tail = newMessage;
    } else {
        list->tail->next = newMessage;
        newMessage->prev = list->tail;
        list->tail = newMessage;
    }
}

void displayMessages(DoublyLinkedList* list, char* username) {
    Message* temp = list->head;
    while (temp != NULL) {
        if (strcmp(temp->receiver, username) == 0) {
            printf("From: %s\nMessage: %s\n\n", temp->sender, temp->content);
        }
        temp = temp->next;
    }
}

// Stack and Queue Functions
void push(Stack** stack, char* action) {
    Stack* newAction = (Stack*)malloc(sizeof(Stack));
    strcpy(newAction->action, action);
    newAction->next = *stack;
    *stack = newAction;
}

char* pop(Stack** stack) {
    if (*stack == NULL) {
        return NULL;
    }
    Stack* temp = *stack;
    *stack = (*stack)->next;
    char* action = temp->action;
    free(temp);
    return action;
}

void enqueue(Queue* queue, Message* message) {
    if (queue->rear == NULL) {
        queue->front = queue->rear = message;
        return;
    }
    queue->rear->next = message;
    queue->rear = message;
}

Message* dequeue(Queue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    Message* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    return temp;
}

// User and Admin Login Functions
void userLogin(User* userHead, DoublyLinkedList* messageList, Queue* messageQueue) {
    char username[50];
    char password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    User* user = findUser(userHead, username, password);
    if (user == NULL) {
        printf("Invalid username or password.\n");
        return;
    }

    printf("Welcome, %s!\n", username);
    displayMessages(messageList, username);

    // For sending a message
    char receiver[50];
    char content[256];
    printf("Enter receiver's username: ");
    scanf("%s", receiver);
    printf("Enter message: ");
    getchar();  // To consume newline character
    fgets(content, 256, stdin);
    content[strcspn(content, "\n")] = 0;  // Remove newline character

    addMessage(messageList, username, receiver, content);
    Message* newMessage = createMessage(username, receiver, content);
    enqueue(messageQueue, newMessage);
}

void adminLogin(Stack** adminStack) {
    char action[256];
    printf("Enter admin action: ");
    getchar();  // To consume newline character
    fgets(action, 256, stdin);
    action[strcspn(action, "\n")] = 0;  // Remove newline character

    push(adminStack, action);
}

// Main Function
int main() {
    User* userHead = NULL;
    DoublyLinkedList messageList = {NULL, NULL};
    Queue messageQueue = {NULL, NULL};
    Stack* adminStack = NULL;

    int choice;
    while (1) {
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    char username[50];
                    char password[50];
                    printf("Enter username: ");
                    scanf("%s", username);
                    printf("Enter password: ");
                    scanf("%s", password);
                    addUser(&userHead, username, password);
                    printf("User registered successfully.\n");
                }
                break;
            case 2:
                userLogin(userHead, &messageList, &messageQueue);
                break;
            case 3:
                adminLogin(&adminStack);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
