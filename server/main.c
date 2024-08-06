#include "server.h"
#include "handle.h"
#include "user_membership.h"
#include "set_user_data.h"
#include "login.h"
#include "userlistprint.h"
#include "userfilelistprint.h"
#include "create_file.h"
#include "download.h"
#include "load_user_file_data.h"
#include "save_user_file_data.h"

SOCKET cliSocks[10];
HANDLE mtxMutex;
int clicount = 0;
User* head = NULL;

int main() {
    SetConsoleOutputCP(CP_UTF8); // 파일이 깨져서 추가한 함수 
    FILE* fp; // 파일 선언 
    WSADATA wsa;
    SOCKET serSock, cliSock;
    SOCKADDR_IN serAddr, cliAddr;
    HANDLE HandleThread;
    int cliAddrSize;
    int portNum = 8080; // 포트번호 고정

    printf("User Uploading....\n");  // 유저 업로딩하기
    if ((fp = fopen("UserInfo.txt", "r")) == NULL) { // 파일이 없을 시 
        printf("User Uploading error\n"); // 에러 출력
        exit(-1); // 종료 
    }

    printf("File List Uploading...\n");

    head = setUserData(fp); // 연결리스트 호출
    loadUserFileData(head);

    printf("FIle List Uploading SUCCESS\n");

    fclose(fp); // 파일 닫기 

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { // 윈속 라이브러리 초기화
        printf("WSAStartup error\n"); //실패시 error 반환
    }

    mtxMutex = CreateMutex(NULL, FALSE, NULL); //뮤텍스 생성 보안속성, 초기 소유권, 이름 순
    if (mtxMutex == NULL) {
        printf("CreateMutex error\n"); // 실패시 에러 반환
    }

    serSock = socket(PF_INET, SOCK_STREAM, 0); //서버 소켓 생성 
    if (serSock < 0) {
        printf("serSock error\n"); // 소켓 생성 실패시 에러 반환
    }

    memset(&serAddr, 0, sizeof(serAddr)); // 주소 초기화
    serAddr.sin_family = AF_INET; // IPV4를 사용함
    serAddr.sin_port = htons(portNum); // 포트의 값은 8080 htons를 사용하여 포트의 값을 변환
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl을 사용하여 값을 변환

    if (bind(serSock, (SOCKADDR*)&serAddr, sizeof(serAddr)) != 0) { // 서버 소켓에 주소 할당
        printf("serSock bind error\n"); // 실패시 에러 반환
    }

    if (listen(serSock, 4) != 0) { // 클라이언트 요청 대기
        printf("serSock listen error\n"); // 실패시 에러 반환
    }
    while (1) {
        cliAddrSize = sizeof(cliAddr); // 클라이언트 주소 크기 설정
        cliSock = accept(serSock, (SOCKADDR*)&cliAddr, &cliAddrSize); // 클라이언트 접속 수락
        if (cliSock == INVALID_SOCKET) {
            printf("cliSock accept error\n"); // 실패시 에러 반환
        }
        WaitForSingleObject(mtxMutex, INFINITE); // 뮤텍스로 클라이언트 공유되는 데이터 보호
        cliSocks[clicount++] = cliSock; // 클라이언트 소켓을 배열에 정리함
        ReleaseMutex(mtxMutex); // 뮤텍스 해제
        HandleThread = (HANDLE)_beginthreadex(NULL, 0, Handle, (void*)&cliSock, 0, NULL); // 쓰레드 생성
        if (HandleThread == NULL) { // 쓰레드를 생성하지 못하였을 시 
            printf("Thread error\n"); // 스레드 생성 실패시 에러 반환
        }
        printf("client join!! \n"); // 클라이언트의 접속을 알림 
    }
    closesocket(serSock); // 소켓을 종료
    WSACleanup();
    return 0;
}
