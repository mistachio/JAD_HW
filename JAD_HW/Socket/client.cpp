#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include<windows.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll


#define MAX_NUMBER 200
#define MAX_THREAD_NUMBER 9
DWORD WINAPI receive_func(LPVOID lpParameter);
DWORD WINAPI create_client(LPVOID lpParameter);

DWORD WINAPI receive_func(LPVOID lpParameter)
{
	unsigned int *p1 = (unsigned int*)lpParameter;
	SOCKET sock = *p1;    //ǿ������ת��
	while (1)
	{
		//���շ��������ص�����
		char recv_pack[MAX_NUMBER] = { 0 };
		recv(sock, recv_pack, MAX_NUMBER, NULL);    //���ܷ�����Ϣ��
		printf("���Կͻ��˵���Ϣ:\n");
		int k = 0;	//����
		while (recv_pack[k] != '#'&& k < MAX_NUMBER)
		{
			k++;
		}
		while (recv_pack[k + 1] != '$'&& k < MAX_NUMBER)
		{
			k++;
			printf("%c", recv_pack[k]);    //��ӡ����
		}
		printf("\n");
		printf("\n");
	}

	return 0;
}
DWORD WINAPI create_client(LPVOID lpParameter)    //numberΪ�ͻ��˵ı��
{
	int *p = (int*)lpParameter;
	int number = *p;    //ǿ������ת��
	//��ʼ��DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//������
	int choose, k, position;
	char IP_address[50] = { 0 };
	char Port_number[10] = { 0 };
	char send_pack[MAX_NUMBER] = { 0 };
	char temp[20] = { 0 };
	int connect_symbol = 0;    //��ʶ��
	int exit_symbol = 0;
	int list_symbol = 0;
	HANDLE child_thread;

	//�����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;    //ʹ��IPV4��ַ

	while (!exit_symbol)
	{
		//�˵����ܵ�ʵ��

		memset(send_pack, '\0', MAX_NUMBER);
		if (connect_symbol == 0)    //δ����״̬
		{
			printf("client %d menu: 0.connect 1.exit\n�����빦�ܵ����ֱ��\n", number);
			scanf_s("%d", &choose);
			if (choose == 0)
			{
				printf("client %d:������IP��ַ�Ͷ˿ں�\n", number);
				printf("10.180.47.254\n");
				printf("3113\n");
				//gets_s(IP_address);    //����ͻ���IP��ַ
				//gets_s(Port_number);    //����ͻ��˵Ķ˿ں�
				sockAddr.sin_addr.s_addr = inet_addr("10.180.79.196");    //IP_address
				sockAddr.sin_port = htons(3113);    //(int)Port_number
				connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));    //���������������
				child_thread = CreateThread(NULL, 0, receive_func, &sock, 0, NULL);   //�������̣߳���ʵ�ֽ��ܷ������ݵĹ���
				printf("������!\n");

				connect_symbol = 1;    //�趨��ʶ��Ϊ������״̬
			}
			else if (choose == 1)
			{
				exit_symbol = 1;
			}
			else
			{
				printf("��������ȷ���ֱ��!\n");
			}
		}
		else    //������״̬
		{
			//��װ���ݰ�
			send_pack[0] = '#';    //��#��Ϊ��ʼ��ʶ��
			char temp[1];
			itoa(number, temp, 10);
			send_pack[1] = temp[0];    //��һ������Ϊ���
			send_pack[3] = '$';    //��$��Ϊ������ʶ��

			printf("menu: 0.disconnect 1.exit 2.get now time 3.get name 4.get client list 5.send message \n�����빦�ܵ����ֱ��\n");
			scanf_s("%d", &choose);    //����û���ѡ��ѡ��
			if (choose == 0)
			{
				send_pack[2] = '0';    //�Ͽ�����������ڶ�������Ϊ0
				send(sock, send_pack, MAX_NUMBER, 0);    //�����������ݰ�
				connect_symbol = 0;
				CloseHandle(child_thread);    //�ر����߳�

			}
			else if (choose == 1)
			{
				send_pack[2] = '0';    //�Ͽ�����������ڶ�������Ϊ0
				send(sock, send_pack, MAX_NUMBER, 0);    //�����������ݰ�
				exit_symbol = 1;
			}
			else if (choose == 2)
			{
				send_pack[2] = '2';    //time������ڶ�������Ϊ2
				send(sock, send_pack, MAX_NUMBER, 0);    //�����������ݰ�
			}
			else if (choose == 3)
			{
				send_pack[2] = '3';    //name������ڶ�������Ϊ3
				send(sock, send_pack, MAX_NUMBER, 0);    //�����������ݰ�
			}
			else if (choose == 4)
			{
				send_pack[2] = '4';    //list������ڶ�������Ϊ4
				send(sock, send_pack, MAX_NUMBER, 0);    //�����������ݰ�
				list_symbol = 1;
			}
			else if (choose == 5)
			{
				if (list_symbol == 0)
				{
					printf("δ��ÿͻ����б�!\n");
				}
				else
				{
					send_pack[2] = '5';    //message������ڶ�������Ϊ5
					char target_client_number;    //�����Ϊ9�������õ����ַ�����
					char send_message[MAX_NUMBER-4];
					getchar();
					scanf_s("%c", &target_client_number);
					getchar();
					scanf("%s", send_message);
					send_pack[3] = target_client_number;
					strcat(&send_pack[4], send_message);
					send_pack[5 + strlen(send_message)] = '$';    //��$��Ϊpcak�Ľ�����ʶ��
					send(sock, send_pack, MAX_NUMBER, 0);    //�����������ݰ�

				}
			}
			else
			{
				printf("��������ȷ���ֱ��!\n");
			}
		}
		Sleep(1000);
	}
	closesocket(sock);  //�ر��׽���
	WSACleanup();  //��ֹʹ�� DLL

	return 0;
}
int main()
{
	HANDLE hThread[MAX_THREAD_NUMBER];
	int client_number;
	printf("������ͻ�������\n");
	scanf_s("%d", &client_number);    //����ͻ�������
	int i;
	for (i = 0; i < client_number; i++)
	{
		hThread[i] = CreateThread(NULL, 0, create_client, &i, 0, NULL);    //��ͬ���̴߳�����ͬ�ͻ���   ���������⣡
	}

	Sleep(300000);    //�ȴ�30��
	printf("���س�����!\n");
	system("pause");    //������Ļ�����ڹ۲�

	for (int i = 0; i < client_number; i++)
	{
		CloseHandle(hThread[i]);    //�ر��������߳�
	}

	return 0;
}
