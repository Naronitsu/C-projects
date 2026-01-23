#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *filename;
    int use_global_total;
} ThreadArg;

typedef struct
{
    char *filename;
    long line_count;
    int ok;
} ThreadResult;

//Thread code
static void *count_lines_thread(void *arg)
{

    ThreadArg *threadArg = (ThreadArg *)arg;

    ThreadResult *res = (ThreadResult *)malloc(sizeof(ThreadResult));
    if (!res)
    {
        free(threadArg->filename);
        free(threadArg);
        return NULL;
    }

    res->filename = threadArg->filename;
    res->line_count = 0;
    res->ok = 0;

    FILE *fp = fopen(res->filename, "r");
    if (!fp)
    {
        free(threadArg);
        return res;
    }

    int c;
    // counting from new lines
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            res->line_count++;
    }

    if (!ferror(fp))
        res->ok = 1;
    fclose(fp);

    printf("%s: %ld\n", res->filename, res->line_count);

    free(threadArg);
    return res;
}

//closes all threads
void wipeThreads(int i, pthread_t *threads)
{
    for (int j = 0; j < i; j++)
        pthread_join(threads[j], NULL);
}

//asserts that atleast one filepath is passed
int assertArgs(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{

    //asserting args are correct
    if (assertArgs(argc, argv))return 1;
    int num_files = argc - 1;

    //allocating size for all threads required
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * (size_t)num_files);
    if (!threads)
        return 1;

    for (int i = 0; i < num_files; i++)
    {
        ThreadArg *arg = (ThreadArg *)malloc(sizeof(ThreadArg));
        //cleaning if malloc fails
        if (!arg)
        {
            wipeThreads(i, threads);
            free(threads);
            return 1;
        }

        arg->filename = strdup(argv[i + 1]);
        //cleaning if strdup fails
        if (!arg->filename)
        {
            free(arg);
            wipeThreads(i, threads);
            free(threads);
            return 1;
        }

        arg->use_global_total = 1;

        int rc = pthread_create(&threads[i], NULL, count_lines_thread, arg);
        // cleaning if threads fail
        if (rc != 0)
        {
            free(arg->filename);
            free(arg);
            wipeThreads(i, threads);
            free(threads);
            return 1;
        }
    }

    for (int i = 0; i < num_files; i++)
    {
        void *vp = NULL;
        pthread_join(threads[i], &vp);

        ThreadResult *res = (ThreadResult *)vp;
        if (res)
        {
            free(res->filename);
            free(res);
        }
    }

    free(threads);
    return 0;
}
