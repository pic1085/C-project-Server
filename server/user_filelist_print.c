#include "userfilelistprint.h"

void userfilelistprint(SOCKET cliSock) { // 파일 목록을 출력하는 함수
    User* current = head; // 리스트 호출
    char result[BUF_SIZE] = ""; // 버퍼 초기화
    char userID[BUF_SIZE] = ""; // 버퍼 초기화
    int fount = 0;

    int string = recv(cliSock, userID, BUF_SIZE - 1, 0); // userID에 값을 전달받고 크기를 구함
    userID[string] = '\0'; // 문장으로 만듬
    sprintf(result, "\tFile list\t\n");
    send(cliSock, result, strlen(result), 0);

    while (current != NULL) { // 리스트의 끝까지 반복
        if (strcmp(current->ID, userID) == 0) { // 이름이 같을 시 
            for (int i = 0; i < current->file_index; i++) { // 해당 유저의 모든 파일을 출력
                sprintf(result, "%s", current->FileName[i]); // result에 파일의 이름을 대입
                send(cliSock, result, strlen(result), 0); // client에게 result 전달 
                Sleep(10);
                fount = 1;
            }

            break;
        }
        current = current->next; // 다음 노드로 이동
    }

    if (fount == 0) {
        sprintf(result, "NO DATA");
        send(cliSock, result, strlen(result), 0);
    }
    current = head;
    return;
}
