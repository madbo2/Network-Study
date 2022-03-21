// TCPServer2.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <WS2tcpip.h>

//��Ʈ ��ȣ, ũ��
#define SERVERPORT 9000
#define BUFSIZE 512

//���� ���� ��������
void err_quit(const char* msg) {
    exit(1);
}

void err_display() {
    printf("����");
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
    SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
    //socket(�ּ�ü��,����Ÿ��,��������)
    //AF_INET6 = IPv6
    //SOCK_STREAM = TCP
    //0 = ��������
    if (listen_sock == INVALID_SOCKET) {
        err_quit("socket()����");
    }

    //���� IP�� ��Ʈ��ȣ ����(bind)
    SOCKADDR_IN6 serveraddr; //SOCKETADDR_IN : ipv6
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    //�ּ� ��������� 0 ���� �ʱ�ȭ
    serveraddr.sin6_family = AF_INET6; // IPV6
    serveraddr.sin6_addr = in6addr_any; 
    // INADDR_ANY�� ���� : Ŭ���̾�Ʈ���� ���ӽ� �ڵ�����
    serveraddr.sin6_port = htons(SERVERPORT);
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
    SOCKADDR_IN6 clientaddr;
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
        char ipaddr[50];
        DWORD ipaddrlen = sizeof(ipaddr);
        WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr), NULL, ipaddr, &ipaddrlen);
        printf("\n[TCP������ Ŭ���̾�Ʈ ���� : IP=%s, ��Ʈ��ȣ = %d\n",
            ipaddr, ntohs(clientaddr.sin6_port));

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
                ipaddr, ntohs(clientaddr.sin6_port), buf);

            //������ ������
            retval = send(client_sock, buf, retval, 0);
            if (retval == SOCKET_ERROR) {
                err_display();
                break;
            }
        }
        //Ŭ���̾�Ʈ ���� ����
        closesocket(client_sock);
        printf("\n[TCP ���� Ŭ���̾�Ʈ ���� IP=%s, ��Ʈ��ȣ = %d \n",
            ipaddr, ntohs(clientaddr.sin6_port));
    } //accept
    closesocket(listen_sock);
    WSACleanup();
}
