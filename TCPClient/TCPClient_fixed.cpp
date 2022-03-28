// TCPClient.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//������ �׽�Ʈ �ּ�
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

//�����Լ� ����
void err_quit() {
    exit(1);
}

void err_display() {
    printf("����");
}

//�������ִ� recv ��ſ� ����� ���� ���� �ռ� (recvn)
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;       //���ź���
    char* ptr = buf;    //������ ���� ��ġ
    int left = len;     //���� ���� ���� ��

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }
        else if (received == 0) {
            break;
        }
        left -= received;   //������ ���� ���� ���� ����
        ptr += received;    //������ ptr ����
    }
    return (len - left);    //��ü - ���� �ڸ��� = ���� ���� ����
}

int main()
{
    int retval; //Ŭ���̾�Ʈ ���� ó�� ���

    //1. ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    //2. ���� ����
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {           //���� ó��
        err_quit();
    }
    //3. connect -> ���� IP, ��Ʈ ��ȣ�� ���� �õ�
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP); //127.0.0.1
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        err_quit();
    }

    //4. send
    char buf[BUFSIZE];
    char * testdata[] = {
        "�ȳ��ϼ���",
        "�ݰ����ϴ�",
        "�ȳ����輼��"
    };
    
    for (int i = 0; i < 4; i++) {
        //���� ������ �غ�
        memset(buf, '#', sizeof(buf));
        //memset(��ġ, ��ȣ, ũ��) : ��ġ ũ�⸸ŭ ��ȣ�� ����
        strncpy(buf, testdata[i], strlen(testdata[i]));
        //testdata[i]�� ���ۿ� ���� 
        retval = send(sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display();
            break;
        }
        printf("TCP Ŭ���̾�Ʈ %d byte ���� \n", retval);

        //    //5. recv
        //    retval = recvn(sock, buf, retval, 0);       // ����� ���� ���� �Լ�
        //    if (retval == SOCKET_ERROR) {
        //        err_display();
        //        break;
        //    }
        //    else if (retval == 0) {
        //        break;
        //    }
        //    buf[retval] = '\0';
        //    printf("TCP Ŭ���̾�Ʈ %d byte ���� \n", retval);
        //    printf("���������� %s \n", buf);
        //}
    }
    //6. ����
    closesocket(sock);
    WSACleanup();

}
