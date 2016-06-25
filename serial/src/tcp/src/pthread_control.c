 #include "../../include/myhead.h"

void pthread_control(int serial_fd)
{
    printf("Client initializing...\n");

    int fd = serial_fd;
    int ret;
    printf("severip = %s\n",ip);
    int soc;

    if((soc = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        close(soc);
        exit(0);
    } 

    struct sockaddr_in seraddr;
    memset(&seraddr, 0, sizeof(struct sockaddr_in));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(8888);     
    seraddr.sin_addr.s_addr = inet_addr(ip);  
    if(connect(soc, (struct sockaddr*)&seraddr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        printf("Please check whether sever is running.\n");
        close(soc);
        exit(-1);
    }
    printf("Client initialize ok!\n");
    printf("Connect successfully!\n");
    printf("Sever: %s:%d\n",ip,8888);

    char ask_info[100];
    strcpy(ask_info, "openwrt");
    send(soc, ask_info, 100, 0);
    printf("ask_info send ok,please check android\n");
    struct para param;
    param.soc = soc;
    param.fd = fd;

    pthread_t pth_tcp_read;
    pthread_t pth_serial_read;

    if(0 != (ret = pthread_create(&pth_tcp_read, NULL, (void *)tcp_read, &param)))
    {
        perror("pthread_create");
        exit(1);
    }

    if(0 != (ret = pthread_create(&pth_serial_read, NULL, (void *)serial_read, &param)))
    {
        perror("pthread_create");
        exit(1);
    }
    pthread_join(pth_tcp_read, NULL);
    pthread_join(pth_serial_read, NULL);

}

