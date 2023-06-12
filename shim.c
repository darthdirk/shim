


#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>



int rand(){
	//declare a function pointer that looks like rand
	int (*original_rand)(void) = dlsym(RTLD_NEXT,"rand");//call dlsym to get a pointer to the systems rand function 
	return original_rand() % 100;
}

