#include <stdio.h>
 
 
int main(void){
    #ifdef FULL
        printf("This is the full version");
    #else
        printf("This is the demo version");
    #endif
 
}
 