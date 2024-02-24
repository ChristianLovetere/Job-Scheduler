# Job-Scheduler
This code uses linked lists to create a job scheduler data structure. The scheduler allows the user to add jobs or remove jobs with specific usernames, job names, hardware requirements, time taken, and job priority. 

These operations are stored as functions and can be accessed freely until the program is exited, at which point all of the jobs' allocated memory is cleared from the heap and the program is ended. The scheduler can also search jobs by username, or by the job's hardware requirements. The code adds jobs to the scheduler in order of priority.
