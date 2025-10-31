all: memoryLayout

memoryLayout: memoryLayout.c
    gcc -std=c11 -Wall -Wextra -O2 -g -o memoryLayout memoryLayout.c

run: memoryLayout
    ./memoryLayout

clean:
    -rm -f memoryLayout *.o