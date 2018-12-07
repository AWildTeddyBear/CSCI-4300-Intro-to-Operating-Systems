#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_PROC 5

void PrintMenu(), FCFS(), SJF(), SRTF(), PS(), RR();

int main(void) {
	int choice = 0, quit = 0, valid;
	
	PrintMenu();
		
	while (quit == 0) {
        printf("Selection: ");

		valid = scanf("%d", &choice);	

		switch (choice) {
            case 1: { FCFS(); break; }
            case 2: { SJF(); break; }
            case 3: { SRTF(); break; }
            case 4: { PS(); break; }
            case 5: { RR(); break; }
            case 6: { PrintMenu(); break; }
            case 7: { quit = 1; break; }
            default: {
				printf("Invalid Input.  Please enter a number between 1-7\n");
				break;
			}
		}

		// Flush scanf buffer (prevents infinite menu loop)
		while(getchar() != '\n');
	}
		
    return 0;
}

void PrintMenu() {
	printf("Please select a scheduling algorithm for the next CPU burst\n");
	printf("1. First Come First Serve (FCFS)\n");
	printf("2. Shortest Job First (SJF)\n");
	printf("3. Shortest Remaining Time First (SRTF)\n");
	printf("4. Priority Scheduling (PS)\n");
	printf("5. Round Robin (RR)\n");
	printf("6. Re-Print Menu\n");
	printf("7. Exit Program\n");
}

// First Come First Serve
void FCFS() {
	int burst_time[NUM_PROC];
	int total_time = 0;
	int timer = 1;
	float avg_wait = 0;

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Please enter the burst time for process %d: ", i + 1);
		scanf("%d", &burst_time[i]);
	}

	printf("\nProcess Data\n");
	printf("------------------------\n");

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Process %d: Burst Time %d\n", i + 1, burst_time[i]);
		total_time += burst_time[i];
	}

	printf("Total Burst Time: %d\n", total_time);

	// Output the CPU burst as first come first served order - burst time ties go to the earlier process

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("timer is %d\n", timer - 1);

		for (int j = 0; j < burst_time[i]; ++j) {
			printf("%d: P%d\n", timer, i + 1);
			timer++;
		}

		avg_wait += timer - 1;
	}

	printf("Average Waiting Time: %f\n", avg_wait / NUM_PROC);
}

// Shortest Job First
void SJF() {
	int burst_time[NUM_PROC], sorted[NUM_PROC] = {1, 2, 3, 4, 5};
	int tmp, total_time, timer = 1;
    float avg_wait = 0;

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Please enter the burst time for process %d: ", i + 1);
		scanf("%d", &burst_time[i]);
	}

	printf("\nProcess Data\n");
	printf("------------------------\n");

    for (int i = 0; i < NUM_PROC; ++i) {
        for (int j = i + 1; j < NUM_PROC; ++j) {
            if (burst_time[i] > burst_time[j]) {
                tmp = burst_time[i];
                burst_time[i] = burst_time[j];
                burst_time[j] = tmp;

                tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Process %d: Burst Time %d\n", sorted[i], burst_time[i]);
		total_time += burst_time[i];
	}
    
	for (int i = 0; i < NUM_PROC; ++i) {
		for (int j = 0; j < burst_time[i]; ++j) {
			printf("%d: P%d\n", timer, sorted[i]);
			timer++;
		}
		printf("timer is %d\n", timer - 1);
		avg_wait += timer - 1;
	}

    printf("Average Waiting Time: %f\n", avg_wait / NUM_PROC);

	printf("Total Burst Time: %d\n", total_time);
	// Determine which processes have the lowest burst CPU burst time and output accordingly
}

// Shortest Remaning Time First
void SRTF() {
	// Determine the output order of the processes while taking into account the arrival times
	// You will have to interrupt processes based on time remaining
	int burst_time[NUM_PROC], arrival[NUM_PROC], time_remaining[NUM_PROC];
	int tmp, total_time, left = 0;

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Please enter the burst time and arrival time for process [example: 10 4] %d: ", i + 1);
		scanf("%d %d", &burst_time[i], &arrival[i]);
		time_remaining[i] = burst_time[i];
	}
	
	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Process %d: Burst Time %d Arrvial time: %d\n", i+1, burst_time[i], arrival[i]);
		total_time += burst_time[i];
	}
	
	printf("\nProcess Data\n");
	printf("------------------------\n");

	time_remaining[NUM_PROC - 1] = 100; // max a number to intially get a max
	int timeCycle = 0;  // this keeps track of the ms of the program
	for (int i = 0; left != NUM_PROC; ++i) { 
		int small = 4;
		
		for (int j = 0; j < NUM_PROC; ++j) { // checks for shortest vs arrival time
			if ((arrival[j] <= i) && (time_remaining[j] < time_remaining[small]) && time_remaining[j] > 0) { small = j; }
		}

		time_remaining[small]--; // decrements time remaining
		if (time_remaining[small] == 0) {
			left++; // one program is done//keep increment as each is done. this exits the loop

			printf("Process %d has finished!\n", small + 1);
			for (int k = 1; k <= burst_time[small]; ++k) {
				timeCycle++;
				printf("%d:  P%d\n", timeCycle, small + 1);
			}
		}
	}

	printf("Total Burst Time: %d\n", total_time);
}

// Priority Scheduler
void PS() {
	// Determine which processes are executed first based on the provided priorities by the user
	// You will have to re-order the processes

	int burst_time[NUM_PROC], sorted[NUM_PROC] = {1, 2, 3, 4, 5}, priority[NUM_PROC] = {1, 2, 3, 4, 5};
	int tmp, total_time, timer = 1;
  	float avg_wait = 0;

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Please enter the burst time for process %d: ", i + 1);
		scanf("%d", &burst_time[i]);
	}

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Please enter the priority for process %d: ", i + 1);
		scanf("%d", &priority[i]);
	}
	
	printf("\nProcess Data\n");
	printf("------------------------\n");

    for (int i = 0; i < NUM_PROC; ++i) {
        for (int j = i + 1; j < NUM_PROC; ++j) {
            if (priority[i] > priority[j]) {
                tmp = priority[i];
                priority[i] = priority[j];
                priority[j] = tmp;

				tmp = burst_time[i];
                burst_time[i] = burst_time[j];
                burst_time[j] = tmp;
		
                tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Process %d: Burst Time %d Priorty %d:\n ", sorted[i], burst_time[i], priority[i]);
		total_time += burst_time[i];
	}
    
	for (int i = 0; i < NUM_PROC; ++i) {
		for (int j = 0; j < burst_time[i]; ++j) {
			printf("%d: P%d\n", timer, sorted[i]);
			timer++;
		}

		printf("timer is %d\n", timer - 1);
		avg_wait += timer - 1;
	}

    printf("Average Waiting Time: %f\n", avg_wait / NUM_PROC);

	printf("Total Burst Time: %d\n", total_time);
}

// Round-Robin
void RR() {
	// Execute the processes one at a time by the user provided time quantum until all are complete
	// Remember that each process gets the exact same amount of time based on the quantum and will not
	// use the full quantum if they do not need it to finish

	int burst_time[NUM_PROC], process[NUM_PROC] = {1, 2, 3, 4, 5}, time_remaining[NUM_PROC] = {0, 0, 0, 0, 0};
	int quantum, total_time = 0, timer = 1;

	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Please enter the burst time for Process %d: " , i + 1);
		scanf("%d", &burst_time[i]);
	}

	//setting time remaining to the burst times of the processes
	for (int i = 0; i < NUM_PROC; ++i) { time_remaining[i] = burst_time[i]; }
	
	//get Quantum time
	printf("Please enter the quantum time: ");
	scanf("%d", &quantum);

	printf("Process Data\n");
	printf("---------------------\n");

	//total time is all the burst times added
	for (int i = 0; i < NUM_PROC; ++i) {
		printf("Process: %d  Burst time: %d\n", i + 1, burst_time[i]);
		total_time += burst_time[i];
	}

	printf("Total Burst Time: %d\n", total_time);

	//Visual timing Process printing function
	while(1) {
		bool complete = true;
		for (int i = 0; i < NUM_PROC; ++i) {
			if (time_remaining[i] > 0) {
				complete = false;

				if (time_remaining[i] > quantum){
					for (int j = 0; j < quantum; ++j) {
						printf("%d: P%d\n", timer, process[i]);
						timer++;
					}

					time_remaining[i] = time_remaining[i] - quantum;
					printf("Timer is %d\n", timer);
				} else {
					for (int j = 0; j < time_remaining[i]; ++j) {
							printf("%d: P%d\n", timer, process[i]);
							timer++;
					}

					time_remaining[i] = 0;
					printf("Timer is %d\n", timer);
				}
			}
		}

		if (complete) { break; }
	}


}