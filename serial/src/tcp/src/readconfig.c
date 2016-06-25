 #include "../../include/myhead.h"


int handle_ip(char *str)
{
    char *head = str;
    char *p = ip;
    memset(ip, 0 ,100);
    while(*head != '\0')
    {
    	if((*head == 'i') && (*(head + 1) == 'p'))
    	{
    		head++;
    		head++;
    		head++;
    		head++;

    		memset(ip,0,100);
    		while(*head != '\'')
    		{
    			*p = *head;
    			p++;
    			head++;
    		}
    		*p = '\0';
    		return 1;
    	}

    	head++;
    }
    return 0;
}

int handle_baudrate(char *str)
{
    char *head = str;
    char *p = baudrate;
    memset(baudrate, 0 ,100);
    while(*head != '\0')
    {
        if((*head == 'b') && (*(head + 1) == 't'))
        {
            head++;
            head++;
            head++;
            head++;

            memset(baudrate,0,100);
            while(*head != '\'')
            {
                *p = *head;
                p++;
                head++;
            }
            *p = '\0';
            return 2;
        }

        head++;
    }
    return 0;
}

void readconfig_ip()
{ 
	FILE *fd;
	if(NULL == (fd = fopen("/etc/config/routerset", "rt")))
	{
		printf("open config file error\n");
	}
	char str[100];

	while(feof(fd) == 0)
	{
		memset(str, 0 , 100);
		fgets(str,100,fd);
		if(1 == handle_ip(str))
        {
            break;
        }
	}

	fclose(fd);
}

void readconfig_baudrate()
{ 
    FILE *fd;

    if(NULL == (fd = fopen("/etc/config/routerset", "rt")))
    {
        printf("open config file error\n");
    }
    char str[100];

    while(feof(fd) == 0)
    {
        memset(str, 0 , 100);
        fgets(str,100,fd);
        if(2 == handle_baudrate(str))
        {
            break;
        }
    }

    fclose(fd);
}
