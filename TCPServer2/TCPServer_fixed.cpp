// TCPServer2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

//포트 번호, 크기
#define SERVERPORT 9000
#define BUFSIZE 50

//소켓 오류 강제종료
void err_quit(const char* msg) {
    exit(1);
}

void err_display() {
    printf("오류");
}
//데이터를 읽어들이는 부분의 함수 정의
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;           //수신한 데이터
    char* ptr = buf;        //버퍼의 시작위치
    int left = len;         //읽지않는 길이
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
    int retval; // 서버처리결과 관리

    //윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    //소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    //socket(주소체계,소켓타입,프로토콜)
    //AF_INET = IPv4
    //SOCK_STREAM = TCP
    //0 = 프로토콜
    if (listen_sock == INVALID_SOCKET) {
        err_quit("socket()종료");
    }

    //서버 IP와 포트번호 설정(bind)
    SOCKADDR_IN serveraddr; //SOCKETADDR_IN : ipv4
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    //주소 저장공간을 0 으로 초기화
    serveraddr.sin_family = AF_INET; // IPV4 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // INADDR_ANY : 클라이언트에서 접속시 자동으로
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    // bind(소켓, 주소 , 크기 )
    if (retval == SOCKET_ERROR) {
        err_quit("bind오류");
    }

    // TCP 환경에서 LISTENING 대기 상태로 변경
    retval = listen(listen_sock, SOMAXCONN);
    //listen (소켓정보, 클라리언트 연결 개수 (SOMAXCONN : 최대치))

    //accept를 위한 준비 -> 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1) {
        addrlen = sizeof(clientaddr);
        // accpet -> 클라이언트 Connect 연결
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display();
            break;
        }
        //접속한 클라이언트 출력
        printf("\n[TCP서버에 클라이언트 접속 : IP=%s, 포트번호 = %d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        while (1) {
            // recv ==> 클라이언트에서 send한 정보를 수신
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display();
                break;
            }
            else if (retval == 0) {
                break;
            }
            //받은 데이터 출력
            buf[retval] = '\0';
            printf("\n[TCP data IP=%s, 포트번호 = %d , 받은 데이터 %s \n",
                inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

            
        }
        //클라이언트 소켓 종료
        closesocket(client_sock);
        printf("\n[TCP 서버 클라이언트 종료 IP=%s, 포트번호 = %d \n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    } //ㅁㅊㅊ뎃
    closesocket(listen_sock);
    WSACleanup();
}
