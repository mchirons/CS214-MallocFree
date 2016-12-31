memgrindmake: memgrind.c mymalloc.c mymalloc.h
	gcc -Wall -g memgrind.c mymalloc.c -o memgrind
clean :
	rm memgrind