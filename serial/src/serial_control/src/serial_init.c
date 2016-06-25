#include "../../include/myhead.h"

void setbaudrate(struct termios *opt)
{
    if(strcmp(baudrate, "4800") == 0)
    {
        cfsetispeed(opt, B4800);
        cfsetospeed(opt, B4800);
    }

    if(strcmp(baudrate, "4800") == 0)
    {
        cfsetispeed(opt, B4800);
        cfsetospeed(opt, B4800);
    }

    if(strcmp(baudrate, "9600") == 0)
    {
        cfsetispeed(opt, B9600);
        cfsetospeed(opt, B9600);
    }

    if(strcmp(baudrate, "19200") == 0)
    {
        cfsetispeed(opt, B19200);
        cfsetospeed(opt, B19200);
    }

    if(strcmp(baudrate, "38400") == 0)
    {
        cfsetispeed(opt, B38400);
        cfsetospeed(opt, B38400);
    }

    if(strcmp(baudrate, "57600") == 0)
    {
        cfsetispeed(opt, B57600);
        cfsetospeed(opt, B57600);
    }

     if(strcmp(baudrate, "115200") == 0)
    {
        cfsetispeed(opt, B115200);
        cfsetospeed(opt, B115200);
    }
}

int serial_init()
{   
    printf("Serial initializing...\n");

    int fd;
    int i;
    int len;
    int n = 0;      
    char read_buf[256];
    char write_buf[256];
    struct termios opt; 
    
    fd = open("/dev/ttyATH0", O_RDWR|O_NOCTTY);
    if(fd == -1)
    {
        perror("open serial 0\n");
        exit(0);
    }

    tcgetattr(fd, &opt);      
    bzero(&opt, sizeof(opt));
    
    tcflush(fd, TCIOFLUSH);

    setbaudrate(&opt);
    
    opt.c_cflag &= ~CSIZE;  
    opt.c_cflag |= CS8;   
    opt.c_cflag &= ~CSTOPB; 
    opt.c_cflag &= ~PARENB; 
    opt.c_cflag &= ~CRTSCTS;
    opt.c_cflag |= (CLOCAL | CREAD);
 
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 
    opt.c_oflag &= ~OPOST;
    
   // opt.c_cc[VTIME] = 1000000;
   // opt.c_cc[VMIN] = 1;
    
    tcflush(fd, TCIOFLUSH);
    
    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        perror("serial error");
        return -1;
    }
    printf("Serial init ok!\n");
    return fd;
}