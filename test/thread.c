//sebastian solorzano --prog71040 w25 -- assignment 2, question 2
//task:Declare a global variable int count= 50
//• The thread function increment() should increment the value of the global variable count by 1
//• You must ensure that a mutex lock protects the increment operation (of count)
//• The main program should create 100 threads using the increment() function and wait for all the threads
//to complete execution using the pthread_join() function. You need to declare an array of thread objects
//and implement pthread_create and pthread_join using loops.
//• After all the threads are done executing, the final value of count must be displayed from the main()

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define NUMBER_OF_THREADS 100


pthread_mutex_t mutlock; //creates mutex object(?)

//increments the global variable by 1;
//function pointer
void* increment(void * count){
	
	int* cn = (int*) count;
	printf("safe here: %d\n",*cn);
	pthread_mutex_lock(&mutlock); //activates a mutex lock when run
	
	*cn= *cn+1;
	
	pthread_mutex_unlock(&mutlock); //deactivates mutex lock
	printf("updated: %d\n",*cn);
}
	
int main(void){
	int count = 50; //global variable

	printf("count at start: %d\n",count);
	
	//initializes array
	pthread_t threadArray[NUMBER_OF_THREADS];
	
	pthread_mutex_init(&mutlock,NULL); //starts mutex in the main
	
	for(int i=0; i<100;i++){
		pthread_create(&threadArray[i],NULL,increment,&count);
	//	pthread_create(&threadArray[i],NULL,increment(&count),NULL); //creates a new thread for each element in the array as it loops
	}
	
	//joins back to main, only when finished
	for(int i=0; i<100;i++){
		pthread_join(threadArray[i],NULL); //same but for joining
	}
		
	pthread_mutex_destroy(&mutlock);//destroys the mutex

	printf("count at end: %d\n",count);
	return(0);
}
