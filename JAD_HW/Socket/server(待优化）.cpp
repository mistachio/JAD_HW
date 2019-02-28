#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll

using namespace std;
#define MAXLEN 200
#define MAX_THRD 6
#define MAX_THREAD_NUMBER 9

DWORD WINAPI SubThread(LPVOID lpParam);

vector<string> cln_list;    //��ַ�б�
vector<int> num_of_addr;    //���еĵ�ַ���
vector<SOCKET> cln_socket;  //���յ��Ŀͻ���socket

int main() {
	//��ʼ�� DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���׽���
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("10.110.51.110");  //�����IP��ַ
	sockAddr.sin_port = htons(3113);  //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//�������״̬
	listen(servSock, 20);

	//���տͻ�������
	SOCKADDR clntAddr;
	int k = 0;
	SOCKET clntSock[MAX_THREAD_NUMBER];    //���9��clntSock�׽���
	clntSock[k] = INVALID_SOCKET;
	HANDLE HDL[MAX_THREAD_NUMBER];    //������
	int nSize = sizeof(SOCKADDR);
	int exit_symbol = 0;    //�˳���ʶ
	while ((k<MAX_THREAD_NUMBER) && (!exit_symbol))    //���ӵ����ͻ�����,�����յ�exit��Ϣ���˳�
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

		if ((DWORD)(HDL[k] = CreateThread(NULL, 0, SubThread, &clntSock[k], 0, NULL)) == 1)    //�˳�ָ��
		{
			exit_symbol = 1;
		}

		k++;
		clntSock[k] = INVALID_SOCKET;
	}

	cout << "Finished!" << endl;
	for (int i = 0; i < cln_socket.size(); i++)    //�ر�cln_socket�׽���
	{
		closesocket(cln_socket[i]);
	}

	//�ر��׽���
	while (k != 0)
	{
		k--;
		closesocket(clntSock[k]);
		CloseHandle(HDL[k]);
	}
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
	WSACleanup();
	system("pause");    //������Ļ�����ڹ۲�

	return 0;


}

DWORD WINAPI SubThread(LPVOID lpParam) {

	//���������
	int rcv = SOCKET_ERROR; //�����ʼ����״̬Ϊʧ��
	char buf[MAXLEN];		//������

	SOCKADDR rcvAddr;
	int nSize = sizeof(SOCKADDR);

	unsigned int *p = (unsigned int*)lpParam;
	SOCKET rcvSock = *p;    //һϵ������ת��
	string str = "#Hello$";
	char cstr[MAXLEN];
	int flag = 0;

	strcpy(cstr, str.c_str());
	send(rcvSock, cstr, str.length() + sizeof(char), NULL);  //��ͻ��˷���hello

	while (true)	//ѭ����ȡ
	{
		memset(buf, '\0', MAXLEN);  //���û�����
		rcv = recv(rcvSock, buf, MAXLEN, NULL);  //receive�������ݰ�
		cout << "circle!" << endl;
		if (rcv != SOCKET_ERROR)
		{
			int position = 0;
			int finish_point = 0;
			while (buf[position] != '#' && position<MAXLEN)  //�ҵ�������Ϣ����ʼ����#
			{
				position++;
			}
			if (position < MAXLEN - 1 && flag == 0)
			{
				int num = (int)(buf[position + 1] - '0');   //���б�ʾ�ͻ��˵ı��
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
			while (buf[finish_point] != '$' && position < MAXLEN)   //�ҵ����еĽ�������$
			{
				finish_point++;
			}
			if (position < finish_point && finish_point < MAXLEN) //��������ҵ���ʼ���ͽ�����������һ�����������ݰ����������²���
			{
				switch (buf[position + 2])
				{
				case '0':    //�ӵ�0��ʶ���Ͽ����ӣ��ر����߳�
				{
					cout << "disconnect!" << endl;
					return 0;	//����ѭ���ص����߳��йر�
					break;
				}
				case '1':    //�ӵ�1��ʶ���Ͽ����������̣߳��ر����߳�
				{
					cout << "exit all!" << endl;
					return 1;	//����ѭ���ص����߳��йر�
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
					send(rcvSock, ch_tm, strlen(ch_tm) + sizeof(char), NULL); //����ʱ���
					break;
				}
				case '3':
				{
					char name[MAXLEN] = "#server name is: server 2.0$";
					send(rcvSock, name, strlen(name) + sizeof(char), NULL); //�������ְ�
					break;
				}
				case '4':
				{
					char temp_c = buf[position + 1];    //�����ͻ��˱�� 
					string s_list = "#client number: ";
					s_list += temp_c;
					s_list += "  IP: ";
					for (int i = 0; i < cln_list.size(); i++)
					{
						if (num_of_addr[i] != 0) {
							char c = num_of_addr[i] + '0';
							s_list += cln_list[i] + ';';  //����б��еĿͻ���ip����"_"��Ϊ�ָ���
						}
					}
					s_list += '$'; //��ӽ�����
					char list[MAXLEN];
					strcpy(list, s_list.c_str());
					send(rcvSock, list, strlen(list) + sizeof(char), NULL); //���Ϳͻ����б��
					break;
				}
				case '5':
				{
					char temp_c = buf[position + 1];    //�����ͻ��˱��

					char ch_num = buf[position + 3];    //��4λ�洢����Ŀ��ͻ��˱��
					int num = ch_num - '0';
					int index = 0;
					while (index < num_of_addr.size() && num_of_addr[index] != num) {
						//��num_of_addr���ҵ�������
						index++;
					}
					int signal;                  //�����жϵ����ն˵�socket�����Ƿ����
					char confirm[MAXLEN];      //���ͻ�ԭ�ͻ��˵ķ����ź�
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
						SOCKET sendSock = cln_socket[index];		//�ҵ�ת�����տͻ��˵�socket		
						char* part_message = buf + position + 4;		//�ӵ�4λ��ʼ��ȡת����Ϣ
						string partmessage = part_message;
						char new_message[MAXLEN];
						string newmessage = "#From ";		//�ӵ�4λ֮��ʼ��ȡ���༭�µ���Ϣ����ʼʶ���#
						newmessage += temp_c;
						newmessage += " messeage: " + partmessage + '$';

						strcpy(new_message, newmessage.c_str());
						int sign = send(sendSock, new_message, strlen(new_message) + sizeof(char), NULL); //������Ϣ��Ŀ��ͻ���
						if (sign == SOCKET_ERROR) //ȷ����Ϣ�Ƿ�ɹ�����
						{
							strcpy(confirm, "#Error: Failed to send the message$");
						}
						else {
							strcpy(confirm, "#Sending the message successfully$");
						}
						send(rcvSock, confirm, strlen(confirm) + sizeof(char), NULL);  //����ȷ�ϵ���Ϣ��ԭ�ͻ���
					}
					else {
						strcpy(confirm, "#Error: Failed to send the message. No target client.$");
						send(rcvSock, confirm, strlen(confirm) + sizeof(char), NULL);  //����ȷ�ϵ���Ϣ��ԭ�ͻ���
					}
					break;
				}
				default:
					char info_default[MAXLEN] = "#Error: Invalid command$";  //����ԭ��Ϊ������Ч
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