#include <stdio.h>
#include <string.h>

#define MAX_MESSAGES 100

enum MessageType { ERROR, TEMPERATURE, TEXT };

union MessageData {
int   errorCode;
float temperature;
char  text[50];
};

struct Message {
enum MessageType type;
union MessageData data;
};

int main(void) {
struct Message messages[MAX_MESSAGES];
int count = 0;
int choice;

do {
    printf("\nMessage Handler Menu\n");
    printf("1. Enter an error message\n");
    printf("2. Enter a temperature message\n");
    printf("3. Enter a text message\n");
    printf("4. Display all messages\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            if (count < MAX_MESSAGES) {
                messages[count].type = ERROR;
                printf("Enter the error code: ");
                scanf("%d", &messages[count].data.errorCode);
                count++;
            } else {
                printf("Message storage full!\n");
            }
            break;

        case 2:
            if (count < MAX_MESSAGES) {
                messages[count].type = TEMPERATURE;
                printf("Enter the temperature: ");
                scanf("%f", &messages[count].data.temperature);
                count++;
            } else {
                printf("Message storage full!\n");
            }
            break;

        case 3:
            if (count < MAX_MESSAGES) {
                messages[count].type = TEXT;
                printf("Enter the text message: ");
                scanf("%49s", messages[count].data.text); 
                count++;
            } else {
                printf("Message storage full!\n");
            }
            break;

        case 4:
            if (count == 0) {
                printf("No messages stored.\n");
            } else {
                printf("\nStored Messages:\n");
                for (int i = 0; i < count; i++) {
                    printf("[%d] ", i + 1);
                    switch (messages[i].type) {
                        case ERROR:
                            printf("Error code = %d\n", messages[i].data.errorCode);
                            break;
                        case TEMPERATURE:
                            printf("Temperature = %.2f\n", messages[i].data.temperature);
                            break;
                        case TEXT:
                            printf("Text message = %s\n", messages[i].data.text);
                            break;
                    }
                }
            }
            break;

        case 5:
            printf("Exiting program.\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
    }
} while (choice != 5);

return 0;
}
