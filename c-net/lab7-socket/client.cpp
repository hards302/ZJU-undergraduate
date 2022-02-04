// 注意编译时最后加-lwsock32
#include <stdio.h>
#include <winsock2.h>

//客户端向服务器传送的结构：
struct text
{
	int type = 0; // 0 is sent message, 1 is get time, 2 is get name, 3 is get list.
	int target = 0; // 只有在选择6即发送信息时才会用
	char data[256];
};

u_short SERVER_PORT = 6666; //侦听端口
WORD wVersionRequested;
WSADATA wsaData;
int ret, nLeft, length;
SOCKET sListen, sClient, sServer; //连接套接字
struct sockaddr_in saServer, saClient;//地址信息
struct text message;
char *ptr = (char *)&message;
char addr[20];
BOOL fSuccess = TRUE;

DWORD WINAPI acceptmessage(LPVOID param) {
	SOCKET tcp = *((int*)param);
	while (1) {
		char recvPackage[256];
		int recvState = recv(tcp, recvPackage, 256, 0);
		struct text* rec = (struct text*)recvPackage;
		if (recvState > 0) printf("%s\n", rec->data);
		else break;
	}
	return 0;
}
int main(int argc, char *argv[])
{
	//WinSock初始化：
	wVersionRequested = MAKEWORD(2, 2);//希望使用的WinSock DLL的版本
	ret = WSAStartup( wVersionRequested, &wsaData );
	if (ret != 0) {
		printf("WSAStartup() failed!\n");
		return 0;
	}
	//确认WinSock DLL支持版本2.2：
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup();
		printf("Invalid Winsock version!\n");
		return 0;
	}

	//创建socket，使用TCP协议：
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if(sClient == INVALID_SOCKET) {
		WSACleanup();
		printf("socket() failed!\n");
		return 0;
	}
	
	//构建服务器地址信息：
	while(1) {
		int num;
		printf("choose your service:\n\t1: connect\n\t2: disconnect\n\t3: get time\n\t4: get server name\n\t5: get link list\n\t6: send message\n\t7: quit\n");
		scanf("%d", &num);
		switch( num ) {
			case 1: {
				printf("input address:\n");
				scanf("%s", addr);
				saServer.sin_family = AF_INET;//地址家族
				saServer.sin_port = htons(SERVER_PORT);//注意转化为网络字节序
				saServer.sin_addr.S_un.S_addr = inet_addr(addr);
				//连接服务器：
				ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
				if (ret == SOCKET_ERROR) {
					printf("connect() failed!\n");
					closesocket(sClient);//关闭套接字
					WSACleanup();
					system("pause");
					break;
				}
				else printf("connect() successed!\n");
				HANDLE subThread;
				subThread =  CreateThread(NULL, 0, acceptmessage, &sClient, 0, 0);
				break;
			}
			case 2: {
				closesocket(sClient);//关闭套接字
				WSACleanup();
				printf("disconnect() successed!\n");
				break;
			}
			case 3: {
				message.type = 1; // get time
				ret = send(sClient, (char *)&message, sizeof(message), 0);
				if(ret == SOCKET_ERROR) printf("get time failed!\n");
				break;
			}
			case 4: {
				message.type = 2; // get name
				ret = send(sClient, (char *)&message, sizeof(message), 0);
				if(ret == SOCKET_ERROR) printf("get name failed!\n");
				break;
			}
			case 5: {
				message.type = 3; // get list
				ret = send(sClient, (char *)&message, sizeof(message), 0);
				if(ret == SOCKET_ERROR) printf("get list failed!\n");
				break;
			}
			case 6: {
				message.type = 0;
				printf("please input client number:\n");
				scanf("%d", &message.target);
				printf("please input text message:\n");
				scanf("%s", message.data);
				ret = send(sClient, (char *)&message, sizeof(message), 0);
				if(ret == SOCKET_ERROR) printf("send() failed!\n");
				else printf("text info has been sent!\n");
				break;
			}
			case 7: {
				closesocket(sClient);//关闭套接字
				WSACleanup();
				return 0;
			}
			default:
				break;
		}
	}
	closesocket(sClient);//关闭套接字
	WSACleanup();
	return 0;
}