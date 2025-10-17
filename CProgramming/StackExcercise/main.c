#include <stdio.h>
#include "stack.h"

int main(void) {
    nodePtr top = createStack();
    int choice = 0, value = 0, popped = 0;

    do {
        printf("\n     My Stack Menu\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Show Stack\n");
        printf("4. How many items on stack?\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            // bad input: clear stdin and continue
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Invalid input. Try again.\n");
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter value to push: ");
                if (scanf("%d", &value) == 1) {
                    if (push(&top, value))
                        printf("Pushed %d onto stack.\n", value);
                    else
                        printf("Error: allocation failed.\n");
                } else {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) { }
                    printf("Invalid value.\n");
                }
                break;

            case 2:
                if (pop(&top, &popped))
                    printf("Popped value: %d\n", popped);
                else
                    printf("Stack is empty.\n");
                break;

            case 3:
                printStack(top);   // uses the implementation in stack.c
                break;

            case 4:
                printf("Stack has %d item(s).\n", stackSize(top));
                break;

            case 5:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    destroyStack(&top);  // free all nodes
    return 0;
}
