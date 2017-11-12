Given a binary file containing PCB (Process Control Block) information about multiple processes –written in sequential order, simulate a CPU scheduling using different scheduling algorithms. The time quantum is 1. After “executing” one process, the current status of each process should be written in the same file. The process should be repeated until each process is finished (no more burst time). The binary file is to be downloaded from Canvas. 

Overview  

PCB is a data structure holding information about processes, such as id, name, activity status, CPU burst time, priority, etc. This data descriptor is necessary for the CPU to know which process is running, what is the current state of the process, where the program is in its execution, etc 

Instructions 

 The binary file contains multiple PCB descriptors in sequential order. In our case it will be not a real PCB descriptor, but each process has the following fields:  
 
 
** Offset   		Type    		Value   	Description **
 

 0000     		16 char 	 	??   		process name

 0016   			32 bit int 	??   		process_id 
 
 0020   			1  char   	 ??   		activity status 
 
 0021   			32 bit int 	??   		CPU burst time 
 
 0025   			32 bit int 	??   		base register 
 	
 0029   			64 bit long   ??  		 limit register 
 
 0037   			1 char    	??   		priority 
 
 
 Notes 
 
  Read the binary file and print out the number of processes available in the file, and the total number of memory allocated by the processes – considering all the processes. 
 
  Each process is “executed” in according to the following algorithm: 
 
 1) For a time quantum of 30 Round Robin scheduling should be considered 
 
 2) For a time quantum of 30 Priority scheduling should be considered 
 
 	 During Priority scheduling after each 2 quantum apply during Priority aging with a value of 1 

3) If all processes “executed stop the simulation

 4) Otherwise GoTo 1  
 
	  When there is a change between the two scheduling algorithms, the scheduling should start from the beginning (first element in the file)!