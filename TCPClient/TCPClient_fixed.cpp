// TCPClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//루프백 테스트 주소
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

//소켓함수 오류
void err_quit() {
    exit(1);
}

void err_display() {
    printf("오류");
}

//제공해주는 recv 대신에 사용자 정의 수신 합수 (recvn)
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;       //수신변수
    char* ptr = buf;    //버퍼의 시작 위치
    int left = len;     //읽지 않은 길이 값

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }
        else if (received == 0) {
            break;
        }
        left -= received;   //읽으면 읽지 않은 길이 감소
        ptr += received;    //읽으면 ptr 증가
    }
    return (len - left);    //전체 - 남은 자리수 = 읽은 값을 리턴
}

int main()
{
    int retval; //클라이언트 연결 처리 결과

    //1. 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    //2. 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {           //에러 처리
        err_quit();
    }
    //3. connect -> 서버 IP, 포트 번호로 연결 시도
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
        "안녕하세요",
        "반갑습니다",
        "안녕히계세요"
    };
    
    for (int i = 0; i < 4; i++) {
        //보낼 데이터 준비
        memset(buf, '#', sizeof(buf));
        //memset(위치, 기호, 크기) : 위치 크기만큼 기호를 삽입
        strncpy(buf, testdata[i], strlen(testdata[i]));
        //testdata[i]를 버퍼에 복사 
        retval = send(sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display();
            break;
        }
        printf("TCP 클라이언트 %d byte 전송 \n", retval);

        //    //5. recv
        //    retval = recvn(sock, buf, retval, 0);       // 사용자 정의 수신 함수
        //    if (retval == SOCKET_ERROR) {
        //        err_display();
        //        break;
        //    }
        //    else if (retval == 0) {
        //        break;
        //    }
        //    buf[retval] = '\0';
        //    printf("TCP 클라이언트 %d byte 수신 \n", retval);
        //    printf("받은데이터 %s \n", buf);
        //}
    }
    //6. 종료
    closesocket(sock);
    WSACleanup();

}
