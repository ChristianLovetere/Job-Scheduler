//This code uses linked lists to create a job scheduler data structure. The scheduler allows the user to add jobs or remove jobs with specific usernames, job names, hardware requirements, time taken, and job priority. These operations are
//stored as functions and can be accessed freely until the program is exited, at which point all of the jobs' allocated memory is cleared from the heap and the program is ended. The scheduler can also search jobs by username, or by the
//job's hardware requirements.

//PROJECT 9: the code now adds jobs to the scheduler in order of priority, instead of time added.
//Christian Lovetere, U46489387

//Project 10: code is now split into two source files and a header. header contains function protos and the declaration for struct job. this source file contains main, and job.c contains the functions for list manipulation.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "job.h" //include header

///////////////////
// main function //
///////////////////

int main() {
	char code;
	char job_name[NAME_LEN+1], user_name[NAME_LEN+1];
	int num_cpus, num_gpus, memory, priority;
	double time;

	struct job *scheduler = NULL;

	help();
	printf("\n");

	for (;;) {
		// read operation code
		printf("Enter operation code: ");
		scanf(" %c", &code);
		while(getchar() != '\n') /* skips to end of line */
			;

		// run operation
		switch (code) {
			case 'h':
				help();
				break;
			case 'a':
				read(job_name, user_name, &num_cpus, &num_gpus, &memory, &time, &priority);
				scheduler = add_job(scheduler, job_name, user_name, num_cpus, num_gpus, memory, time, priority);
				break;
			case 'p':
				scheduler = pop_job(scheduler);
				break;
			case 'u':
				read(NULL, user_name, NULL, NULL, NULL, NULL, NULL);
				list_user(scheduler, user_name);
				break;
			case 'j':
				read(NULL, NULL, &num_cpus, &num_gpus, &memory, &time, NULL);
				list_jobs(scheduler, num_cpus, num_gpus, memory, time);
				break;
			case 'l':
				list_all_jobs(scheduler);
				break;
			case 'q':
				scheduler = clear_jobs(scheduler);
				return 0;
			default:
				printf("Illegal operation code!\n");
		}
		printf("\n");
	}
}
