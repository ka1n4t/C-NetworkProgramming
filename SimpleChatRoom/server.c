#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define PROTOPORT 5188 //默认协议端口号

#define QLEN 1 //监听套接字请求队列的大小

void main(int argc, char* argv[]){
	
	struct hostent *ptrh;	//指向主机列表的指针
	struct sockaddr_in servaddr; //存放服务器网络地址的结构体
	struct sockaddr_in clientaddr; //存放客户端网络地址的结构体
	int listenfd; //监听套接字描述符
	int clientfd; //响应套接字描述符
	int port; //协议端口号
	int alen; //地址长度
	char sendbuf[1000]; //发送缓冲区
	char recvbuf[1000]; //接收缓冲区
	int recvdatalen; //接收数据的长度

	memset((char*)& servaddr, 0, sizeof(servaddr)); //清空sockaddr结构
	servaddr.sin_family = AF_INET; //设置为Internet协议族
	servaddr.sin_addr.s_addr = INADDR_ANY; //设置本地IP地址

	port = argc > 1 ? atoi(argv[1]) : PROTOPORT; //把字符串转换成整数，或直接只用默认端口号
	if(port > 0){
		servaddr.sin_port = htons((u_short)port); //无符号短整形转换成网络字节顺序
	}else {
		fprintf(stderr, "bad port number %s\n", argv[1]);
		exit(1);
	}

	//创建监听流式套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0) {
		fprintf(stderr, "socket creation failed\n");	
		exit(1);
	}
	
	//绑定本地地址
	if(bind(listenfd, (struct sockaddr *)& servaddr, sizeof(servaddr)) < 0) {
		fprintf(stderr, "bind failed\n");
		exit(1);
	}

	//开始监听，并指定接收连接数
	if(listen(listenfd, QLEN) < 0) {
		fprintf(stderr, "listen failed\n");
		exit(1);
	}

	//主套接字等待客户端连接
	while(1) {
		alen = sizeof(clientaddr);
		if((clientfd = accept(listenfd, (struct sockaddr *)& clientaddr, &alen)) < 0) {
			fprintf(stderr, "accept failed\n");
			exit(1);
		}

		printf("friend connected\n");

		while(1) {
			printf("say something:");
			scanf("%s", sendbuf);
			printf("\n");
			send(clientfd, sendbuf, strlen(sendbuf), 0);
			recvdatalen = recv(clientfd, recvbuf, 1024, 0);
			if(recvdatalen > 0) {
				recvbuf[recvdatalen] = "\0";
				printf("friend says:%s\n", recvbuf);
			}

		}
		close(clientfd);
	}

	return 0;
}
