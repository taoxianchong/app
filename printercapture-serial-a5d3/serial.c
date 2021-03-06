#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include "include/log.h"

int SerialOpen(char *nComport)
{
    int fd = 0;
    
     fd = open(nComport, O_RDWR|O_NOCTTY);
    //fd = open(nComport, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(fd <= 0) 
    {
        LOGE("Can't open serial port.\n");
        return -1;
    }
    else
       LOGD("Open %s\n",nComport);

    return fd;
}

void SerialClose(int Serialfd)
{
    close(Serialfd);
}

int SerialOpt(int Serialfd, int nSpeed, int nBits, int nFctl, char nEvent, int nStop)
{
    struct termios newtio, oldtio;

    if (tcgetattr(Serialfd, &oldtio) != 0)  
        { 
            LOGD("Setup Serial getattr error\n");  
            return -1;
        }
    LOGD("Speed=%d, nBits=%d, nFctl=%d,nEvent=%c, nStop=%d\n", nSpeed, nBits, nFctl, nEvent, nStop);
    bzero(&newtio, sizeof(newtio));  

    switch (nSpeed)  
        {  
        case 2400:  
            cfsetispeed(&newtio, B2400);  
            cfsetospeed(&newtio, B2400);  
            break;  
        case 4800:  
            cfsetispeed(&newtio, B4800);  
            cfsetospeed(&newtio, B4800);  
            break;  
        case 9600:  
            cfsetispeed(&newtio, B9600);  
            cfsetospeed(&newtio, B9600);  
            break;  
        case 19200:  
            cfsetispeed(&newtio, B19200);  
            cfsetospeed(&newtio, B19200);  
            break;  
        case 38400:  
            cfsetispeed(&newtio, B38400);  
            cfsetospeed(&newtio, B38400);  
            break;  
        case 57600:  
            cfsetispeed(&newtio, B57600);  
            cfsetospeed(&newtio, B57600);  
            break;  
        case 115200:  
            cfsetispeed(&newtio, B115200);  
            cfsetospeed(&newtio, B115200);  
            break;  
        case 230400:  
            cfsetispeed(&newtio, B230400);  
            cfsetospeed(&newtio, B230400);  
            break;
       case 460800:
            cfsetispeed(&newtio, B460800);  
            cfsetospeed(&newtio, B460800);  
            break;
        case 500000:
            cfsetispeed(&newtio, B500000);  
            cfsetospeed(&newtio, B500000);  
            break;
        case 576000:
            cfsetispeed(&newtio, B576000);  
            cfsetospeed(&newtio, B576000);  
            break;
        case 921600:
            cfsetispeed(&newtio, B921600);  
            cfsetospeed(&newtio, B921600);  
            break;
        case 1000000:
            cfsetispeed(&newtio, B1000000);  
            cfsetospeed(&newtio, B1000000);  
            break;

        case 1152000:
            cfsetispeed(&newtio, B1152000);  
            cfsetospeed(&newtio, B1152000);  
            break;
            
        case 2500000:
            cfsetispeed(&newtio, B2500000);  
            cfsetospeed(&newtio, B2500000);  
            break;
        case 3000000:
            cfsetispeed(&newtio, B3000000);  
            cfsetospeed(&newtio, B3000000);  
            break;

        default:  
            cfsetispeed(&newtio, B9600);  
            cfsetospeed(&newtio, B9600);  
            break;  
        } 

    // newtio.c_cflag |= CLOCAL | CREAD;     
    newtio.c_cflag &= ~CSIZE;  

    switch (nBits)  
        {  
        case 7:  
            newtio.c_cflag |= CS7;  
            break;  
        case 8:
        default:
            newtio.c_cflag |= CS8;  
            break;  
        }  

    switch (nFctl)  
        {  
        case 0: 
        default:
            newtio.c_cflag &= ~CRTSCTS; 
		newtio.c_iflag &= ~(IXON | IXOFF | IXANY);  
            break;  
        case 1:  
            newtio.c_cflag |= CRTSCTS;  
		newtio.c_iflag &= ~(IXON | IXOFF | IXANY);         
    	break;  
        case 2:  
            newtio.c_iflag |= IXON | IXOFF | IXANY;  
            break;  
        }  

    switch (nEvent)  
        {  
        case 'o':  
        case 'O':  
            newtio.c_cflag |= PARENB; 
            newtio.c_cflag |= PARODD;  
            newtio.c_iflag |= INPCK;  
            break;  
        case 'e':  
        case 'E':  
            newtio.c_cflag |= PARENB;  
            newtio.c_cflag &= ~PARODD;  
            newtio.c_iflag |= INPCK;  
            break;  
        case 'n':  
        case 'N':  
            newtio.c_cflag &= ~PARENB;  
            newtio.c_iflag &= ~INPCK;  
            break;  
        case 's':
        case 'S':
            newtio.c_cflag &= ~PARENB;  
            newtio.c_cflag &= ~CSTOPB;  
            newtio.c_iflag |= INPCK;  
            break;  
        }  

    if (nStop == 1)  
        newtio.c_cflag &= ~CSTOPB;  
    else if (nStop == 2)  
        newtio.c_cflag |= CSTOPB;  
          
    /* 修改此处参数，以保证正常接收 */  
    newtio.c_cc[VTIME] = 0;  
	
    newtio.c_cc[VMIN] = 1;  

 

    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
    newtio.c_oflag &= ~OPOST;  
    newtio.c_iflag &= ~(INPCK|BRKINT|ISTRIP|IXON|ICRNL);
    tcflush(Serialfd, TCIOFLUSH);  

    if ((tcsetattr(Serialfd, TCSANOW, &newtio)) != 0)  
        {  
            LOGD("Setup Serial setattr error\n");  
            return -1;
        }  

    return 0;
}

int SerialRead(int Serialfd, char *ReadBuf, int ReadNum)
{
    int nRead = 0;

    memset(ReadBuf, 0, sizeof(ReadBuf));
    nRead = read(Serialfd, ReadBuf, ReadNum);
    /* if(-1==nRead) */
    /*     { */
    /*         perror("error"); */
    /*     } */
    //LOGD("%d bytes data had read\n", nRead);

    return nRead;
}

int SerialWrite(int Serialfd, char *WriteBuf, int WriteNum)
{
    int nWrite = 0;

    nWrite = write(Serialfd, WriteBuf, WriteNum);
//    LOGD("%d bytes data had write\n", nWrite);

    return nWrite;
}
