#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void) {
	char *args[MAX_LINE / 2 + 1];

	char tokenCommand[MAX_LINE];
	int i = 0;

	while (1) {
		printf("osh> ");
		fflush(stdout);

		scanf("%[^\n]%*c", tokenCommand);
		printf("input:%s\n", tokenCommand);

		args[i] = strtok(tokenCommand, " ");

		while (args[i] != NULL) {
			args[++i] = strtok(NULL, " ");
		}

		if (strcmp(args[0], "exit") == 0) { break; }

		pid_t pid = fork();
		if (pid < 0) { fprintf(stderr, "FORK Failed\n"); return 1; }

		if (strcmp(args[i - 1], "&") != 0) {
			if (pid == 0) {
				execvp(args[0], args);
				for (int j = 0; j < i; args[++j] = NULL);
			} else {
				wait(NULL);
			}
		} else {
			if (pid == 0) {
				args[i - 1] = NULL;
				execvp(args[0], args);
			} else {
				printf("\n\n");
			}
		}
	}

	return 0;
}