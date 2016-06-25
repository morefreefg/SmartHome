#include "../../include/myhead.h"

void serial_read(struct para *param)
{
    struct para para_temp = *param;
    printf("Serial read in\n");
    int soc = para_temp.soc;
    int fd = para_temp.fd;

    char read_buf[100];
    char write_buf[100];

    //while(1);
    int i,n,len;
    printf("fd = %d\n",fd);
    int flag = 0;
    char ch;
    while(1)
    {
        n = 0;
        len = 0;
        flag = 0;
        bzero(read_buf, sizeof(read_buf));    //类似于memset
        bzero(write_buf, sizeof(write_buf));
        //printf("1\n");
        while(flag == 0)
        {
            n = read(fd, read_buf, sizeof(read_buf));

            for(i = len; i < (len + n); i++)
            {
                write_buf[i] = read_buf[i - len];
                if(write_buf[i] == '*')
                {
                    flag = 1;
                }
            }
            
            bzero(read_buf, sizeof(read_buf));
            len += n;
        }

        write_buf[len] = '\0';   
 
        if(len != 0)
        { 
            printf("%s\n", write_buf);
            send(soc, write_buf, 100, 0);
        }
    
    }
}