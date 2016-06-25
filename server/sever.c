#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h> 
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h> 


#define OPTIONAL_FLAG 0
#define ROUTER_SEND_FLAG 1 
#define ANDROID_SEND_FLAG 2

#define ANDROID_EXIT_FLAG 4
#define OPENWRT_EXIT_FLAG 5
#define OPTIONAL_EXIT_FLAG 0
 
int i;
int msg_flag = 0;
 
char msg[100];
int ROUTER_ONLINE_FLAG = OPTIONAL_FLAG;

int a_e_f = OPTIONAL_EXIT_FLAG;
int o_e_f = OPTIONAL_EXIT_FLAG;

void android_read(int *socket)
{
    int soc = *socket;
    char message[100];
    char msg1[100], msg2[100];
    while(1)
    {
        memset(message, 0, 100);
        if(0 == recv(soc, message, 100, 0))
        {
        	printf("android offline\n");
        	printf("android read thread exit\n");
        	msg_flag = OPTIONAL_FLAG;
            close(soc);
            a_e_f = ANDROID_EXIT_FLAG;
        	pthread_exit(pthread_self());
        }
        printf("android message:%s\n",message);
        //read_msg(message, msg1, msg2);
        if(strncmp(message, "ask", 3) == 0) 
        {
        	memset(message, 0, 100);
            printf("android ask if router online.\n");
            if(ROUTER_ONLINE_FLAG == 0)
        	{
                printf("offline\n");
                strcpy(message, "ROUTER_OFFLINE");
        		send(soc, message, strlen(message), 0);
        	}

        	if(ROUTER_ONLINE_FLAG == 1)
        	{
                printf("online\n");
        		strcpy(message, "ROUTER_ONLINE");
        		send(soc, message, strlen(message), 0);
        	}
        }

        else
        {
            strcpy(msg, message);
            msg_flag = ANDROID_SEND_FLAG;
        }
        /****************/
    }
}

void android_write(int *socket)
{
    int soc = *socket;
    char str[100];

    while(1)
    {
    	if(a_e_f == ANDROID_EXIT_FLAG)
        {
        	printf("android write thread exit.\n");
        	memset(msg, 0, 100);
        	msg_flag = OPTIONAL_FLAG;
        	a_e_f = OPTIONAL_EXIT_FLAG;
        	pthread_exit(pthread_self());
        }
        if(msg_flag == ROUTER_SEND_FLAG)
        {
        	send(soc, msg, 100, 0);
	        msg_flag = OPTIONAL_FLAG;
	        printf("send to android.\n");
	        memset(msg, 0, 100);       	
        }
    }
}

void openwrt_read(int *socket)
{
    int soc = *socket;
    char str[100];

    while(1)
    {
    	if(0 == recv(soc, str, 100, 0))
    	{
    		printf("router offline\n");
    		printf("router read thread exit.\n");
    		msg_flag = OPTIONAL_FLAG;
    		o_e_f = OPENWRT_EXIT_FLAG;
            close(soc);
    		ROUTER_ONLINE_FLAG = 0;
    		pthread_exit(pthread_self());
    	}

        printf("router message:%s\n",str);       
    	strcpy(msg, str);
        msg_flag = ROUTER_SEND_FLAG;
        memset(str, 0, 100);
    }
}

void openwrt_write(int *socket)
{
    int soc = *socket;
    char str[100];

    while(1)
    {
    	if(o_e_f == OPENWRT_EXIT_FLAG)
        {
        	printf("router write thread exit.\n");
        	memset(msg, 0, 100);
        	msg_flag = OPTIONAL_FLAG;
        	o_e_f = OPTIONAL_EXIT_FLAG;
        	pthread_exit(pthread_self()); 
        }

        if(msg_flag == ANDROID_SEND_FLAG)
        {
            send(soc, msg, 100, 0);
	        printf("send to router.\n");
            msg_flag = OPTIONAL_FLAG;
            memset(msg, 0, 100);
        }
        
    }
}


int main()
{
    system("clear");
    i= 0;
    printf("Sever initializing...\n");
    int soc;

    if((soc = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(0);
    }

    static struct sockaddr_in my_addr;                   
    memset(&my_addr, 0, sizeof(struct sockaddr_in));   
    my_addr.sin_family = AF_INET;                   
    my_addr.sin_port = htons(8888);     
    my_addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
 
    if(bind(soc, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in)) == -1)
    { 
        perror("bind");
        printf("Port 8888 is occupied by other programme.\n");
        close(soc);
        exit(-1); 
    }

    if(-1 == listen(soc,100))
    {
        perror("listen");
        close(soc);
        exit(-2);
    }
    printf("Sever init ok !\n");


    int new_soc;   
    int port;
    int ret;
    int len = sizeof(struct sockaddr_in);
    pthread_t pth[50];

    struct sockaddr_in client_soc;
    memset(&client_soc, 0, sizeof(struct sockaddr));
    char str[100];
    while(1)
    {
        new_soc = accept(soc, (struct sockaddr*)&client_soc, &len);

        printf("%s:%d successfully connect!\n",inet_ntoa(client_soc.sin_addr),ntohs(client_soc.sin_port));
        memset(str, 0 ,100);
        recv(new_soc, str, 100, 0);      
        if(strncmp(str, "android", 7) == 0)
        {
            printf("android online\n");
            if(0 != (ret = pthread_create(&pth[i], NULL, (void *)android_write, &new_soc)))
            {
                perror("pthread_create");
                exit(1);
            }

            if(0 != (ret = pthread_create(&pth[i], NULL, (void *)android_read, &new_soc)))
            {
                perror("pthread_create");
                exit(1);
            }
        }

        if(strncmp(str, "openwrt", 7) == 0)
        {
            printf("router online\n");
            ROUTER_ONLINE_FLAG = 1;
            if(0 != (ret = pthread_create(&pth[i], NULL, (void *)openwrt_read, &new_soc)))
            {
                perror("pthread_create");
                exit(1);
            }

            if(0 != (ret = pthread_create(&pth[i], NULL, (void *)openwrt_write, &new_soc)))
            {
                perror("pthread_create");
                exit(1);
            }
        }       

        if((strncmp(str, "openwrt", 7) != 0) && (strncmp(str, "android", 7) != 0))
        {
            printf("Illegal terminal!\n");
            close(new_soc);
            continue;
        }

        i++;
    }  
}
