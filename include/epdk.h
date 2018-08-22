/*author: charles cheng 2016-07-21*/
#ifndef EPDK_H
#define EPDK_H

#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "./elibs/elibs.h"


#define EPDK_OK		 0
#define EPDK_FAIL 	-1



//debug level
#define DEBUG_LEVEL_GATE 3
#if (DEBUG_LEVEL_GATE == 3)
	#define __dbg(...) printf("[debug]L%d(%s):", __LINE__, __FILE__);\
			   printf(__VA_ARGS__)           //__LINE__, __FILE__ show the line number and the file name, __VA_ARGS__ is the arg you want to print.
					   
	#define __msg(...) printf("[message]");\
		           printf(__VA_ARGS__) 
	
	#define __wrn(...) printf("[warning]L%d(%s):", __LINE__, __FILE__);\
			   printf(__VA_ARGS__) 
#elif(DEBUG_LEVEL_GATE == 2)
	#define __dbg(...) 
	#define __msg(...) printf("[message]");\
		           printf(__VA_ARGS__) 
	
	#define __wrn(...) printf("[warning]L%d(%s):", __LINE__, __FILE__);\
		           printf(__VA_ARGS__) 
#elif(DEBUG_LEVEL_GATE == 1)
	#define __dbg(...)	
	#define __msg(...)	
	#define __wrn(...) printf("[warning]L%d(%s):", __LINE__, __FILE__);\
		           printf(__VA_ARGS__) 
#elif(DEBUG_LEVEL_GATE == 0)
	#define __dbg(...)	
	#define __msg(...)	
	#define __wrn(...) 
#endif

#define show_para(a) __dbg(#a" = %d\n", a)
#define show_para_double(a) __dbg(#a" = %lf\n", a)


#define show_table(size0, size1, array_name)  {\
                                              	printf("\n");\
	                                           	printf(#array_name"\n");\
	                                           	int i, j;\
                                               	for(i=0;i<size0;i++){\
	                                        		for(j=0;j<size1;j++){\
		                                            	printf("0x%x, ", array_name[i*size0+j]);\
	                                            	}\
	                                               	printf("\\\n");\
                                               	}\
                                              }

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)

#define create_list_link(value_type) \
struct _link{\
	void *next;\
	value_type *value;\
};\
struct _list{\
	struct _link *head;\
	int count;\
};
#define init_list(list_name, head_name, head_value) \
	struct _list *list_name;\
	struct _link *head_name;\
	head_name = (struct _link*)malloc(sizeof(struct _link));\
	list_name = (struct _list*)malloc(sizeof(struct _list));\
	list_name->head = head_name;\
	list_name->count = 1;\
	head_name->next = NULL;\
	head_name->value = head_value;

#define init_link(link_name, link_value) \
	struct _link *link_name;\
	link_name = (struct _link*)malloc(sizeof(struct _link));\
	link_name->value = link_value;

#define insert_link(list , pre_link, link) {\
	link->next = pre_link->next;\
	pre_link->next = link;\
	list->count++;\
}
#define show_list(list) {\
	struct _link *cur = list->head;\
	while(cur != NULL){\
		printf("%d\n", *(cur->value));\
		cur = cur->next;\
	}\
}


#endif
