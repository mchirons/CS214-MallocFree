#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mymalloc.h"

/*
    This file represents methods that implement simplified versions of 
    malloc(size_t) and free() that operations on a static char array of 
    size 5000. These versions do not allow certain programming mistakes
    to occur and print messages informing the user of their infraction.
*/


static char memory[5000];

int isEmpty = 1;

/*
    Overwrites malloc and intitializes the memory array if this is
    the first time malloc is being called in the client file.
*/
void * mymalloc(size_t size, char* file, int line) {
    if (isEmpty){
        initialize();
        isEmpty = 0;
    }
    void *ptr = allocateBlock((int)size);
    if (ptr == NULL){
    	fprintf(stderr, "ERROR: Not enough free space for malloc in FILE: %s LINE: %d \n", file, line );
    }
    return ptr;
}

/*
    Creates an unallocated block that covers the entire memory array.
*/
void initialize(){
    char *p;
    p = &memory[0];
    *((short*)p) = -5000;
}

/*
    This method iterates through the memory array until it finds an
    unallocated block with enough space for the allocation or until
    the end of memory is reached.

    Returns
        pointer to allocated space
        null space is not found for allocation
*/
void *allocateBlock (int size){
    char * current = memory;
    int spaceNeeded = size + 2;
    int index = 0;
    
    int i;
    i = 0;

    //too small or too big
    if (size < 1 || size > 4998){
    	return NULL;
    }

    do {

        short *blockSize = (short*)(current);
      
        short savedBlockSize = *blockSize * -1;
        if (savedBlockSize < 0){
            savedBlockSize = savedBlockSize *  -1;
        }
        int margin = savedBlockSize - spaceNeeded;
        if ((*blockSize < 0) && (margin == 0 || margin > 2)){
            
        	
        	short *p;
            //sets size of allocated space
            p = (short*)current;
            *p = spaceNeeded;
            
            //sets size of new unallocated block if block is split
            if (spaceNeeded != savedBlockSize){
                p = (short*)(current + spaceNeeded);
                *p = (savedBlockSize - spaceNeeded) * -1; //negative because unallocated
            }
            return (current + 2);
        	
    
        }
        else{
            index = index + savedBlockSize;
            current = &memory[index];
            i++;
            
        }
    } while (index < 5000);

    return NULL;
}


/*
    Iterates through the memory array and frees a block of memory if the pointer argument
    points to an allocated block. Prints an error message otherwise.
*/
void myfree(void * ptr, char* file, int line){
    char * current = memory;
    char * prev = memory;
    short* currentSize = NULL;
    int index = 0;
    int blockNotFound = 1;

    if (ptr < (void*)&memory[0] || ptr > (void*)&memory[4997]){
        fprintf(stderr, "ERROR: Invalid pointer in FILE: %s at LINE: %d\n", file, line );
        blockNotFound = 0;
    }
    
    while (index < 5000 && blockNotFound){
        if (ptr == current + 2){
            currentSize = (short*)current;

           
            if(*currentSize < 0){           //already free
                //do nothing
                fprintf(stderr, "ERROR: Double free or pointer not allocated in FILE: %s at LINE: %d\n", file, line  );
                
            }
            else if (isFreeBlockBefore(current, prev) && isFreeBlockBehind(index, current)){
                short* prevSize = (short*)prev;
                char *next = &memory[index + *currentSize];
                short *nextSize = (short*)next;
                *prevSize = *prevSize - *currentSize + *nextSize;
                //printf("FREE SUCCESSFULL A\n");
            }
            else if (isFreeBlockBefore(current, prev)){
                //increases size of previous block and keeps negative
                short* prevSize = (short*)prev;
                *prevSize = *prevSize - *currentSize;
                //printf("FREE SUCCESSFULL B\n");
            }
            else if (isFreeBlockBehind(index, current)){
                //increases size of current block and makes negative
                char *next = &memory[index + *currentSize];
                short *nextSize = (short*)next;
                *currentSize = *nextSize - *currentSize;
                //printf("FREE SUCCESSFULL C\n");
            }
            else{
                *currentSize = *currentSize * -1;
                //printf("FREE SUCCESSFULL D\n");
            }
            blockNotFound = 0;
        }
        else {
            currentSize = (short*)current;
            prev = current;
            if (*currentSize < 0){
                index = index + (*currentSize * -1);
            }
            else{
                index = index + *currentSize;
            }
            current = &memory[index];
        }
    }
    if (blockNotFound == 1){
        //fprintf(stderr, "ERROR: Double free or pointer not allocated in FILE: %s LINE: %d \n", __FILE__, __LINE__ );
        fprintf(stderr, "ERROR: Double free or pointer not allocated in FILE: %s at LINE: %d\n", file, line  );
    }
    else {

    }

}


/*
    Checks to see if the current block has an unallocated block before it. For
    use in the myfree function.
        Returns 
            1 if free block is before current block
            0 otherwise
*/
int isFreeBlockBefore(char * current, char * prev){
    short* prevSize = (short*)prev;
    //if first block or prev is allocated
    if (current == prev && *prevSize > 0){
        return 0;
    }
    else if (*prevSize > 0){
        return 0;
    }
    else {
        return 1;
    }
    
}
/*
    Checks to see if the current block has an unallocated block behind it. For 
    use in the myfree function. 
        Returns
            1 if free block is behind the current block
            0 otherwise
*/
int isFreeBlockBehind(int index, char * current){
    short *currentSize = (short*)current;
    char *next = &memory[index + *currentSize];
    short *nextSize = (short*)next;

    
    if (((index + *currentSize) == 5000) || (*nextSize > 0)){
        return 0;
    }
    else {
        //printf("isFreeBlockBehind index = %d currentSize = %d nextSize = %d\n", index, *currentSize, *nextSize);
        return 1;
    }
}
