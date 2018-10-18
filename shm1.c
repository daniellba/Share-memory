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
