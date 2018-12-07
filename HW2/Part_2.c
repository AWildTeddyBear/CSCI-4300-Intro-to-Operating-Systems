#include <stdio.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_SEAT 3
#define MAX_SLEEP 5
#define PATH_BUFF 100

sem_t sem_runner;
pthread_mutex_t mutex;

char filePath[PATH_BUFF];
int num = 0, next_meet = 0;

void wait_sleep(void), *run(void *runnerId);

int main(int argc, char *argv[]) {
	int *runnerIds, *sids;
	
	pthread_t *threads, *students;

	srand(time(NULL));
	
	if (argc == 1) {
		printf("No arguments were passed in.\nPlease enter the number of threads first, then the full path name\n");
		return 1;
	} else if(argc < 3) {
		printf("Not enough arguments were passed in. \nPlease enter the number of threads first, then the full path name\n");
		return 1;
	} else if(argc > 3) {
		printf("Too many arguments have been passed in. \nPlease enter the number of threads first, then the full path name\n");
		return 1;
	}

	printf("The proper amount of arguments have been passed in.\nNumber of threads that will be run is: %s\n File Path entered is: %s\n", argv[1], argv[2]);

	wait_sleep();

	int threadNum = atoi(argv[1]);
	strcpy(filePath, argv[2]);
	printf("ThreadNum is %d, and File path is %s\n", threadNum, filePath);
	
	threads = (pthread_t*) malloc(sizeof(pthread_t) * threadNum);
	runnerIds = (int*) malloc(sizeof(int) * threadNum);

	memset(runnerIds, 0, threadNum);

	sem_init(&sem_runner, 0, 0);
	pthread_mutex_init(&mutex, NULL);
		
	
	for (int i = 0; i < threadNum; ++i) {	
		runnerIds[i] = i + 1;
		pthread_create(&threads[i], NULL, run, (void*) &runnerIds[i]);
	} 
	
	for (int i = 0; i < threadNum; pthread_join(threads[++i], NULL));

	return 0;
}

void wait_sleep(void) {
	sleep(rand() % MAX_SLEEP + 1);
}

void *run(void* runnerId) {
	pthread_mutex_lock(&mutex);

	char command[80];
	FILE *fptr;
	if ((fptr = fopen(filePath, "r")) == NULL) { 
		printf("Cannot open file!");
		return 1;
	}
		
	while (fscanf(fptr, "%80s", command)) {
		if (!strcmp(command, "exit")) { break; }
		
		system(command);
		printf("\n");
	}

	fclose(fptr);
	pthread_mutex_unlock(&mutex);
}