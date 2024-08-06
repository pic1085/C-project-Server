#include "load_user_file_data.h"

void loadUserFileData(User* head) { // 유저의 파일 목록 리스트를 호출하는 함수
    FILE* fp = fopen("FileInfo.txt", "r"); // 파일을 r모드로 오픈
    if (fp == NULL) { // 파일이 없다면
        printf("File open error....\n"); // 파일 오픈 에러
        return;
    }

    char userID[10]; // 유저의 이름을 담을 변수 선언
    int fileCount; // 파일의 갯수를 담을 변수 선언
    while (fscanf(fp, "%s %d", userID, &fileCount) != EOF) { // 유저의 ID와 파일의 갯수를 읽어옴
        User* current = head; // 연결리스트 호출
        while (current != NULL && strcmp(current->ID, userID) != 0) { // 유저를 찾을때까지 반복
            current = current->next; // 다음 노드로 이동
        }

        if (current != NULL) {
            current->file_index = fileCount; // 파일 수를 대입
            current->FileName = (char**)malloc(fileCount * sizeof(char*)); // 파일 이름을 읽을 메모리 할당
            for (int i = 0; i < fileCount; i++) { // 파일의 갯수만큼 반복
                current->FileName[i] = (char*)malloc(BUF_SIZE * sizeof(char)); // 파일의 이름을 저장할 메모리 할당 
                fscanf(fp, "%s", current->FileName[i]); // 파일의 이름을 저장
            }
        }
    }

    fclose(fp); // 파일 닫기
}
