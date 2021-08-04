/*
    Author: Jesus Minjares BSEE 
        -------Catalan Formula-----
        C_n+1 = 2*(2*n + 1)/(n+2) * Cn
        C_n+1 = (4n + 2)/(n+2) *Cn
        -------Catalan Formula with Factorial----
        Cn = (2*n)!/(n+1)!n!
        ---------------------------

        3 threads 
        15 catalan numbers 
        0: 1 4 7 10 13
        1: 2 5 8 11 14
        2: 3 6 9 12 15
        for(int i = threadNum; i < nthNumber; i = i + totalThreads)
*/
#include <stdlib.h> 
#include <stdio.h>
#include <pthread.h>
#define MAXSIZE 8196 //the max number catalan number 
void *printCatalan(void *); //thread subroutine 
typedef struct Data{
    int threadNum;
    int numOfThreads;
    int nthNumber;
}Data; //data structure for the threads 
pthread_mutex_t lock;  //mutex lock 
long double *array = NULL; //create a global array 
long double catalanNumber(int n); //catalan number sequence function 
int main(int argc, char **argv){ 
     if(argc < 3){  //check if it received 3 arguments 
        printf("Usage: ./executable nth threads\n");
        return EXIT_FAILURE;
    }
    //save the 2 argument to the nth variable 
    int nth = atoi(argv[1]);
    //check the nth number
    if(nth <0)  nth = 0;
    //set the max nth to the MAXSIZE
    if(nth > MAXSIZE) nth = MAXSIZE;
    //store the last argument to the numThread
    int numThreads = atoi(argv[2]);
    //only accept threads > 0
    if(numThreads < 0)  numThreads = 1;
    //allocate pthreads of size numThreads
    pthread_t *tasks = malloc(sizeof(pthread_t)*numThreads);
    pthread_attr_t attr; //attribute 
    pthread_attr_init(&attr); //initialize atttribute to default 
    pthread_mutex_init(&lock, NULL); //initialize the mutex 

    //allocate the nth size of array for the catalan numbers
    array = (long double *)malloc(sizeof(long double)*nth);
   
    //allocate the data structure for the threads 
    Data *threadsData = (Data*)malloc(sizeof(Data)*numThreads);

     //print message 
    printf("Allocating an catalanArray, threads, and data structure in the heap\n");

    //load the data structure with the correct values 
    for(int i = 0; i < numThreads; i++){
        threadsData[i].threadNum = i; //set thread num 
        threadsData[i].nthNumber = nth; //set the nth number 
        threadsData[i].numOfThreads = numThreads; //set the total threads 
        //create the thread
        pthread_create(&tasks[i], &attr, printCatalan,(void*)&threadsData[i]);
    }
    //wait for all threads to finish 
    for(int i =0; i < numThreads; i++)
        pthread_join(tasks[i], NULL);

   //create a file name "catalan.dat" as a write file 
   FILE *file = fopen("catalan.dat", "w");
   //iterate over the array 
    for(int i =0; i < nth;i++)
        fprintf(file,"%d: %Lf\n", i+1, array[i]); //print to the file 
    fclose(file);  //close the file object 
    printf("Catalan sequence locate at catalan.dat\n");
    //destory the lock and  attr 
    pthread_mutex_destroy(&lock);
    pthread_attr_destroy(&attr);
    //free the heap memory 
    free(array);
    free(threadsData);
    free(tasks);
    //print message
    printf("Deleting dynamic allocation.\n");
    return EXIT_SUCCESS;
}
//thread subroutine 
void *printCatalan(void *param){
   Data *currThread = param; //point to the data structure 
   //iterate over the thread num, i < nthNumber, and step of the total threads
   for(int i=currThread->threadNum; i < currThread->nthNumber; i+= currThread->numOfThreads){
       pthread_mutex_lock(&lock);  //protect the critical section 
       array[i] = catalanNumber(i); //save to the array 
       pthread_mutex_unlock(&lock);//release the critical section
   } 
    pthread_exit(0); //exit the thread
}

long double catalanNumber(int n){ 
    return n < 1 ? 1 : catalanNumber(n - 1) * (4*n + 2)  / (n + 2);
}
//Driver thread to test the sweep among the threads
#if 0
void *printCatalan(void *param){
   Data *currThread = param;
   char buffer[14];
   sprintf(buffer, "file%d.dat", currThread->threadNum);
   FILE *file = fopen(buffer,"w");
   for(int i=currThread->threadNum; i < currThread->nthNumber; i+= currThread->numOfThreads){
       pthread_mutex_lock(&lock); 
       array[i] = catalanNumber(i);
       fprintf(file,"%d: %Lf\n", i, array[i]);
       pthread_mutex_unlock(&lock);
   } 
    fclose(file);
    pthread_exit(0);
}
#endif 
