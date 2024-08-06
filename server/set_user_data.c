#include "set_user_data.h"

User* setUserData(FILE* fp) {
    char name[10]; // 임시로 값을 저장할 변수 선언
    char password[20]; // 임시로 값을 저장할 변수 선언
    User* head = NULL, * list = NULL; // 연결리스트 초기화
    fscanf(fp, "%s %s", name, password); // 각각 변수에 임시로 값을 저장함
    while (!feof(fp)) {
        head = (User*)malloc(sizeof(User)); // 동적 메모리 할당
        strcpy(head->ID, name); // 파일의 값을 대입
        strcpy(head->PassWord, password); // 파일의 값을 대입
        head->file_index = 0;

        // 파일 이름을 저장할 포인터 배열 동적 할당
        head->FileName = (char**)malloc(100 * sizeof(char*));
        for (int i = 0; i < 100; i++) {
            head->FileName[i] = (char*)malloc(BUF_SIZE * sizeof(char));
            strcpy(head->FileName[i], "");
        }

        head->next = list;
        list = head;
        fscanf(fp, "%s %s", name, password); // 임시로 값을 저장함 
    }
    printf("Uploading SUCCESS\n"); // 성공 메시지 반환
    return head;
}
