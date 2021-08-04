/*
    Author: Jesus Minjares BSEE 
    Problem:
        Write a multi-threaded program that computes the Catalan number
        sequence (http://mathworld.wolfram.com/CatalanNumber.html) and 
        writes thosenumbers in fixed point format to a file called “catalan.dat”.
        Your program should take two command line arguments: the first specifies 
        the number of Catalan numbers to generate and the second specifies the
        number of threads to use to compute the Catalan number sequence; you can
        assume a maximum of four threads.Create a way to balance the load among
        the threads.You must use the “long double” type to get the largest range
        possible; the Catalan numbers grow very quickly. Output must be in ascending
        order (fixed point format) in the file “catalan.dat”
        -------Catalan Formula-----
        C_n+1 = 2*(2*n + 1)/(n+2) * Cn
        C_n+1 = (4n + 2)/(n+2) *Cn
        -------Catalan Formula with Factorial----
        Cn = (2*n)!/(n+1)!n!
        ---------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_THREADS 4 //max threads 
#define MIN_THREADS 1 //minimum thread 
void * printCatalanNumbers(void *param); //subroutine for print 
long double catalanNumber(int n); //catalan number function 
typedef struct Boundary{
    int high;
    int low;
    int threadNum;
}Boundary; //data structure that hold the bounds and thread number
int main(int argc, char *argv[]){
    if(argc < 3){ 
        printf("./executable nth threads\n");
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]); //convert array to integer 
    int numThreads = atoi(argv[2]); //convert array to integer 
    FILE *file = fopen("catalan.dat", "w"); //create a file, open a file as "write"
    if(file == NULL){
       fprintf(stderr, "Error opening the file\n");
       return EXIT_FAILURE;             
    }
    //set the thread at minimum of 1
    if(numThreads< MIN_THREADS) numThreads = MIN_THREADS; 
    //set the threads at max of 4
    if(numThreads > MAX_THREADS) numThreads = MAX_THREADS;
    
    int load = (int)n/numThreads; //get the increments
    //create an array of threads bounds 
    Boundary bound[numThreads];
    //distribute the load among the threads 
    for(int i = 0; i < numThreads; i++){
        bound[i].threadNum = i;
        bound[i].low = i * load;
        bound[i].high = (i + 1) * load;
    }
    //set the last thread.high at the nth number 
    bound[numThreads - 1].high = n;
     //create the threads, default attr, printCatalan function, and pass the structure 
    pthread_t thread[MAX_THREADS]; //pthread_t array of 4
    pthread_attr_t attr; //pthread default attribute
    pthread_attr_init(&attr);
    //create the threads 
    for(int i = 0; i  < numThreads; i++)
        pthread_create(&thread[i],&attr, printCatalanNumbers,(void*)&bound[i]);
    //wait for all threads to finish
    for(int i =0; i < numThreads; i++)
        pthread_join(thread[i], NULL);
    //combine all files into 1
    for(int i =0; i < numThreads; i++){
        char buffer[14];
        sprintf(buffer,"file%i.dat",i); //write to buffer file name
        FILE *threadFile = fopen(buffer,"r"); //open file name as read file 
        if(threadFile == NULL){
            fprintf(stderr, "Error opening the file\n");
            return EXIT_FAILURE; 
        }
        char c = fgetc(threadFile);  //get the character of the file 
        while (c != EOF) {  //iterate until End Of File 
            fputc(c, file); //write to file 
            c = fgetc(threadFile);  //get new character
        } 
        fclose(threadFile); //close the file 
    }
    fclose(file); //close the main file 
    printf("Catalan sequence stored in catalan.dat\n");
    return EXIT_SUCCESS;
}
void * printCatalanNumbers(void *param){
    Boundary *bound = param;//implicit type cast the param to a struct *
    //iterate from the low and high of the sequence
    char buffer[14]; //buffer to set the thread filename
    sprintf(buffer,"file%i.dat",bound->threadNum);//set thread file name
    FILE *threadFile = fopen(buffer,"w"); //create a write file for the thread 
    for(int i = bound->low; i < bound->high; i++)
        fprintf(threadFile,"%d: %Lf\n",i + 1,catalanNumber(i)); //print to the file 
    fclose(threadFile); //close the thread file 
    pthread_exit(NULL); //exit thread and return nothing 
}
/*
base case: n < 1, set 1
else: call recursive C(n-1)
NOTE: there will be a precision lost because of the division
*/
long double catalanNumber(int n){ 
     return n < 1 ? 1 : catalanNumber(n - 1) * (4*n + 2)  / (n + 2);
}