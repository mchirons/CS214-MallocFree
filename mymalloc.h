#ifndef mymalloc_h
#define mymalloc_h
#define malloc( x ) mymalloc( x, __FILE__, __LINE__)
#define free( x ) myfree( x , __FILE__, __LINE__ )
void * mymalloc(size_t size, char* file, int line);
void myfree(void * ptr, char* file, int line);
void initialize();
void * allocateBlock(int size);
int isFreeBlockBefore(char * current, char * prev);
int isFreeBlockBehind(int index, char * current);
#endif /* mymalloc_h */
