#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "job.h" //include header

//////////////////////////
// function definitions //
//////////////////////////

void help() {
	printf("List of operation codes:\n");
	printf("\t'h' for help;\n");
	printf("\t'a' for adding a job to the scheduler;\n");
	printf("\t'p' for removing a job from the scheduler;\n");
	printf("\t'u' for searching jobs per user;\n");
	printf("\t'j' for searching jobs per capacity;\n");
	printf("\t'l' for listing all jobs;\n");
	printf("\t'q' for quit.\n");
}

void read(char *job_name, char *user_name, int *num_cpus, int *num_gpus, int *memory, double *time, int *priority) {
	if(job_name != NULL) {
		printf("Enter the name of the job: ");
		scanf("%s", job_name);
	}
	if(user_name != NULL) {
		printf("Enter the name of the user: ");
		scanf("%s", user_name);
	}
	if(num_cpus != NULL) {
		printf("Enter the number of CPUs: ");
		scanf("%d", num_cpus);
	}
	if(num_gpus != NULL) {
		printf("Enter the number of GPUs: ");
		scanf("%d", num_gpus);
	}
	if(memory != NULL) {
		printf("Enter the amount of memory: ");
		scanf("%d", memory);
	}
	if(time != NULL) {
		printf("Enter the amount of time: ");
		scanf("%lf", time);
	}
	if(priority != NULL) {
		printf("Enter the priority: ");
		scanf("%d", priority);
	}
}

struct job * add_job(struct job *scheduler, char *job_name, char *user_name, int num_cpus, int num_gpus, int memory, double time, int priority) { //adds a new job to the scheduler.

	struct job *new_job;
	new_job = malloc(sizeof(struct job)); //allocate memory.
	strcpy(new_job->job_name, job_name); //the following 8 lines (including this one) assign the inputted values to the new struct.
	strcpy(new_job->user_name, user_name);
	new_job->num_cpus = num_cpus;
	new_job->num_gpus = num_gpus;
	new_job->memory = memory;
	new_job->time = time;
	new_job->priority = priority;
	new_job->next = NULL;
	if (scheduler == NULL) //if the linked list was previously empty, simply return a linked list with this new job as the only link.
		return new_job;
	struct job *current, *previous; //initialize pointers
	for(previous = NULL, current = scheduler; current != NULL; previous = current, current = current->next) { //assign current pointer to the linked list, and find at which point the current operation's priority should be placed
		if (new_job->priority > current->priority)
			break;
	}
	if (previous == NULL) { //if we are at the first link, make the new_job->next point to the whole rest of the scheduler, and return the new_job with scheduler attached.
		new_job->next = scheduler;
		return new_job;
	}
	else { //if we are NOT at the first link, make the previous item link to the new job, and the new job link to the current job, placing the new job in between them.
		previous->next = new_job;
		new_job->next = current;
		return scheduler; //return appended scheduler.
	}
}

struct job * pop_job(struct job *scheduler) {//removes the first job.

	if(scheduler == NULL) //if scheduler is empty, quit out of this function.
		return NULL;

	struct job *p = scheduler->next; //otherwise, create a pointer to the second job

	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");//print the current job
	printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", scheduler->job_name, scheduler->user_name, scheduler->num_cpus, scheduler->num_gpus, scheduler->memory, scheduler->time, scheduler->priority);
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");

	free(scheduler); //free the current job from memory
	return p; //return a linked list that starts at what was previously the second job
}

void list_user(struct job *scheduler, char *user_name) { //lists a job if it has the username requested.

	int flag_print_header = 0, flag_is_header_printed = 0; //flags for printing the header: The first flag becomes 1 when a match is found, because if no matches are found, we won't need a header. The second header becomes 1 after the header is printed.
	if (scheduler != NULL) { //if the scheduler is empty, exit this function

		struct job *p;
		for(p = scheduler; p != NULL; p = p->next) { //check each job to see if it has a matching username.
			if (strcmp(p->user_name,user_name) == 0) { //if names match
				flag_print_header = 1; //once we have found our first match, we need to print the header before printing that match's info.
				if (flag_print_header == 1 && flag_is_header_printed == 0) {// if we found a match but have not yet printed the header, print the header.
					printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
					printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
					printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
					flag_is_header_printed = 1; //this marks that the header has already been printed. Without this flag, the header would print every time a match is found, instead of before the first match.
				}
				printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", p->job_name, p->user_name, p->num_cpus, p->num_gpus, p->memory, p->time, p->priority);//print the match's info.
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
			}
		}
	}
}

void list_jobs(struct job *scheduler, int num_cpus, int num_gpus, int memory, double time) {//list the jobs with less or equal requirements for hardware (cpu, gpu, mem, time)

	int flag_print_header = 0, flag_is_header_printed = 0; //see comments on function above for how the flags work.
	if (scheduler != NULL) { //if the scheduler is empty, exit this function

		struct job *p;
		for(p = scheduler; p != NULL; p = p->next) {//check each job for matching hardware req.
			if (p->num_cpus <= num_cpus && p->num_gpus <= num_gpus && p->memory <= memory && p->time <= time) {
				flag_print_header = 1;
				if (flag_print_header == 1 && flag_is_header_printed == 0) {
					printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
					printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
					printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
					flag_is_header_printed = 1;
				}
				printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", p->job_name, p->user_name, p->num_cpus, p->num_gpus, p->memory, p->time, p->priority);//print match's info
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
			}
		}
	}
}

void list_all_jobs(struct job *scheduler) {//prints the header, and then all jobs.

	if(scheduler == NULL)
		return;
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	struct job *p;
	for(p = scheduler; p != NULL; p= p->next) {//print each job
		printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", p->job_name, p->user_name, p->num_cpus, p->num_gpus, p->memory, p->time, p->priority);
		printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	}
}

struct job * clear_jobs(struct job *scheduler) {//clears all jobs.

	if(scheduler == NULL) //if the scheduler is already empty, exit the function.
		return NULL;

	if(scheduler->next == NULL) { //if the scheduler has one job, clear it and exit the function.
		free(scheduler);
		return NULL;
	}

	struct job *p = scheduler->next; //pointer p points to the next link

	free(scheduler); //clear the link before p, p is now the first link
	return clear_jobs(p); //recursively call the function with the new first link

}
