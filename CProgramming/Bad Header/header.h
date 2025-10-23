//always use file guards in header files to make sure you dont include the code multiple times.
//Another solution is to use #pragma once at the top of the header file instead of file guards.

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <stdio.h>
void test(void) {
    printf("Hello World\n");
}
#endif