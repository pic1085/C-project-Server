#include "user_membership.h"

void UserMembership(User** head, SOCKET cliSock) { // 회원가입 함수
    FILE* fp;
    int string; // 수신받은 문자의 길이
    char sendmsg[BUF_SIZE] = ""; // 버퍼 초기화
    char recvmsg[BUF_SIZE] = ""; // 버퍼 초기화
    User* current = *head; // current 는 head의 값을 가짐 
    User* newUser = (User*)malloc(sizeof(User)); // 새로운 노드 생성

    if (newUser == NULL) {
        perror("메모리 할당 오류");
        exit(EXIT_FAILURE);
    }

    string = recv(cliSock, recvmsg, BUF_SIZE - 1, 0); // recvmsg에 전달받은 값을 넣음
    if (string < 0) {
        perror("수신 오류");
        free(newUser);
        return;
    }
    recvmsg[string] = '\0'; // recvmsg의 끝에 널문자를 추가하여 문장으로 변경

    char username[BUF_SIZE] = ""; // 버퍼 초기화
    char userpassword[BUF_SIZE] = ""; // 버퍼 초기화
    sscanf(recvmsg, "%s %s", username, userpassword); // 전달받은 값을 username과 userpassword에 저장함 

    // 입력값 검증
    if (strlen(username) <= 0 || strlen(username) > 10 || strlen(userpassword) <= 0 || strlen(userpassword) > 20) {
        sprintf(sendmsg, "input error");
        send(cliSock, sendmsg, strlen(sendmsg), 0);
        free(newUser);
        return;
    }

    // 사용자 정보 설정
    strncpy(newUser->ID, username, sizeof(newUser->ID) - 1); // 안전한 복사
    newUser->ID[sizeof(newUser->ID) - 1] = '\0'; // 널문자 추가
    strncpy(newUser->PassWord, userpassword, sizeof(newUser->PassWord) - 1); // 안전한 복사
    newUser->PassWord[sizeof(newUser->PassWord) - 1] = '\0'; // 널문자 추가
    newUser->next = NULL;
    newUser->file_index = 0;

    // FileName 배열 초기화
    newUser->FileName = (char**)malloc(MAX_FILES * sizeof(char*));
    if (newUser->FileName == NULL) {
        perror("FileName 메모리 할당 오류");
        free(newUser);
        return;
    }
    for (int i = 0; i < MAX_FILES; ++i) {
        newUser->FileName[i] = (char*)malloc(BUF_SIZE * sizeof(char));
        if (newUser->FileName[i] == NULL) {
            perror("FileName 배열 요소 메모리 할당 오류");
            for (int j = 0; j < i; ++j) {
                free(newUser->FileName[j]);
            }
            free(newUser->FileName);
            free(newUser);
            return;
        }
    }

    // 이미 존재하는 아이디인지 확인
    while (current != NULL) {
        if (strcmp(current->ID, username) == 0) {
            sprintf(sendmsg, "ID Already Exists\n");
            send(cliSock, sendmsg, strlen(sendmsg), 0);
            for (int i = 0; i < MAX_FILES; ++i) {
                free(newUser->FileName[i]);
            }
            free(newUser->FileName);
            free(newUser);
            return;
        }
        current = current->next;
    }

    // 연결 리스트에 새로운 사용자 추가
    if (*head == NULL) {
        *head = newUser; // 리스트가 비어있을 경우 첫 번째 노드로 설정
    }
    else {
        current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newUser; // 리스트의 끝에 새로운 유저를 추가
    }

    // 파일에 사용자 정보 추가
    fp = fopen("UserInfo.txt", "a");
    if (fp == NULL) {
        perror("파일 열기 오류");
        for (int i = 0; i < MAX_FILES; ++i) {
            free(newUser->FileName[i]);
        }
        free(newUser->FileName);
        free(newUser);
        return;
    }
    fprintf(fp, "%s %s\n", username, userpassword);
    fclose(fp);

    printf("%d", newUser->file_index);
    // 회원가입 성공 메시지 전송
    sprintf(sendmsg, "Join SUCCESS");
    send(cliSock, sendmsg, strlen(sendmsg), 0);
    printf("%d Join SUCCESS\n", cliSock);

    return;
}
