#include "save_user_file_data.h"

void saveUserFileData(User* head) { // 유저의 파일 목록 리스트를 저장하는 함수
    FILE* fp = fopen("FileInfo.txt", "w"); // 파일을 w모드로 오픈 
    if (fp == NULL) { // 파일이 없다면
        printf("File open error....\n"); // 파일 오픈 에러
        return;
    }

    User* current = head; // 연결리스트 호출
    while (current != NULL) { // 연결리스트의 끝까지 반복
        fprintf(fp, "%s %d\n", current->ID, current->file_index); // 유저의 ID와 파일의 갯수를 작성
        for (int i = 0; i < current->file_index; i++) { // 파일의 갯수만큼 반복
            fprintf(fp, "%s\n", current->FileName[i]); // 파일의 이름을 저장
        }
        current = current->next; // 다음 노드로 이동
    }
     
    fclose(fp); // 파일 닫기 
}
