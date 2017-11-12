#include <stdio.h>                     // Preprocessor directive, fuction prototypes
#include <stdlib.h>                    // For performing general functions, 4 variable
                                       // types and several macros                                       
#include <unistd.h>                                          
#include <string.h>                    // For using strings
#include <sys/types.h>                 // Used for system calls
#include <fcntl.h>                     // Performs lock functions on open files
#include <iostream>
#include <iomanip>                     // For hex
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>     
#include <cstdlib>    
#include <pthread.h>                   // Creates a new thread and makes it executable 
#include <thread>
#include <mutex>                       // Used for creating locks
#include <time.h> 

using namespace std;

FILE * file;

struct processer
{
char name[16];
 int32_t id;
 int32_t number_processes=0;
 int32_t total_mem_allocated=0;
 char  status;
 int32_t burst_time;
 int32_t base_register;
 int64_t limit_register;
 char priority; 

} p;

//sleep(1);
int term()
{
	file= fopen("processes.bin", "r");
	if(!file)
	{
		perror("file could not be found");
		exit(-1);
	}
	while(!feof(file))
	{
		fseek(file, 21, SEEK_CUR);
		fread(&p.burst_time, 4, 1, file);
		if(p.burst_time!=0)
		{
			fclose(file);
			return 1;
		}
		fseek(file, 12, SEEK_CUR);
		fread(&p.priority, 1, 1, file);  // just to detect end of file

	}
	fclose(file);
	return 0;



}
int main(int argc, char *argv[])
{


	file= fopen("processes.bin", "r");
	if(!file)
	{
		perror("file could not be found");
		exit(-1);
	}
	while(!feof(file))
	{
		fread(&p.name , 16, 1 , file);
			if(!feof(file))
			{
				fread(&p.id , 4, 1 , file);
				fread(&p.status, 1, 1 , file);
				fread(&p.burst_time , 4, 1 , file);
				fread(&p.base_register, 4, 1 , file);
				fread(&p.limit_register , 8, 1 , file);
				p.total_mem_allocated=p.total_mem_allocated + p.limit_register - p.base_register;
				fread(&p.priority , 1, 1 , file); 
				p.number_processes++;
			}
	}

//reads the file size and the total memory alocated 
cout<< "Total Memory allocated :" <<p.total_mem_allocated<<endl;
cout<< "Number of processes in FILE :" << p.number_processes<<endl;

//processer  * buff = processer[101]; 




while (term()==1)
{
	file = fopen("processes.bin" , "rb+");
	if(!file)
	{
		perror("file could not be found");
		exit(-1);
	}
	int count, quantum=0;
	
	while(quantum<30)  //simulate round robin
	{
		//sleep(1);
	    count=0;
		fseek(file, 0, SEEK_SET);

		while(!feof(file) && quantum<30)
		{
			memset(p.name, 0, sizeof(p.name));
			fread(&p.name , 16, 1 , file);
			if(!feof(file))
			{
				//This part of the code handels the Round Robin
				printf("*********************************************\n");
				printf("Round Robin\n");
				printf("*********************************************\n");
				cout<<"process name : " <<p.name <<endl;
				fread(&p.id , 4, 1 , file);
				cout<<"pid : "<<  p.id <<endl;
				fread(&p.status, 1, 1 , file);
				cout<<"status: "<<  p.status <<endl;
				fread(&p.burst_time , 4, 1 , file);
				cout<<"burst time : "<<  p.burst_time <<endl;
				if(p.burst_time!=0)
				{
					p.burst_time--;
					fseek(file, -5, SEEK_CUR);
					fputs("e", file);
					fwrite(&p.burst_time, 4, 1, file);
					fseek(file, -5, SEEK_CUR);
					fputs("w", file);
					if (p.burst_time==0)
					{
						fseek(file, -1, SEEK_CUR);
						fputs("t", file);
					}	
					fseek(file, 4, SEEK_CUR);
					quantum++;
				}
				fread(&p.base_register, 4, 1 , file);
				cout<<"base register : "<<  p.base_register <<endl;
				fread(&p.limit_register , 8, 1 , file);
				cout<<"limit register: "<< p.limit_register <<endl;
				fread(&p.priority , 1, 1 , file);
				cout<<"priority: "<< (int)p.priority <<endl;
				cout<<endl;
		//buff[count] = p; 
				count++;
				cout<<"count"<<count<<endl;
		 	}

			
		}
		fclose(file);
		if(term()==0)
		{
			printf("simulation terminated. Program is exiting. \n\n");
			exit(0);
		}
		file = fopen("processes.bin" , "rb+");
		if(!file)
		{
			perror("file could not be found");
			exit(-1);
		}	

    }
    
    int count1=0, offset1, offset2;
    //this part of the code handels the priority scheduling
    //and handels againg 
    //sleep(1);
    while(quantum<60) //simulate priority scheduling
    {	printf("*********************************************\n");
    	printf("Priority scheduling\n");
    	printf("*********************************************\n");
    	fseek(file, 0, SEEK_SET);
    	char highest_priority;
    	int offset;
    	count=0;
    	while(!feof(file))
    	{
    		fseek(file, 21, SEEK_CUR);
    		fread(&p.burst_time,4 ,1, file);
    		if(p.burst_time!=0)
    		{
    			fseek(file, 12, SEEK_CUR);
    			fread(&p.priority,1 ,1 ,file);
    			if (count==0)
    			{
    				highest_priority=p.priority;
    				count++;
    				offset=ftell(file)-1;
    			}
    			else if(p.priority<highest_priority)
    			{
    				highest_priority = p.priority;
    				offset= ftell(file)-1;
    			}

    		}
    		else
    		{
    			fseek(file, 12, SEEK_CUR);
    			fread(&p.priority, 1, 1, file);
    		}
    	}
    	if (count==0)
    	{
    		printf("\nsimulation terminated. Program exiting.\n\n");
    		exit(0);
    	}
    	else
    	{
    		fseek(file, offset-37, SEEK_SET);
    		fread(&p.name , 16, 1 , file);
			cout<<"process name : " <<p.name <<endl;
			fread(&p.id , 4, 1 , file);
			cout<<"pid : "<<  p.id <<endl;
			fread(&p.status, 1, 1 , file);
			cout<<"status: "<<  p.status <<endl;
			fread(&p.burst_time , 4, 1 , file);
			cout<<"burst time : "<<  p.burst_time <<endl;
			p.burst_time--;
			fseek(file, -5, SEEK_CUR);
			fputs("e", file);
			fwrite(&p.burst_time, 4, 1, file);
			fseek(file, -5, SEEK_CUR);
			fputs("w", file);
			if (p.burst_time==1)
			{
				fseek(file, -1, SEEK_CUR);
				fputs("t", file);
			}	
			fseek(file, 4, SEEK_CUR);
			quantum++;
			fread(&p.base_register, 4, 1 , file);
			cout<<"base register : "<<  p.base_register <<endl;
			fread(&p.limit_register , 8, 1 , file);
			cout<<"limit register: "<< p.limit_register <<endl;
			fread(&p.priority , 1, 1 , file);
			cout<<"priority : "<< (int)p.priority <<endl;
			cout<<endl;
    	}
    	if(count1==0)
    	{
    		offset1=offset;
    		count1++;
    	}
    	else
    	{
    		offset2=offset;
    		count1=0;
    		fseek(file, 0, SEEK_SET);
    		while(!feof(file))
    		{
    			fseek(file, 37, SEEK_CUR);
    			fread(&p.priority, 1, 1, file);
    			if(!feof(file) && ftell(file)-1!=offset1 && ftell(file)-1!=offset2 )
    			{	
    				p.priority--;
    				fseek(file, -1, SEEK_CUR);
    				fwrite(&p.priority, 1, 1, file);
    			}

    		}
    	}

  	}

    fclose(file);
}

}