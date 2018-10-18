# Share-memory
Share different data using shared memory, insert 30 different numbers to an array, and receive it sorted!



<---------------- Shared memory #1 ---------------->

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <signal.h>

#include <fcntl.h>

#include <sys/stat.h>

#include <sys/types.h>  

#include <sys/ipc.h>  

#include <sys/shm.h>

#define SIZE 31

int main()

{
	key_t key; 
	int shm_id;
	int *shm_ptr;
	
	if ((key = ftok("/tmp", 'y')) == -1)  
	{   
		perror("ftok() failed");   
		exit(EXIT_FAILURE);
	}
		

	shm_id = shmget(key, SIZE*sizeof(int), IPC_CREAT | IPC_EXCL | 0600 );
	if (shm_id == -1)  
	{			
		perror("Failed to get memory");   
		exit(EXIT_FAILURE);
	}
	
	
	shm_ptr = (int *) shmat ( shm_id, NULL, 0);
	if (shm_ptr == (int *) -1)
	{
		perror( "shmat failed" ) ;
		exit( EXIT_FAILURE ) ;
	}

	printf("Please insert 30 numbers you like- \n");
	
	for(int i=0; i<SIZE-1; i++)
	{
		scanf("%d", &shm_ptr[i]);
	}
	
	shmdt(shm_ptr);
	return 0;
}



<---------------- Shared memory #2 ---------------->

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <signal.h>

#include <fcntl.h>

#include <sys/wait.h>

#include <sys/stat.h>

#include <sys/types.h>  

#include <sys/ipc.h>  

#include <sys/shm.h>

#define SIZE 31


int main()

{
	key_t key;
	int sum = 0;
	int shm_id;
    int *shm_ptr;


	key = ftok("/tmp", 'y');

	shm_id = shmget( key, 0 , 0600 );
	if (shm_id == -1)  
	{
		perror("Failed to get memory");   
		exit(EXIT_FAILURE);
	}
    shm_ptr = (int*) shmat(shm_id,NULL,0);

	pid_t process1, process2, process3;

	process1 = fork();

	if (process1 < 0)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}

	

	if (process1 == 0)

	{
		for(int i = 0; i< 10; i++)
		{
			shm_ptr[SIZE-1] = shm_ptr[i]+shm_ptr[SIZE-1];
		}
	    exit(1);
	}
	

	process2 = fork();

	if (process2 < 0)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}


	if (process2 == 0)
	{
		for(int i = 10; i< 20; i++)
		{
			shm_ptr[SIZE-1] = shm_ptr[i]+shm_ptr[SIZE-1];
		}
    	exit(1);
	}

	process3 = fork();

	if (process3 < 0)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}


	if (process3 == 0)
	{
		for(int i = 20; i< 30; i++)
		{
			shm_ptr[SIZE-1] = shm_ptr[i]+shm_ptr[SIZE-1];
		}
    	exit(1);
	}

	
	wait(NULL);
	wait(NULL);
	wait(NULL);

	printf("The sum is: %d", shm_ptr[SIZE-1]);

	

	shmdt(shm_ptr);

	shmctl(shm_id,IPC_RMID, NULL);

	

	return 0;

}
