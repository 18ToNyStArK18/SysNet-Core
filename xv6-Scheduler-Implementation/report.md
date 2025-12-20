FCFS:
	It is the basic approach As it is first come first service we need store when a process arrived and once a process start we should not send traps to it so in trap.c you can see that i changed the trap calling yield for FCFS dont do anything 
	And also its Bad with respect to responce because of the convoy effect
ROUND ROBIN:
	Default option : Best in response time because multiple execute simultaneously but its trunaround time is very huge for everything because you are creating room for other process

CFS:
	Its the best of three it calculates the runtime based on the nicevalues and weight and and it will decide number_of_ticks each process gets (48/num_runnable) but min of 3 ticks and it will increase ticks in the trap.c and i am storing number_of_ticks it increases everytime when the usertrap is called and after that if the number_of_ticks it ran is = the numbe_of_ticks given to the program the yield is called as this process used cu and its vruntime increased ut willbe lower in the priority and the process that didnt run will le runned next
	And if the process is not cpu bounf like sleep or read/write then its vruntime will not be increased same job will be runned until its completion. 



# output i got for the given code as there is no sleep function i just changed it a bit
# CFS is Ai generated 

[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
[Scheduler Tick]
PID: 4 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 4 (lowest vRuntime)
Process 0 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 5 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 5 (lowest vRuntime)
Process 1 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
[Scheduler Tick]
PID: 6 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 6 (lowest vRuntime)
Process 2 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
[Scheduler Tick]
PID: 7 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 7 (lowest vRuntime)
Process 3 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
[Scheduler Tick]
PID: 8 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 8 (lowest vRuntime)
Process 4 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 0
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 9 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 900
PID: 10 | vRuntime: 0
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 10 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 900
PID: 10 | vRuntime: 900
PID: 11 | vRuntime: 0
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 11 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 900
PID: 10 | vRuntime: 900
PID: 11 | vRuntime: 900
PID: 12 | vRuntime: 0
PID: 13 | vRuntime: 0
--> Scheduling PID 12 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 900
PID: 10 | vRuntime: 900
PID: 11 | vRuntime: 900
PID: 12 | vRuntime: 900
PID: 13 | vRuntime: 0
--> Scheduling PID 13 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 900
PID: 10 | vRuntime: 900
PID: 11 | vRuntime: 900
PID: 12 | vRuntime: 900
PID: 13 | vRuntime: 900
--> Scheduling PID 9 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 1800
PID: 10 | vRuntime: 900
PID: 11 | vRuntime: 900
PID: 12 | vRuntime: 900
PID: 13 | vRuntime: 900
--> Scheduling PID 10 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 1800
PID: 10 | vRuntime: 1800
PID: 11 | vRuntime: 900
PID: 12 | vRuntime: 900
PID: 13 | vRuntime: 900
--> Scheduling PID 11 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 1800
PID: 10 | vRuntime: 1800
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 900
PID: 13 | vRuntime: 900
--> Scheduling PID 12 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 1800
PID: 10 | vRuntime: 1800
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 900
--> Scheduling PID 13 (lowest vRuntime)
[Scheduler Tick]
PID: 9 | vRuntime: 1800
PID: 10 | vRuntime: 1800
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 9 (lowest vRuntime)
Process 5 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 10 | vRuntime: 1800
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 10 | vRuntime: 1800
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 10 (lowest vRuntime)
Process 6 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 11 | vRuntime: 1800
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 11 (lowest vRuntime)
Process 7 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 12 | vRuntime: 1800
PID: 13 | vRuntime: 1800
--> Scheduling PID 12 (lowest vRuntime)
Process 8 finished
[Scheduler Tick]
PID: 3 | vRuntime: 0
PID: 13 | vRuntime: 1800
--> Scheduling PID 3 (lowest vRuntime)
[Scheduler Tick]
PID: 13 | vRuntime: 1800
--> Scheduling PID 13 (lowest vRuntime)
Process 9 finished

