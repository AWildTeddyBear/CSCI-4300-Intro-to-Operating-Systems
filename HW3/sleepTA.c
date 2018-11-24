#include <stdio.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_SEAT 3
#define MAX_SLEEP 5

/*
	================
	Global Variables
	================
*/

// Semaphore for student
sem_t sem_student;

// Semaphore to wake TA
sem_t sem_ta;


// Mutex to protect critical section
pthread_mutex_t mutex;


// Students waiting for help
int waiting = 0;

// Three chairs either empty (0) or full (1)
int chairs[NUM_SEAT];

// Index of the next seat a student should sit in
int next_seat = 0;

// Id of the next student who will meet with the TA
int next_meet = 0;


/*
	===================
	Function Prototypes
	===================
*/
void wait_sleep(), *student(void *student_id), *ta_help();


int main(void) {
	// Pointer to list of sids
	int *sids;

	// Total number of students given by user
	int total_students;

	// pointer to list of student threads
	pthread_t *students;
	
	// Pointer to the single TA thread
	pthread_t ta;


	// Seed RNG
	srand(time(NULL));

	// User input
	printf("How many students will visit the TA? "); scanf("%d", &total_students);

	// Initialize number of students
	students = (pthread_t*) malloc(sizeof(pthread_t) * total_students);

	// Initialize ids for students
	sids = (int*) malloc(sizeof(int) * total_students);


	// Set the value of sid for each student to 0
	memset(sids, 0, total_students);

	/*
		Initialize semaphore at address for students
		semaphore is shared between threads of a process
		initial value for the semaphore is 0
	*/
	sem_init(&sem_student, 0, 0);

	/*
		Initialize semaphore at address for TA
		semaphore is shared between threads of a process
		initial value for the semaphore is 1
	*/
	sem_init(&sem_ta, 0, 1);

	// Initialize pthread mutex
	pthread_mutex_init(&mutex, NULL);

	/*
		Create TA thread first, NULL (default attributes)
		use the ta() function, and no parameters sent
	*/
	pthread_create(&ta, NULL, ta_help, NULL);
	

	// Create each student thread one by one - send sid + 1 because 0 indicates an empty seat
	for (int i = 0; i < total_students; i++) {
		sids[i] = i + 1;
		pthread_create(&students[i], NULL, student, (void*) &sids[i]);
	} 
	
    // Wait for the TA thread to finish
	pthread_join(ta, NULL);
	
	// Join each student thread one by one
	for (int i = 0; i < total_students; pthread_join(students[i++], NULL));	
	
	return 0;
}

/*
	Called when TA needs to sleep or when students need to wait
	before checking if TA is available
*/
void wait_sleep() {	sleep(rand() % MAX_SLEEP + 1); }

/*
	Thread to handle the students finding a seat and checking for TA
*/
void *student(void *student_id) {
	int sid = *(int*)student_id;

	printf("\x1B[36mStudent-Thread\x1B[0m: Sid %d is checking available seats\n", sid);		
	
	while(1) {
		// Make the student wait a random amount of time
		wait_sleep();
		
		// Lock mutex
		pthread_mutex_lock(&mutex);

		// Make sure we have less than 3 people waiting (seats available)
		if (waiting < NUM_SEAT) {
			// Sit the student down in the next avaliable chair
			chairs[next_seat] = sid;

			// Make the seat we have a student in reserved now
			next_seat = (next_seat + 1) % NUM_SEAT;

			// Increment to say another student is waiting
			waiting++;

			// Make the ID of next_meet to the one meeting with TA
			next_meet = sid;

			// Unlock Mutex
			pthread_mutex_unlock(&mutex);

			// If a student is waiting, then print that and increment up one
			printf("\x1B[36mStudent-Thread\x1B[0m: \x1B[33mSid %d is waiting\x1B[0m\n", sid);
			
			// Signal that student is avaliable
			sem_post(&sem_student);

			// Wait until TA is avaliable
			sem_wait(&sem_ta);

			// Print current seat status
			printf("\x1B[35mStudent-Thread_Seat Status\x1B[0m: (S1) %d (S2) %d (S3) %d\n", chairs[0], chairs[1], chairs[2]);
		} else {
			// There are no chairs avaliable

			// Unlock Mutex
			pthread_mutex_unlock(&mutex);

			// No seats avaliable, student is still waiting
			printf("\x1B[36mStudent-Thread\x1B[0m: \x1B[31mno seats available, Sid %d will come back later\x1B[0m\n", sid);		
		}
	}				
}

/*
	Thread to handle TA helping students or sleeping
*/
void *ta_help() {
	while (1) {
		// Wait until the student flags they are ready
		sem_wait(&sem_student);

		// Make the student wait a random amount of time
		wait_sleep();

		// TA is currently helping a student
		printf("\x1B[32mTA-Thread\x1B[0m: Currently meeting with Sid %d\n", next_meet);	

		// Signal that student is avaliable
		sem_post(&sem_ta);

		// Lock mutex to make sure we can write vars safely
		pthread_mutex_lock(&mutex);

		// Update the next student to meet
		chairs[next_seat] = next_meet;

		// Cycle out the chairs
		chairs[0] = chairs[1];
		chairs[1] = chairs[2];
		chairs[2] = 0;

		// Print updated seat status
		printf("\x1B[35mTA-Thread_Seat Status\x1B[0m: (S1) %d (S2) %d (S3) %d\n", chairs[0], chairs[1], chairs[2]);

		// Decrement the avaliable waiting students
		waiting--;

		// Unlock Mutex
		pthread_mutex_unlock(&mutex);

		// TA is finished helping student
		printf("\x1B[32mTA-Thread\x1B[0m: \x1B[32mTA has finished helping the student\x1B[0m\n");
	}	
}