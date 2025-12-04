#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: count_marks <textfile>\n");
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        printf("Failed to open %s\n", filename);
        return 1;
    }

    int mark;
    int total = 0;
    int pass = 0;

    while (fscanf(fp, "%d", &mark) == 1) {
        total++;
        if (mark >= 50)
            pass++;
    }

    fclose(fp);

    if (total == 0) {
        // Avoid divide-by-zero; define as 0%
        printf("Pass rate is 0 of 0 (0.00%%)\n");
        return 0;
    }

    double pct = (100.0 * pass) / total;
    printf("Pass rate is %d of %d (%.2f%%)\n", pass, total, pct);

    return 0;
}
