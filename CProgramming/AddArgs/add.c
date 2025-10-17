//Write a program to accept two integers as command line arguments and print their sum.
//if the arguments are not provided/not numbers/>2/<2, print an error message.
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("Error: Please provide exactly two integer arguments.\n");
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    if (num1 == 0 && argv[1][0] != '0') {
        printf("Error: First argument is not a valid integer.\n");
        return 1;
    }
    if (num2 == 0 && argv[2][0] != '0') {
        printf("Error: Second argument is not a valid integer.\n");
        return 1;
    }

    int sum = num1 + num2;
    printf("The sum of %d and %d is %d\n", num1, num2, sum);

    return 0;
}