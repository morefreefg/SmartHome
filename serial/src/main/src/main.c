#include "../../include/myhead.h"

char ip[100];
char baudrate[100];
int main()
{
    int serial_fd = 10;
    uint16_t port;
    int soc;

    readconfig_ip();
    readconfig_baudrate();

    printf("ip = %s\n", ip);
    printf("baudrate = %s\n", baudrate);

    serial_fd = serial_init();

    pthread_control(serial_fd);

    return 0;
}
