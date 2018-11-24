#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 10

int sum;
void *runner(void *param);

int main(int argc, char *argv[]) {
    pthread_t tid;
    pthread_attr_t attr;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./out <integer value>\n");
        return -1;
    }
    if (atoi(argv[1]) < 0) {
        fprintf(stderr, "%d must be >= 0", atoi(argv[1]));
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);

    printf("sum = %d\n", sum);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_t tidTemp;
        pthread_attr_t attrTemp;

        pthread_attr_init(&attrTemp);
        pthread_create(&tidTemp, &attrTemp, runner, argv[1]);
        pthread_join(tidTemp, NULL);
    }
    printf("sum = %d\n", sum);

    return 0;
}

void *runner(void *param) {
    int upper = atoi(param);
    sum = 0;

    for (int i = 0; i <= upper; i++) sum += i;

    pthread_exit(0);
}