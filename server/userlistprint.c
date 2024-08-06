#include "userlistprint.h"

void userlistprint(SOCKET cliSock) { //모든 유저의 정보를 출력
    User* current = head; // 연결리스트 호출
    char result[BUF_SIZE] = ""; // 버퍼 초기화

    sprintf(result, "\tUser list\t\n");
    send(cliSock, result, strlen(result), 0);

    while (current != NULL) { // 연결리스트의 끝까지 반복
        sprintf(result, "%s", current->ID); // 유저의 ID를 result에 대입 
        send(cliSock, result, strlen(result), 0); // client에게 값을 전달
        Sleep(10);    // 넣지 않으면 정보가 섞여서 보내짐 
        current = current->next; // 다음 노드로 이동
    }
    current = head;
    sprintf(result, "User information output success\n");
    send(cliSock, result, strlen(result), 0);
}
