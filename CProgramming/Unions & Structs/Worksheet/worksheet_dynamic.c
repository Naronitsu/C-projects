#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum MessageType { ERROR, TEMPERATURE, TEXT };

union MessageData {
int   errorCode;
float temperature;
char  text[50];
};

struct Message {
enum MessageType type;
union MessageData data;
struct Message *next;
};

int main(void) {
struct Message *head = NULL;
struct Message *tail = NULL;
int choice;

do {
    printf("\nMessage Handler Menu\n");
    printf("1. Enter an error message\n");
    printf("2. Enter a temperature message\n");
    printf("3. Enter a text message\n");
    printf("4. Display all messages\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
    (void)scanf("%d", &choice);

    switch (choice) {
        case 1: {
            struct Message *newMsg = malloc(sizeof(struct Message));
            if (!newMsg) {
                printf("Memory allocation failed.\n");
                break;
            }
            newMsg->type = ERROR;
            printf("Enter the error code: ");
            (void)scanf("%d", &newMsg->data.errorCode);
            newMsg->next = NULL;
            if (tail) tail->next = newMsg; else head = newMsg;
            tail = newMsg;
            break;
        }

        case 2: {
            struct Message *newMsg = malloc(sizeof(struct Message));
            if (!newMsg) {
                printf("Memory allocation failed.\n");
                break;
            }
            newMsg->type = TEMPERATURE;
            printf("Enter the temperature: ");
            (void)scanf("%f", &newMsg->data.temperature);
            newMsg->next = NULL;
            if (tail) tail->next = newMsg; else head = newMsg;
            tail = newMsg;
            break;
        }

        case 3: {
            struct Message *newMsg = malloc(sizeof(struct Message));
            if (!newMsg) {
                printf("Memory allocation failed.\n");
                break;
            }
            newMsg->type = TEXT;
            printf("Enter the text message: ");
            (void)scanf("%49s", newMsg->data.text); // reads one word only
            newMsg->next = NULL;
            if (tail) tail->next = newMsg; else head = newMsg;
            tail = newMsg;
            break;
        }

        case 4: {
            if (!head) {
                printf("No messages stored.\n");
            } else {
                printf("\nStored Messages:\n");
                int i = 1;
                struct Message *current = head;
                while (current) {
                    printf("[%d] ", i++);
                    switch (current->type) {
                        case ERROR:
                            printf("Error code = %d\n", current->data.errorCode);
                            break;
                        case TEMPERATURE:
                            printf("Temperature = %.2f\n", current->data.temperature);
                            break;
                        case TEXT:
                            printf("Text message = %s\n", current->data.text);
                            break;
                    }
                    current = current->next;
                }
            }
            break;
        }

        case 5:
            printf("Exiting program.\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
    }
} while (choice != 5);

// Free all allocated memory
struct Message *current = head;
while (current) {
    struct Message *temp = current;
    current = current->next;
    free(temp);
}

return 0;

}
