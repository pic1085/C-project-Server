#include "handle.h"
#include "user_membership.h"
#include "set_user_data.h"
#include "login.h"
#include "userlistprint.h"
#include "userfilelistprint.h"
#include "create_file.h"
#include "download.h"

// 전역 변수 선언 (필요한 파일에서 정의)
extern SOCKET cliSocks[10];
extern HANDLE mtxMutex;
extern int clicount;
extern User* head;

unsigned _stdcall Handle(void* arg) {
    SOCKET cliSock = *(SOCKET*)arg;
    printf("Client connected: [%d]\n", cliSock); // 연결된 클라이언트 출력
    char recvmsg[BUF_SIZE] = ""; // 받을 문자열을 저장할 버퍼 생성
    char sendmsg[BUF_SIZE] = ""; // 보낼 문자열을 저장할 버퍼 생성
    int string = 0; // 문자열 길이를 측정할 변수 

    while (1) {
        // recvmsg 버퍼 초기화 
        memset(recvmsg, 0, sizeof(recvmsg));
        string = recv(cliSock, recvmsg, BUF_SIZE - 1, 0); // 받은 문자의 크기 
        if (string <= 0) {
            // recv 함수가 오류를 반환하거나 클라이언트가 연결을 끊었을 경우
            printf("Client-[%d] Disconnected\n", cliSock);
            break;
        }
        recvmsg[string] = '\0'; // 받은 문자의 끝에 널문자를 추가하여 문장으로 변경
        printf("Client-[%d] Send Message : %s\n", cliSock, recvmsg); // 클라이언트가 보낸 메시지 출력

        if (strcmp(recvmsg, "exit") == 0) { // exit가 수신되면
            printf("Client-[%d] Exit\n", cliSock); // 어느 클라이언트가 나갔는지 출력
            send(cliSock, "exit", strlen("exit"), 0); // exit를 송신
            break; // 종료
        }
        else if (strcmp(recvmsg, "login") == 0 || strcmp(recvmsg, "1") == 0) { // login or 1을 입력시 로그인 기능 수행
            login(&head, cliSock); // 로그인 기능 수행 
        }
        else if (strcmp(recvmsg, "join") == 0 || strcmp(recvmsg, "2") == 0) { // join or 2를 입력시 회원가입 기능 수행
            UserMembership(&head, cliSock); // 회원가입 함수에 저장한 값을 전달 
        }
        else if (strcmp(recvmsg, "userlist") == 0) { // userlist를 전달 받을 시 
            userlistprint(cliSock); // 유저의 목록을 출력함 
        }
        else if (strcmp(recvmsg, "filelist") == 0) { // filelist를 받을 시 
            userfilelistprint(cliSock); // 입력받은 유저의 파일 리스트를 출력
        }
        else if (strcmp(recvmsg, "upload") == 0) { // upload를 전달 받을 시 
            create_file(&head, cliSock); // 파일 업로드 기능 수행
        }
        else if (strcmp(recvmsg, "download") == 0) { // download 전달 받을 시 
            Download(cliSock); // download 기능 수행 
        }
        else { // 잘못 입력 받았을 시 
            sprintf(sendmsg, "Input error try again"); // 에러 메시지를 sendmsg에 저장
            send(cliSock, sendmsg, strlen(sendmsg), 0); // client에 전달함 
        }
    }

    // 클라이언트 연결 종료 처리
    WaitForSingleObject(mtxMutex, INFINITE); // 뮤텍스 잠금
    for (int i = 0; i < clicount; i++) {
        if (cliSock == cliSocks[i]) {
            while (i < clicount - 1) {
                cliSocks[i] = cliSocks[i + 1];
                i++;
            }
            break;
        }
    }
    clicount--;
    ReleaseMutex(mtxMutex); // 뮤텍스 해제
    closesocket(cliSock); // 소켓 종료

    return 0;
}
