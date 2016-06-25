#include "../../include/myhead.h"

void tcp_read(struct para *param)
{
	struct para para_temp = *param;
    printf("Tcp read in\n");
	int soc = para_temp.soc;
	int fd = para_temp.fd;
	char msg[100];
	char *head;
    int ret;
    char ch;

   	while(1)
	{
		memset(msg,0,100);
		if((ret = recv(soc, msg, 100, 0) == 0))
        {
        	printf("sever unexpected break!\n");
        	pthread_exit(pthread_self());
        }

        printf("android :%s\n", msg);
        handle_message(msg);
        write(fd, msg, strlen(msg));
	}
}


void handle_message(char *str)
{
	char *p = str;
	while(*p != '*')
	{
		p++;
	}
	p++;
	*p = '\0';
}
