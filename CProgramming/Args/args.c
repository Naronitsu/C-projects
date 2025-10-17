#include <stdio.h>

/*
This program demonstrates command-line argument handling.
Parameters should not be named differently.
    argc - number of arguments (Name of the program is also counted)
    argv - array of dynamic strings - stores name of program, and the rest of the arguments

THIS WORKS ONLY WITH MAIN.
*/


// This will accept CLAs and print them out
int main(int argc, char* argv[]){

    for (int i = 0; i < argc; i++) {
        printf("Argument %d has value: %s\n", i, argv[i]);
    }

    return 0;
}