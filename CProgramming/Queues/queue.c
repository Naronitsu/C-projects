#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

nodePtr head = NULL;
nodePtr tail = NULL;

// Function to load queue from file
void LoadQueueFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    CreateQueue(&head, &tail);

    int value;
    while (fscanf(file, "%d", &value) == 1) {
        if (!Enqueue(&head, &tail, value)) {
            printf("Error: Failed to enqueue %d\n", value);
        }
    }

    fclose(file);
    printf("Queue loaded successfully from %s\n", filename);
}

// Main menu
void ShowMenu() {
    printf("\nMy Queue\n");
    printf("1. Load Queue\n");
    printf("2. Next Please!\n");
    printf("3. Add to Queue\n");
    printf("4. Show Queue\n");
    printf("5. How many more to serve?\n");
    printf("6. Exit\n");
    printf("Choose an option: ");
}

int main() {
    int choice, value;
    char filename[100];

    CreateQueue(&head, &tail);

    while (1) {
        ShowMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename to load: ");
                scanf("%s", filename);
                LoadQueueFromFile(filename);
                break;

            case 2:
                if (IsEmptyQueue(head)) {
                    printf("Queue is empty. No one to serve.\n");
                } else {
                    printf("Next item to be served is: %d\n", head->item);
                    Dequeue(&head, &tail);
                }
                break;

            case 3:
                printf("Enter number to add to queue: ");
                scanf("%d", &value);
                if (Enqueue(&head, &tail, value)) {
                    printf("%d added to queue.\n", value);
                } else {
                    printf("Error: Could not add to queue.\n");
                }
                break;

            case 4:
                ShowQueue(head);
                break;

            case 5:
                value = QueueSize(head);
                if (value == 0) {
                    printf("No more items to serve.\n");
                } else {
                    printf("Items remaining in queue: %d\n", value);
                }
                break;

            case 6:
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}
