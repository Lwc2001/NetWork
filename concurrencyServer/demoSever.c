#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


#define SEVER_PORT 8080
#define MAX_LINK 128
#define LOCAL_IPADDRESS "127.0.0.1"
#define BUFFER_SIZE 128

// void sigHander(int sigNum)
// {
//     int ret = 0;
//     /* 资源回收 */
// }

int main()
{
    // /* 信号注册 */
    // signal(SIGINT, sigHander);
    // signal(SIGQUIT, sigHander);
    // signal(SIGTSTP, sigHander);


    /* 创建socket套接字 */
    /* 并监听新创建的套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    /* 设置端口复用 */
    int enableOpt = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&enableOpt, sizeof(enableOpt));
    if (ret == -1)
    {
        perror("setsockopt error");
        exit(-1);
    }
    
#if 0
    /* 绑定 */
    struct sockaddr localAddress;
#else
    struct sockaddr_in localAddress;
#endif
    memset(&localAddress, 0, sizeof(localAddress));

    /* 地址族 */
    localAddress.sin_family = AF_INET;
    /* 端口需要转成大端字节序 */
    localAddress.sin_port = htons(SEVER_PORT);
    /* IP地址需要转成大端字节序 */
    localAddress.sin_addr.s_addr = htons(INADDR_ANY);

    // /* 将点分十进制转化成大端字节序 */
    // inet_pton(AF_INET, LOCAL_IPADDRESS, &(localAddress.sin_addr.s_addr));


    int localAddressLen = sizeof(localAddress);
    /* 将IP地址和端口绑定 */
    ret = bind(sockfd, (struct sockaddr *)&localAddress, localAddressLen);
    if (ret == -1)
    {
        perror("bind error");
        exit(-1);
    }

    /* 监听 */
    ret = listen(sockfd, MAX_LINK);
    if (ret == -1)
    {
        perror("listen error");
        exit(-1);
    }
    
    /* 客户的信息 */
    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));

    socklen_t clientAddressLen = 0;
    int acceptfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
    if (acceptfd == -1)
    {
        perror("accept error");
        exit(-1);
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    char replyBuffer[BUFFER_SIZE];
    memset(replyBuffer, 0, sizeof(replyBuffer));

    int readBytes = 0;
    while (1)
    {
        readBytes = read(acceptfd, buffer, sizeof(buffer));
        if (readBytes <= 0)
        {
            printf("111\n");
            perror("read error");
            close(acceptfd);
            break;
        }
        else
        {
            /* 读到的字符串 */
            printf("buffer = %s\n", buffer);
           
            sleep(3);

            strncpy(replyBuffer, "一起加油", sizeof(replyBuffer) - 1);
            write(acceptfd, replyBuffer, sizeof(replyBuffer));
        
        }
    }

    close(sockfd);
    

    return 0;
}