#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
#include <cstdlib>
#include <stdint.h>
#include <cstdint>

using namespace std;

#define NUM_THREADS 5
#define MEMORY_SIZE 1000

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size

int index = 0;

bool release = false;

void release_function()
{
	//This function will be called
	//whenever the memory is no longer needed.
	//It will kill all the threads and deallocate all the data structures.

	release = true;
	return;
}

void my_malloc(int thread_id, int size)
{
	pthread_mutex_lock(&sharedLock);
	//This function will add the struct to the queue
	node *k = new node;
	k->id = thread_id;
	k->size = size;
	myqueue.push(*k);	
	pthread_mutex_unlock(&sharedLock);
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size
	while (!release)
	{

		if (myqueue.size() != 0) // there is an item in the queue
		{
			int tid = myqueue.front().id;
			int size = myqueue.front().size;
			if ( size + index  <=  MEMORY_SIZE  ) // if there are enough memory
			{

				thread_message[tid] = index;
				index += size;
				//grant access
			}

			else // there are not enough memoryas
			{
				thread_message[tid] = -1;
				// decline access
			}
			myqueue.pop();
			sem_post(&semlist[tid]); // semaphore up ??
		}

	}


}

void * thread_function(void * id) 
{
	int * tid = (int*) id;
	int size = rand() % MEMORY_SIZE/3;// so that size is between 0-333
	my_malloc(*tid,size); //This function will create a random size, and call my_malloc

	sem_wait(&semlist[*tid]); // semaphore down ???       //Block


	//Then fill the memory with 1's or give an error prompt

	if (thread_message[*tid] == -1 ) // eðer -1 geldiyse error
	{
		printf( "Thread ID: %d Not enough memory\n", *tid);
	}

	else if (thread_message[*tid] != -1 )
	{
		// alýnan yerleri 1 yap
		pthread_mutex_lock(&sharedLock);
		for (int i = thread_message[*tid] ; i < size + thread_message[*tid] ; i++)
		{
			memory[i] = '1';
		}
		pthread_mutex_unlock(&sharedLock);
	}

}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
	{char zero = '0'; memory[i] = zero;}
	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}

void dump_memory() 
{
	// You need to print the whole memory array here.
	cout << "Memory Dump:\n";
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		cout << memory[i];
	}
}

int main (int argc, char *argv[])
{

	//You need to create a thread ID array here
	int thread_id[5];
	for (int i = 0; i < 5; i++)
	{
		thread_id[i] = i ;
	}

	init();	// call init

	pthread_t thread1, thread2, thread3, thread4, thread5;

	pthread_create(&thread1,NULL, thread_function,(void*) &thread_id[0]);
	pthread_create(&thread2,NULL, thread_function,(void*) &thread_id[1]);
	pthread_create(&thread3,NULL, thread_function,(void*) &thread_id[2]);
	pthread_create(&thread4,NULL, thread_function,(void*) &thread_id[3]);
	pthread_create(&thread5,NULL, thread_function,(void*) &thread_id[4]);
	//You need to create threads with using thread ID array, using pthread_create()

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	pthread_join(thread4,NULL);
	pthread_join(thread5,NULL);

	//You need to join the threads

	dump_memory(); // this will print out the memory
	printf("\nMemory Indexes:\n" );
	for (int i = 0; i < NUM_THREADS; i++)
	{
		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
	}
	printf("\nTerminating...\n");
}