#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define PROTOPORT 5188

extern int errno;
char localhost[] = "localhost";

main(int argc, char *argv[]){
	
	struct hostent *ptrh; //指向主机列表中的一个条目的指针
	struct sockaddr_in servaddr; //存放服务器网络地址的结构体
	int sockfd; //
	int port;
	char* host;
	int n;
	char recvbuf[1000];
	char sendbuf[1000];

	memset((char*)& servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	port = argc > 2 ? atoi(argv[2]) : PROTOPORT;
	if(port > 0) {
		servaddr.sin_port = htons((u_short)port);
	}else {
		fprintf(stderr, "bad port number %s\n", argv[2]);
		exit(1);
	}
	
	host = argc > 1?argv[1] : localhost;

	//将主机名转换成相应的IP地址并复制到servaddr结构中
	ptrh = gethostbyname(host);
	//检查主机名的有效性	
	if((char *)ptrh == NULL){
		fprintf(stderr, "invalid host: %s\n", host);
		exit(1);
	}	
	memcpy(&servaddr.sin_addr, ptrh->h_addr, ptrh->h_length);

	//创建套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		fprintf(stderr, "socket creation failed\n");
		exit(1);
	}

	//连接服务器
	if(connect(sockfd, (struct sockaddr *)& servaddr, sizeof(servaddr)) < 0){
		fprintf(stderr, "connect failed\n");
		exit(1);
	}
	printf("friends connected.\n");
	
	while(1) {
		n = recv(sockfd, recvbuf, sizeof(recvbuf), 0); // n表示recv的字节数
		/*while(n > 0) {
			write(1, recvbuf, n);
			n = recv(sockfd, recvbuf, sizeof(recvbuf), 0);
		}*/
		/*while(n > 0) {
			recvbuf[n] = "\0";
			printf("friend says:%s\n", recvbuf);
			n = recv(sockfd, recvbuf, sizeof(recvbuf), 0);
		}*/
		if(n > 0) {
			recvbuf[n] = "\0";
			printf("friend says:%s\n", recvbuf);
		}
		printf("say something:");
		scanf("%s", sendbuf);
		printf("\n");
		send(sockfd, sendbuf, strlen(sendbuf), 0);
		
	}
	exit(0);
}
