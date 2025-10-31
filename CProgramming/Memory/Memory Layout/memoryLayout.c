#include <stdio.h>
#include <stdlib.h>

// GLOBAL INITIALISED - stored in data segment

int global_init = 10;

// GLOBAL UNITIALISED - stored in BSS (initialised at runtime)
int global_uninit;

// FUNCTION - stored on stack segment
void test() {

    int local_fn = 50; //lives only when test() runs
    static int im_static = 100;  //remains alive after function exits - stored in data segment
    printf("Address of local variable in TEST (stack):     %p\n", &local_fn);
    printf("Address of static local in TEST (data):        %p\n", &im_static);
    
}

int main() {
    // Local variable - stored on stack segment
    int local_var = 5;

    // Static local variable - stored in data segment
    static int static_local = 50;

    // Dynamically allocated memory - stored on heap
    int *heap_var = malloc(sizeof(int));
    *heap_var = 100;

    printf("Memory layout demonstration:\n\n");
    printf("Code segment stores the executable code:\n");
    printf("Address of function MAIN (code segment):    %p\n", main);
    printf("Address of function TEST (code segment):    %p\n\n", test);

    printf("Data segment stores global and static initialised variables:\n");
     
    printf("Address of initialised global (data):       %p\n", &global_init);
    printf("Address of static local in MAIN(data):      %p\n\n", &static_local);
    
    printf("BSS stores uninitialised global and static variables:\n");
    printf("Address of uninitialised global (bss):      %p\n\n", &global_uninit);
    
    printf("Heap segment stores dynamically allocated memory:\n");
    printf("Address of dynamic memory block (heap):     %p\n\n", heap_var);
    
    printf("Stack segment stores local variables in functions:\n");
    printf("Address of local variable in main (stack):  %p\n", &local_var);
    printf("Address of local ptr variable in main (stack):  %p\n\n", &heap_var);
   
    test();
    free(heap_var);
    return 0;
}
