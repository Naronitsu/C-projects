#include <stdio.h>
#include <string.h>
#include "message.h"

int main(void) {
    struct Message oneMessage;
    printf("Press 1 for error code\n");
    printf("Press 2 for temperature\n");
    printf("Press 3 for text message\n");

    int choice;
    if (scanf("%d", &choice) != 1) return 1;
    int ch = getchar(); 
    (void)ch;

    switch (choice) {
    case 1: {
        union MessageData code;
        printf("Enter the error code: ");
        if (scanf("%d", &code.errorCode) != 1) return 1;
        oneMessage = make_message(ERROR, &code);
        break;
    }
    case 2: {
        union MessageData temp;
        printf("Enter the temperature: ");
        if (scanf("%f", &temp.temperature) != 1) return 1;
        oneMessage = make_message(TEMPERATURE, &temp);
        break;
    }
    case 3: {
        union MessageData buf;
        printf("Enter the text message: ");
        if (fgets(buf.text, sizeof buf.text, stdin) == NULL) return 1;
        buf.text[strcspn(buf.text, "\n")] = '\0';
        oneMessage = make_message(TEXT, &buf);
        break;
    }
    default:
        printf("Invalid selection.\n");
        return 1;
    }

    print_message(oneMessage);
    printf("sizeof(oneMessage) = %zu\n", sizeof(oneMessage));
    return 0;
}