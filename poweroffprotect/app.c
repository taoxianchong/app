#include <stdio.h>  
#include <unistd.h>  
#include <signal.h>  
#include <fcntl.h>  
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
//#include "include/timer.h"

int msSleep(long ms) {

    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = ms;

    return select(0, NULL, NULL, NULL, &tv);
}

//#include <sleep.h>  
void sig_handler(int sig)  
{  
	long thetime = 0;    
	if(sig == SIGIO)  
    {  
	//system("cat /proc/interrupts");	
	while(1)
	{    
		printf("sync return%ld\n", ++thetime);    
		usleep(10);		
		//msSleep(10);
	//thetime = time((time_t*)0);
	
	//system("sync");	
	//printf("sync return%ld\n", ++thetime);
	}  
    }  
}  
  
int main(void)  
{  
    int fd;  
    signal(SIGIO, sig_handler);  
    fd = open("/dev/protector",O_RDWR);  
    fcntl(fd, F_SETOWN, getpid());  
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | FASYNC);  
    printf("waiting key interrupt:\n");  
system("touch fortest1");
system("cat aaa.txt > fortest1");
system("touch fortest2");
system("cat /root/ggg.txt > fortest2");
    while(1)  
    {  
    }  
}  
