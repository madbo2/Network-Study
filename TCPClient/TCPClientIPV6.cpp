// TCPClient.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>

//������ �׽�Ʈ �ּ�
#define SERVERIP "::1"
#define SERVERPORT 9000
#define BUFSIZE 512

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
    SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {           //���� ó��
        err_quit();
    }
    //3. connect -> ���� IP, ��Ʈ ��ȣ�� ���� �õ�
    SOCKADDR_IN6 serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    int addrlen = sizeof(serveraddr);
    WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR*)&serveraddr, &addrlen);
    //serveraddr.sin_addr.s_addr = inet_addr(SERVERIP); //127.0.0.1
    serveraddr.sin6_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        err_quit();
    }

    //4. send
    char buf[BUFSIZE + 1];
    int len;
    while (1) {
        printf("\n [����������] :");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
            break;
        }
        len = strlen(buf);      //�Էµ� ���� ���� ����
        if (buf[len - 1] == '\n') {
            buf[len - 1] = '\0';        //enter�� ������ enter�ڸ��� 0���� ó��
        }
        if (strlen(buf) == 0) {
            break;
        }
        retval = send(sock, buf, strlen(buf), 0);
        if (retval == SOCKET_ERROR) {
            err_display();
            break;
        }
        printf("TCP Ŭ���̾�Ʈ %d byte ���� \n", retval);

        //5. recv
        retval = recvn(sock, buf, retval, 0);       // ����� ���� ���� �Լ�
        if (retval == SOCKET_ERROR) {
            err_display();
            break;
        }
        else if (retval == 0) {
            break;
        }
        buf[retval] = '\0';
        printf("TCP Ŭ���̾�Ʈ %d byte ���� \n", retval);
        printf("���������� %s \n", buf);
    }

    //6. ����
    closesocket(sock);
    WSACleanup();

}
