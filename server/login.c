#include "login.h"
#include "user_directory.h"

int login(User** head, SOCKET cliSock) {  // 로그인 기능 함수
    char sendmsg[BUF_SIZE] = ""; // 버퍼 초기화
    char recvmsg[BUF_SIZE] = ""; // 버퍼 초기화
    int count = 0; // 로그인을 성공한지 판별하기 위함

    int string = recv(cliSock, recvmsg, BUF_SIZE - 1, 0); // 받은 문자의 크기 저장
    recvmsg[string] = '\0'; // recvmsg의 끝에 널문자를 추가하여 문장으로 변경
    char username[BUF_SIZE] = ""; // 버퍼 초기화
    char userpassword[BUF_SIZE] = ""; // 버퍼 초기화
    sscanf(recvmsg, "%s %s", username, userpassword); // username 과 userpassword에 값을 저장

    if ((username) <= 0 || strlen(username) > 10 || strlen(userpassword) <= 0 || strlen(userpassword) > 20) { // 크기가 다를시 fail 전송
        sprintf(sendmsg, "Login Size Exceeded\n"); // sendmsg에 에러 대입
        send(cliSock, sendmsg, strlen(sendmsg), 0); // client에 전달
        return;
    }
    User* current = *head; // 연결리스트 불러오기

    while (current != NULL) { // 연결리스트의 끝까지 읽어옴
        if (strcmp(username, current->ID) == 0 && strcmp(userpassword, current->PassWord) == 0) { // 받은 문자를 연결리스트랑 비교하여 같으면 성공
            sprintf(sendmsg, "Login SUCCESS\n"); // sendmsg에 성공 메시지 대입
            send(cliSock, sendmsg, strlen(sendmsg), 0); // client에게 전달
            printf("%d Login SUCCESS\n", cliSock);
            count += 1; // 로그인 성공  
        }
        current = current->next; // 다음 노드로 이동
    }
    current = *head; // 연결리스트의 주소가 다시 head를 가리키게 함

    if (count == 0) { // 로그인 실패시
        sprintf(sendmsg, "Login Failed Try Again\n"); // 정보가 일치하지 않다면 다시 시도하라는 문구 전송
        send(cliSock, sendmsg, strlen(sendmsg), 0); // client에게 전달
    }
    else { // 로그인 성공시 
        user_directory(username); // 해당 유저의 디렉토리 생성

    }

    return 0;
}
