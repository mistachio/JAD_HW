#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

using namespace std;
#define MAXLEN 200
#define MAX_THRD 6
#define MAX_THREAD_NUMBER 9

DWORD WINAPI SubThread(LPVOID lpParam);

vector<string> cln_list;    //地址列表
vector<int> num_of_addr;    //已有的地址编号
vector<SOCKET> cln_socket;  //接收到的客户端socket

int main() {
	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("10.110.51.110");  //具体的IP地址
	sockAddr.sin_port = htons(3113);  //端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//进入监听状态
	listen(servSock, 20);

	//接收客户端请求
	SOCKADDR clntAddr;
	int k = 0;
	SOCKET clntSock[MAX_THREAD_NUMBER];    //最多9个clntSock套接字
	clntSock[k] = INVALID_SOCKET;
	HANDLE HDL[MAX_THREAD_NUMBER];    //定义句柄
	int nSize = sizeof(SOCKADDR);
	int exit_symbol = 0;    //退出标识
	while ((k<MAX_THREAD_NUMBER) && (!exit_symbol))    //连接到最大客户端数,或者收到exit消息后退出
	{
		while (clntSock[k] == INVALID_SOCKET)
		{
			cout << "wait link......" << endl;
			clntSock[k] = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		}
		sockaddr_in sin;
		memcpy(&sin, &clntAddr, sizeof(sin));
		string address = inet_ntoa(sin.sin_addr);
		cln_list.push_back(address);

		cout << "Address = " << address << endl;

		cln_socket.push_back(clntSock[k]);

		printf("A client link successfully!\n");

		if ((DWORD)(HDL[k] = CreateThread(NULL, 0, SubThread, &clntSock[k], 0, NULL)) == 1)    //退出指令
		{
			exit_symbol = 1;
		}

		k++;
		clntSock[k] = INVALID_SOCKET;
	}

	cout << "Finished!" << endl;
	for (int i = 0; i < cln_socket.size(); i++)    //关闭cln_socket套接字
	{
		closesocket(cln_socket[i]);
	}

	//关闭套接字
	while (k != 0)
	{
		k--;
		closesocket(clntSock[k]);
		CloseHandle(HDL[k]);
	}
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();
	system("pause");    //冻结屏幕，便于观察

	return 0;


}

DWORD WINAPI SubThread(LPVOID lpParam) {

	//接收请求包
	int rcv = SOCKET_ERROR; //假设初始连接状态为失败
	char buf[MAXLEN];		//缓冲区

	SOCKADDR rcvAddr;
	int nSize = sizeof(SOCKADDR);

	unsigned int *p = (unsigned int*)lpParam;
	SOCKET rcvSock = *p;    //一系列类型转换
	string str = "#Hello$";
	char cstr[MAXLEN];
	int flag = 0;

	strcpy(cstr, str.c_str());
	send(rcvSock, cstr, str.length() + sizeof(char), NULL);  //向客户端发送hello

	while (true)	//循环读取
	{
		memset(buf, '\0', MAXLEN);  //重置缓冲区
		rcv = recv(rcvSock, buf, MAXLEN, NULL);  //receive接收数据包
		cout << "circle!" << endl;
		if (rcv != SOCKET_ERROR)
		{
			int position = 0;
			int finish_point = 0;
			while (buf[position] != '#' && position<MAXLEN)  //找到包中信息的起始符号#
			{
				position++;
			}
			if (position < MAXLEN - 1 && flag == 0)
			{
				int num = (int)(buf[position + 1] - '0');   //包中表示客户端的编号
				int i;
				for (i = 0; i < num_of_addr.size() && num_of_addr[i] != num; i++);
				if (i == num_of_addr.size() && num_of_addr.size() != 0)
				{
					num_of_addr.push_back(0);
				}
				else {
					num_of_addr.push_back(num);
				}
				flag = 1;
			}
			while (buf[finish_point] != '$' && position < MAXLEN)   //找到包中的结束符号$
			{
				finish_point++;
			}
			if (position < finish_point && finish_point < MAXLEN) //如果包中找到起始符和结束符，则含有一个完整的数据包，进行以下操作
			{
				switch (buf[position + 2])
				{
				case '0':    //接到0标识，断开连接，关闭子线程
				{
					cout << "disconnect!" << endl;
					return 0;	//跳出循环回到主线程中关闭
					break;
				}
				case '1':    //接到1标识，断开所有连接线程，关闭主线程
				{
					cout << "exit all!" << endl;
					return 1;	//跳出循环回到主线程中关闭
					break;
				}
				case '2':
				{
					time_t timep;
					struct tm *p;
					time(&timep);
					p = gmtime(&timep);
					char year;
					char month;
					char day;
					char hour;
					char min;
					_itoa(p->tm_year + 1900, &year, 10);
					_itoa(p->tm_mon + 1, &month, 10);
					_itoa(p->tm_mday, &day, 10);
					_itoa(p->tm_hour + 8, &hour, 10);
					_itoa(p->tm_min, &min, 10);
					char ch_tm[MAXLEN] = { 0 };
					char slant[2] = { 0 };
					char end[2] = { 0 };
					char blank[2] = { 0 };
					char colon[2] = { 0 };
					slant[0] = '/';
					end[0] = '$';
					blank[0] = ' ';
					colon[0] = ':';
					ch_tm[0] = '#';
					strcat(ch_tm, &year);
					strcat(ch_tm, slant);
					strcat(ch_tm, &month);
					strcat(ch_tm, slant);
					strcat(ch_tm, &day);
					strcat(ch_tm, blank);
					strcat(ch_tm, &hour);
					strcat(ch_tm, colon);
					strcat(ch_tm, &min);
					strcat(ch_tm, end);
					send(rcvSock, ch_tm, strlen(ch_tm) + sizeof(char), NULL); //发送时间包
					break;
				}
				case '3':
				{
					char name[MAXLEN] = "#server name is: server 2.0$";
					send(rcvSock, name, strlen(name) + sizeof(char), NULL); //发送名字包
					break;
				}
				case '4':
				{
					char temp_c = buf[position + 1];    //读出客户端编号 
					string s_list = "#client number: ";
					s_list += temp_c;
					s_list += "  IP: ";
					for (int i = 0; i < cln_list.size(); i++)
					{
						if (num_of_addr[i] != 0) {
							char c = num_of_addr[i] + '0';
							s_list += cln_list[i] + ';';  //输出列表中的客户端ip，以"_"作为分隔符
						}
					}
					s_list += '$'; //添加结束符
					char list[MAXLEN];
					strcpy(list, s_list.c_str());
					send(rcvSock, list, strlen(list) + sizeof(char), NULL); //发送客户端列表包
					break;
				}
				case '5':
				{
					char temp_c = buf[position + 1];    //读出客户端编号

					char ch_num = buf[position + 3];    //第4位存储的是目标客户端编号
					int num = ch_num - '0';
					int index = 0;
					while (index < num_of_addr.size() && num_of_addr[index] != num) {
						//在num_of_addr中找到具体编号
						index++;
					}
					int signal;                  //用于判断到接收端的socket连接是否存在
					char confirm[MAXLEN];      //发送回原客户端的反馈信号
					if (index == num_of_addr.size())
					{
						signal = 0;
					}
					else {
						signal = 1;
					}
					cout << signal << endl;
					cout << index << endl;
					if (signal == 1)
					{
						SOCKET sendSock = cln_socket[index];		//找到转发接收客户端的socket		
						char* part_message = buf + position + 4;		//从第4位开始截取转发信息
						string partmessage = part_message;
						char new_message[MAXLEN];
						string newmessage = "#From ";		//从第4位之后开始截取，编辑新的消息，起始识别符#
						newmessage += temp_c;
						newmessage += " messeage: " + partmessage + '$';

						strcpy(new_message, newmessage.c_str());
						int sign = send(sendSock, new_message, strlen(new_message) + sizeof(char), NULL); //发送信息至目标客户端
						if (sign == SOCKET_ERROR) //确认信息是否成功发送
						{
							strcpy(confirm, "#Error: Failed to send the message$");
						}
						else {
							strcpy(confirm, "#Sending the message successfully$");
						}
						send(rcvSock, confirm, strlen(confirm) + sizeof(char), NULL);  //发送确认的信息回原客户端
					}
					else {
						strcpy(confirm, "#Error: Failed to send the message. No target client.$");
						send(rcvSock, confirm, strlen(confirm) + sizeof(char), NULL);  //发送确认的信息回原客户端
					}
					break;
				}
				default:
					char info_default[MAXLEN] = "#Error: Invalid command$";  //出错原因为命令无效
					send(rcvSock, info_default, strlen(info_default) + sizeof(char), NULL);
					break;
				}
			}
		}
		else {
			return 0;
		}
	}
}