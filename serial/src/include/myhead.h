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
#include <termios.h>
#include <errno.h>

struct para
{
    int soc;
    int fd;
};

extern char baudrate[100];
extern char ip[100];
void pthread_control(int serial_fd);
int serial_init();
void tcp_read(struct para *param);
void serial_read(struct para *param);
void readconfig_baudrate();
void readconfig_ip();
int handle_ip(char *str);
int handle_baudrate(char *str);
void setbaudrate(struct termios *opt);
void handle_message(char *str);


