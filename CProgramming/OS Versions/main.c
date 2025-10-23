#if defined (WIN32)
   #include "hello_win.h"
#elif defined (__linux__)
   #include "hello_lnx.h"
#else
   #error "Only Windows (_WIN32) and Linux (__linux__) supported. Define one of those or update the checks."
#endif

int main (void) {
   hello();
   return 0;
}
