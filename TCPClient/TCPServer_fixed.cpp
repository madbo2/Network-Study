// TCPServer2.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

//��Ʈ ��ȣ, ũ��
#define SERVERPORT 9000
#define BUFSIZE 50

//���� ���� ��������
void err_quit(const char* msg) {
    exit(1);
}

void err_display() {
    printf("����");
}
//�����͸� �о���̴� �κ��� �Լ� ����
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;           //������ ������
    char* ptr = buf;        //������ ������ġ
    int left = len;         //�����ʴ� ����
    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }
        else if (received == 0) {
            break;
        } 
        left -= received;
        ptr += received;
    }
    return(len - left);

}

int main()
{
    int retval; // ����ó����� ����

    //���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    //���� ����
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    //socket(�ּ�ü��,����Ÿ��,��������)
    //AF_INET = IPv4
    //SOCK_STREAM = TCP
    //0 = ��������
    if (listen_sock == INVALID_SOCKET) {
        err_quit("socket()����");
    }

    //���� IP�� ��Ʈ��ȣ ����(bind)
    SOCKADDR_IN serveraddr; //SOCKETADDR_IN : ipv4
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    //�ּ� ��������� 0 ���� �ʱ�ȭ
    serveraddr.sin_family = AF_INET; // IPV4 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // INADDR_ANY : Ŭ���̾�Ʈ���� ���ӽ� �ڵ�����
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    // bind(����, �ּ� , ũ�� )
    if (retval == SOCKET_ERROR) {
        err_quit("bind����");
    }

    // TCP ȯ�濡�� LISTENING ��� ���·� ����
    retval = listen(listen_sock, SOMAXCONN);
    //listen (��������, Ŭ�󸮾�Ʈ ���� ���� (SOMAXCONN : �ִ�ġ))

    //accept�� ���� �غ� -> ��ſ� ����� ����
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1) {
        addrlen = sizeof(clientaddr);
        // accpet -> Ŭ���̾�Ʈ Connect ����
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display();
            break;
        }
        //������ Ŭ���̾�Ʈ ���
        printf("\n[TCP������ Ŭ���̾�Ʈ ���� : IP=%s, ��Ʈ��ȣ = %d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        while (1) {
            // recv ==> Ŭ���̾�Ʈ���� send�� ������ ����
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display();
                break;
            }
            else if (retval == 0) {
                break;
            }
            //���� ������ ���
            buf[retval] = '\0';
            printf("\n[TCP data IP=%s, ��Ʈ��ȣ = %d , ���� ������ %s \n",
                inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

            
        }
        //Ŭ���̾�Ʈ ���� ����
        closesocket(client_sock);
        printf("\n[TCP ���� Ŭ���̾�Ʈ ���� IP=%s, ��Ʈ��ȣ = %d \n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    } //��������
    closesocket(listen_sock);
    WSACleanup();
}
