#include <stdio.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <stdlib.h>  
   
int  main(int argc, char **argv)  
{  
    int ret, i, status;  
    pid_t pid;  
	int a1,a2,a3;
 
    while(1)
	{  
		sleep(1);   
        pid = fork();   
        if (pid == -1) {  
            printf("fork() error.errno:%d error:%s\n", errno, strerror(errno));  
        }  
        if (pid == 0) {  
            ret = execv("./xbtmaker",NULL);
            if (ret < 0) {  
                printf("execv ret:%d errno:%d error:%s\n", ret, errno, strerror(errno));  
            }  
            exit(0);  
        }  
   
        if (pid > 0) { 
            printf("parent start......\n");  
            pid = wait(&status);  
			sleep(2);
        }  
   
    }  
    return 0;  
}  
