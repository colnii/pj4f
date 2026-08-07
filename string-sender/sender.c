/*Using nc -ukl 8888 without py*/
#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <message>\n", argv[0]);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        perror("Failed to create socket");
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8888);
    servaddr.sin_family = AF_INET;


    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
    }

    printf("Sending message\n");
    char msg[1000];
    memset(msg,0,sizeof(msg));
    memcpy(msg,argv[1],strlen(argv[1]));
    msg[strlen(argv[1])] = '\n';
    send(sockfd, msg, strlen(msg), 0);
    printf("Sent message\n");

}