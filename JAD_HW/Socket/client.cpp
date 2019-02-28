#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll


#define MAX_NUMBER 200
#define MAX_THREAD_NUMBER 9
DWORD WINAPI receive_func(LPVOID lpParameter);
DWORD WINAPI create_client(LPVOID lpParameter);

DWORD WINAPI receive_func(LPVOID lpParameter)
{
	unsigned int *p1 = (unsigned int*)lpParameter;
	SOCKET sock = *p1;    //强制类型转换
	while (1)
	{
		//接收服务器传回的数据
		char recv_pack[MAX_NUMBER] = { 0 };
		recv(sock, recv_pack, MAX_NUMBER, NULL);    //接受返回消息包
		printf("来自客户端的消息:\n");
		int k = 0;	//置零
		while (recv_pack[k] != '#'&& k < MAX_NUMBER)
		{
			k++;
		}
		while (recv_pack[k + 1] != '$'&& k < MAX_NUMBER)
		{
			k++;
			printf("%c", recv_pack[k]);    //打印内容
		}
		printf("\n");
		printf("\n");
	}

	return 0;
}
DWORD WINAPI create_client(LPVOID lpParameter)    //number为客户端的编号
{
	int *p = (int*)lpParameter;
	int number = *p;    //强制类型转换
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//定义区
	int choose, k, position;
	char IP_address[50] = { 0 };
	char Port_number[10] = { 0 };
	char send_pack[MAX_NUMBER] = { 0 };
	char temp[20] = { 0 };
	int connect_symbol = 0;    //标识符
	int exit_symbol = 0;
	int list_symbol = 0;
	HANDLE child_thread;

	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;    //使用IPV4地址

	while (!exit_symbol)
	{
		//菜单功能的实现

		memset(send_pack, '\0', MAX_NUMBER);
		if (connect_symbol == 0)    //未连接状态
		{
			printf("client %d menu: 0.connect 1.exit\n请输入功能的数字编号\n", number);
			scanf_s("%d", &choose);
			if (choose == 0)
			{
				printf("client %d:请输入IP地址和端口号\n", number);
				printf("10.180.47.254\n");
				printf("3113\n");
				//gets_s(IP_address);    //输入客户端IP地址
				//gets_s(Port_number);    //输入客户端的端口号
				sockAddr.sin_addr.s_addr = inet_addr("10.180.79.196");    //IP_address
				sockAddr.sin_port = htons(3113);    //(int)Port_number
				connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));    //与服务器进行连接
				child_thread = CreateThread(NULL, 0, receive_func, &sock, 0, NULL);   //创建子线程，来实现接受返回数据的功能
				printf("已连接!\n");

				connect_symbol = 1;    //设定标识符为已连接状态
			}
			else if (choose == 1)
			{
				exit_symbol = 1;
			}
			else
			{
				printf("请输入正确数字编号!\n");
			}
		}
		else    //已连接状态
		{
			//组装数据包
			send_pack[0] = '#';    //以#作为起始标识符
			char temp[1];
			itoa(number, temp, 10);
			send_pack[1] = temp[0];    //第一组数据为编号
			send_pack[3] = '$';    //以$作为结束标识符

			printf("menu: 0.disconnect 1.exit 2.get now time 3.get name 4.get client list 5.send message \n请输入功能的数字编号\n");
			scanf_s("%d", &choose);    //获得用户的选择选项
			if (choose == 0)
			{
				send_pack[2] = '0';    //断开连接请求，则第二组数据为0
				send(sock, send_pack, MAX_NUMBER, 0);    //发送请求数据包
				connect_symbol = 0;
				CloseHandle(child_thread);    //关闭子线程

			}
			else if (choose == 1)
			{
				send_pack[2] = '0';    //断开连接请求，则第二组数据为0
				send(sock, send_pack, MAX_NUMBER, 0);    //发送请求数据包
				exit_symbol = 1;
			}
			else if (choose == 2)
			{
				send_pack[2] = '2';    //time请求，则第二组数据为2
				send(sock, send_pack, MAX_NUMBER, 0);    //发送请求数据包
			}
			else if (choose == 3)
			{
				send_pack[2] = '3';    //name请求，则第二组数据为3
				send(sock, send_pack, MAX_NUMBER, 0);    //发送请求数据包
			}
			else if (choose == 4)
			{
				send_pack[2] = '4';    //list请求，则第二组数据为4
				send(sock, send_pack, MAX_NUMBER, 0);    //发送请求数据包
				list_symbol = 1;
			}
			else if (choose == 5)
			{
				if (list_symbol == 0)
				{
					printf("未获得客户端列表!\n");
				}
				else
				{
					send_pack[2] = '5';    //message请求，则第二组数据为5
					char target_client_number;    //最大编号为9，所以用单个字符即可
					char send_message[MAX_NUMBER-4];
					getchar();
					scanf_s("%c", &target_client_number);
					getchar();
					scanf("%s", send_message);
					send_pack[3] = target_client_number;
					strcat(&send_pack[4], send_message);
					send_pack[5 + strlen(send_message)] = '$';    //以$作为pcak的结束标识符
					send(sock, send_pack, MAX_NUMBER, 0);    //发送请求数据包

				}
			}
			else
			{
				printf("请输入正确数字编号!\n");
			}
		}
		Sleep(1000);
	}
	closesocket(sock);  //关闭套接字
	WSACleanup();  //终止使用 DLL

	return 0;
}
int main()
{
	HANDLE hThread[MAX_THREAD_NUMBER];
	int client_number;
	printf("请输入客户端数量\n");
	scanf_s("%d", &client_number);    //输入客户端数量
	int i;
	for (i = 0; i < client_number; i++)
	{
		hThread[i] = CreateThread(NULL, 0, create_client, &i, 0, NULL);    //不同主线程创建不同客户端   可能有问题！
	}

	Sleep(300000);    //等待30秒
	printf("按回车结束!\n");
	system("pause");    //冻结屏幕，便于观察

	for (int i = 0; i < client_number; i++)
	{
		CloseHandle(hThread[i]);    //关闭所有主线程
	}

	return 0;
}
