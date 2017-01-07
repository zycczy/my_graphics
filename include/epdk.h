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

#include "./elibs/elibs.h"
#include "./algorithm/algorithm.h"

#define EPDK_OK		 0
#define EPDK_FAIL 	-1




//debug level
#define DEBUG_LEVEL_GATE 3
#if (DEBUG_LEVEL_GATE == 3)
	#define __dbg(...) printf("[debug]L%d(%s):", __LINE__, __FILE__);\
					   printf(__VA_ARGS__)           //__LINE__, __FILE__为显示所在行和所在文件， __VA_ARGS__ 为可变参数
					   
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




#endif
