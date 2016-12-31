#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

/*
	This file represents a series of tests on our implementation of
	malloc and free
*/


/*
	Returns a random integer with the range of min and max
*/
int rand_lim(int min, int max){
   
    return ((double)rand() / (((double)RAND_MAX) + 1.0)) * (max-min+1) + min;
}
/*
	Removes the pointer at index and shifts the values in the array up to
	fill the vacancy.
*/
void deleteArrayItem( int index, int length,  char ** array){
    int i; 
    for (i = index; i < length - 1; i++ ){
         array[i] = array[i + 1];
    }
}

/*
	All tests below are run 100 times per call
*/

/*
	Calls malloc for 1 byte 3000 times. The pointers are stored in an
	array and then freed in the order that they were allocated.
*/
void test1(){
    char * op[5000];
    struct timeval tv1;
    struct timeval tv2;
    long avg=0;
    int successCounter = 0;
    int currentSuccessCount = 0;
    int freeCounter = 0;
    int totalMallocs = 0;
    int totalFrees = 0;
    printf("TEST 1, malloc 1 byte 3000 times:\n");
    //time_t begin;
    //time_t currtime;
    int i;
    int y;
    for (y=0; y<100; y++) {
        gettimeofday(&tv1, 0);
        for (i=0; i<3000; i++) {
            op[i] = malloc(1);
            totalMallocs++;
            
            if (op[i] != NULL){
            	successCounter++;
                currentSuccessCount++;
            }
        }
        for (i = 0; i < currentSuccessCount; i++) {
            free(op[i]);
            freeCounter++;
            totalFrees++;
        }
        currentSuccessCount = 0;
         gettimeofday(&tv2, 0);
        avg += ((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
    }
    printf("Total mallocs: %d\n", totalMallocs);
    printf("Successful mallocs: %d\n", successCounter);
    printf("Total frees: %d\n", totalFrees);

    printf("AVG Time Elasped: %ld microseconds.\n", avg/100);

     char * test = malloc(4998);
    if (test != NULL){
    	printf("TEST PASSED\n");
    }
    free(test);
}

/*
	Calls malloc for 1 byte and then frees the pointer immediately 3000 times.
*/
void test2(){
    printf("TEST 2, malloc 1 byte 3000 times and free immediately:\n");
    char * ptr = NULL;
    int i;
    int y;
    int successCounter = 0;
    int freeCounter = 0;
    int totalMallocs = 0;
    int totalFrees = 0;
    long avg= 0;
    struct timeval tv1;
    struct timeval tv2;
    //time_t begin;
    //time_t currtime;
    for (y=0; y<100; y++) {
    
        gettimeofday(&tv1, 0);
        for (i=0; i<3000; i++) {
            ptr = malloc(1);
            totalMallocs++;
            if (ptr != NULL){
                 free(ptr);
                 successCounter++;
                 freeCounter++;
                 totalFrees++;
            }
        }
        gettimeofday(&tv2, 0);
        avg+=((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
    }
    char * test = malloc(4998);
    printf("Total mallocs: %d\n", totalMallocs);
    printf("Successful mallocs: %d\n", successCounter);
    printf("Total frees: %d\n", totalFrees);
    printf("Successful frees: %d\n", freeCounter);
    printf("AVG Time Elasped: %ld microseconds.\n", avg/100);
    if (test != NULL){
    	printf("TEST PASSED\n");
    }
    free(test);
}

/*
	Randomly choose between a malloc of 1 byte or a free(of a random pointer allocated). 
	Once 3000 malloc calls have occurred the function stops calling malloc and frees 
	any remaining pointers stored in an array randomly. If there are no pointers in the 
	array when free is called then it tries to free a garbage value which myfree will
	catch with an error message.
*/
void test3(){
    //int i;
    int mallocCounter = 0;
    int freeCounter = 0;
    int totalMallocs = 0;
    int successCounter = 0;
    char * pointers[3000];
    int length = 0;
    long avg= 0;
    
    struct timeval tv1;
    struct timeval tv2;
    //time_t t;
    //time_t begin;
    //time_t currtime;
    printf("TEST 3, Randomly choose between a malloc() of size 1 or free() 6000 times:\n");
    //srand((unsigned)time(&t));

    int y;
    for (y = 0; y < 100; y++){
    	gettimeofday(&tv1, 0);
        while(mallocCounter < 3000){
            int pick = rand_lim(0, 1);
            if (pick == 0){
                int size = 1;       //random size
                void * ptr = malloc(size);
                totalMallocs++;
                if (ptr != NULL){
                    //printf("pointer is stored\n");
                    pointers[length] = ptr;
                    length++;
                    //printf("length: %d\n", length);
                    successCounter++;
                }
                mallocCounter++;
               
            }
            else {
                //free random pointer 
                if (length > 0){
                	int index = rand_lim(0, length - 1);
                	free(pointers[index]);
                	char ** start = &pointers[0];
                   	deleteArrayItem(index, length, start);
                    length--;
                    freeCounter++;
                }

            }
        
        }
   //if not enough frees called
        while (length > 0){
            int index = rand_lim(0, length - 1);
            free(pointers[index]);
            char ** start = &pointers[0];
            deleteArrayItem(index, length, start);
            length--;
            freeCounter++;
            //printf("length: %d\n", length);
        }
        mallocCounter = 0;  
        gettimeofday(&tv2, 0);
        avg += ((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
    }
    
    printf("Total mallocs: %d\n", totalMallocs);
    printf("Successful mallocs: %d\n", successCounter);
    printf("Frees: %d\n", freeCounter);
    printf("Avg Time Elasped: %ld microseconds.\n", avg/100);

    char * test = malloc(4998);
    if (test != NULL){
    	printf("TEST PASSED\n");
    }
    free(test);
}

/*
	Randomly choose between a malloc of random size or a free(of a random pointer allocated). 
	Once 3000 malloc calls have occurred the function stops calling malloc and frees 
	any remaining pointers stored in an array randomly. If there are no pointers in the 
	array when free is called then it tries to free a garbage value which myfree will
	catch with an error message.
*/
void test4(){
    //int i;
    int mallocCounter = 0;
    int freeCounter = 0;
    int successCounter = 0;
    int totalMallocs = 0;
    char * pointers[3000];
    int length = 0;
    long avg = 0;
    struct timeval tv1;
    struct timeval tv2;
    //time_t t;
    //time_t begin;
    //time_t currtime;
    printf("TEST 4, Randomly choose between a randomly sized malloc() or free() 6000 times:\n");
    //srand((unsigned)time(&t));

    int y;
    for (y = 0; y < 100; y++){
    	gettimeofday(&tv1, 0);
        while(mallocCounter < 3000){
            int pick = rand_lim(0, 1);
            if (pick == 0){
                int size = rand_lim(1, 4998);       //random size
                void * ptr = malloc(size);
                totalMallocs++;
                if (ptr != NULL){
                    //printf("pointer is stored\n");
                    pointers[length] = ptr;
                    length++;
                    //printf("length: %d\n", length);
                    successCounter++;
                }
                mallocCounter++;
               
            }
            else {
                //free random pointer 
                if (length > 0){
                	int index = rand_lim(0, length - 1);
                	free(pointers[index]);
                	char ** start = &pointers[0];
                   	deleteArrayItem(index, length, start);
                    length--;
                    freeCounter++;
                }

            }
        
        }
   //if not enough frees called
        while (length > 0){
            int index = rand_lim(0, length - 1);
            free(pointers[index]);
            char ** start = &pointers[0];
            deleteArrayItem(index, length, start);
            length--;
            freeCounter++;
            //printf("length: %d\n", length);
        }    
        mallocCounter = 0;
        gettimeofday(&tv2, 0);
        avg += ((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
    }
    
   
    printf("Total mallocs: %d\n", totalMallocs);
    printf("Successful mallocs: %d\n", successCounter);
    printf("Frees: %d\n", freeCounter);
    printf("Avg Time Elasped: %ld microseconds.\n", avg/100);
    

     char * test = malloc(4998);
    if (test != NULL){
    	printf("TEST PASSED\n");
    }
    free(test);
}

/*
	Calls malloc repeatedly of increasing size and frees the pointer immediately.
*/
void test5(){
    int i;
    int y;
    int z =1;
    int freeCounter = 0;
    int successCounter = 0;
    int totalMallocs = 0;
    long avg = 0;
    char * ptr = NULL;
    struct timeval tv1;
    struct timeval tv2;
    printf("TEST 5, call malloc of increasing size starting at 1 and incrementing by 1. 50 iterations\n");
    //time_t begin;
    //time_t currtime;
    for (y=0; y<100; y++) {
        
        gettimeofday(&tv1, 0);
        
        for (i = 0; i<50;i++){
            z++;
            ptr = malloc(z);
            totalMallocs++;
            if (ptr != NULL){
                successCounter++;
                free(ptr);
                freeCounter++;
            }
        
        }
        z = 0;
        
        gettimeofday(&tv2, 0);
        avg+=((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
    }
    printf("Total mallocs: %d\n", totalMallocs);
    printf("Successful mallocs: %d\n", successCounter);
    printf("Frees: %d\n", freeCounter);
    printf("AVG Time Elasped: %ld microseconds.\nTEST PASSED\n", avg/100);
    //printf("mallocs: %d\n", mallocCounter);
    //printf("frees: %d\n", freeCounter);
}

/*
	Calls malloc with pointers of various primitive type, assigns them a random value and
	then prints the values to show that the information is being stored in the array.
*/
void test6(){
    int y;
    long avg = 0;
    void * ptr;
    struct timeval tv1;
    struct timeval tv2;
    printf("TEST 6, Malloc various primitive type pointers dereference with values and use the printf function with them:");
    //time_t begin;
    //time_t currtime;
    for (y=0; y<100; y++) {
        gettimeofday(&tv1, 0);
        printf("\n");
        ptr=(int*)malloc(sizeof(int));
        *(int*)ptr = rand_lim(0,100);
        printf("int = %d\n",*(int*)ptr);
        free(ptr);
        
        ptr=(char*)malloc(sizeof(char));
        *(char*)ptr = (rand_lim(65,90));
        printf("char = %c\n",*(char*)ptr);
        free(ptr);
        
        ptr=(long*)malloc(sizeof(long));
        *(long*)ptr = rand_lim(0,1000);
        printf("long = %ld\n",*(long*)ptr);
        free(ptr);
        
        ptr=(long int*)malloc(sizeof(long int));
        *(long int*)ptr = rand_lim(0,1000);
        printf("long int = %ld\n",*(long int*)ptr);
        free(ptr);
        
        ptr=(double*)malloc(sizeof(double));
        *(double*)ptr = rand_lim(0,5000);
        printf("double = %f\n",(*(double*)ptr)/rand_lim(1, 99));
        free(ptr);
        
        ptr=(long double*)malloc(sizeof(long double));
        *(long double*)ptr = rand_lim(0,5000);
        printf("long double = %Lf\n",(*(long double*)ptr)/rand_lim(1, 99));
        free(ptr);
        printf("\n");
        gettimeofday(&tv2, 0);
        avg+=((tv2.tv_sec-tv1.tv_sec)*1000000 + tv2.tv_usec-tv1.tv_usec);
    }
    printf("AVG Time Elasped: %ld microseconds.\nTEST PASSED\n", avg/100);
    //printf("mallocs: %d\n", mallocCounter);
    //printf("frees: %d\n", freeCounter);
}

/*
	These are the examples given in the assignment instructions.
*/
void bonusTest(){

	printf("\nThese tests produce the output to the examples in the assignment\ninstructions except for part of case B (see note).\n");
	//A
	printf("\nA. Free()ing addresses that are not pointers:\n");
	int x;
	free(&x);

	//B
	printf("\nB. Free()ing pointers that were not allocated by malloc():\n(Note) We removed the test to check for freeing uninitialized pointers \nbecause of the compiler warning caused by the -Wall tag. Our\nmyfree functionwill detect this however if you want to test it yourself.\n\n");
	char * p;
	p = (char *)malloc(200);
	free(p + 10);
	free(p);
	//int * z;
	//free(z);

	//C
	printf("\nC. Redundant free()ing of the same pointer:\n");
	p = (char*)malloc(100);
	free(p);
	free(p);

	p = (char *)malloc(100);
	free( p );
	p = (char *)malloc(100);
	free(p);

	//D
	printf("\nD. Saturation of dynamic memory :\n");
	p = (char*)malloc(4999);
	
	char * q;
	p = (char*)malloc(4998);
	q = (char*)malloc(1);

	free(p);
	//to suppress compiler warning
	p = q;

	
}

/*
	Runs our test cases that test mymalloc and myfree
*/
int main(int argc, char ** argv){

	if (argc != 1){
		printf("Correct usage: \n./memgrind\n");

	}
	else {
		
	    time_t t;
	    srand((unsigned)time(&t));
	    printf("Each test is performed 100 times\n");
	    printf("\nTEST1...\n");
	    test1();
	    printf("\nTEST2...\n");
	    test2();
	    printf("\nTEST3...\n");
	    test3();
	    printf("\nTEST4...\n");
	    test4();
	    printf("\nTEST5...\n");
	    test5();
	    printf("\nTEST6...\n");
	    test6();
	    printf("\nLastly, Bonus Test\n");
	    bonusTest();
	    printf("\n\n");
	}

    return 0; 
    
}
