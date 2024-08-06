#include "create_file.h"
#include "save_user_file_data.h"

int create_file(User** head, SOCKET cliSock) { // 파일 업로드 함수
    char file_name[BUF_SIZE] = ""; // 버퍼 초기화
    char recvmsg[BUF_SIZE] = "";    // 버퍼 초기화
    char buffer[BUF_SIZE] = "";    // 버퍼 초기화  
    char username[BUF_SIZE] = "";    // 버퍼 초기화
    char result[BUF_SIZE] = ""; // 버퍼 초기화
    FILE* file = NULL; // 파일 선언 및 초기화

    // head와 cliSock이 유효한지 확인
    if (head == NULL || *head == NULL) {
        printf("Invalid user list head pointer.\n");
        return 0;
    }
    if (cliSock == INVALID_SOCKET) {
        printf("Invalid socket.\n");
        return 0;
    }

    // 파일 이름과 사용자 이름을 받음
    int bytes_received = recv(cliSock, recvmsg, BUF_SIZE - 1, 0); // 전달받은 값의 길이를 구함
    if (bytes_received <= 0) {
        printf("NO DATA\n");
        return 0;
    }
    recvmsg[bytes_received] = '\0'; // 문자 끝에 널 문자 추가하여 문장으로 만듬
    sscanf(recvmsg, "%s %s", file_name, username); // 받은 값을 file_name과 username 각각 나누어 받음

    char user_dir[BUF_SIZE] = "C:\\server\\server"; // 서버 파일 저장 디렉토리
    char user_subdir[BUF_SIZE] = ""; // 유저별 서브 디렉토리
    snprintf(user_subdir, BUF_SIZE, "%s\\%s", user_dir, username); // 서브 디렉토리 경로 활성화

    // 디렉토리가 없으면 생성
    _mkdir(user_subdir);

    char file_path[BUF_SIZE] = ""; // 버퍼 초기화
    snprintf(file_path, BUF_SIZE, "%s\\%s", user_subdir, file_name); // file_path 에 경로를 넣음 

    bytes_received = recv(cliSock, buffer, BUF_SIZE, 0); // 클라이언트로부터 첫 번째 데이터를 수신하여 저장
    if (bytes_received <= 0) { // 데이터가 없을 시 
        printf("NO DATA\n"); // 에러 출력
        return 0;
    }

    if (strcmp(buffer, "NO DATA") == 0) { // 에러 메시지를 받을경우
        printf("No Data\n"); // 에러 출력
        return 0;
    }

    file = fopen(file_path, "wb"); // 파일을 바이너리 모드로 열음
    if (file == NULL) { // 파일이 없을경우
        perror("File Open Error\n"); // 에러 출력
        return 0;
    }

    // 파일 내용 수신하여 저장
    while (1) {
        fwrite(buffer, 1, bytes_received, file); // 첫 번째 수신한 데이터를 파일에 저장
        if (bytes_received < BUF_SIZE) { // 버퍼의 크기가 작을 시 종료 -> (마지막 데이터라서)
            break;
        }

        bytes_received = recv(cliSock, buffer, BUF_SIZE, 0);// 다음 데이터를 수신
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                // 정상적으로 연결이 종료됨
                break;
            } else {
                // 에러가 발생함
                perror("recv error");
                fclose(file);
                return 0;
            }
        }
    }

    User* current = *head; // 구조체 선언
    while (current != NULL) { // 리스트의 모든 노드를 확인
        if (strcmp(current->ID, username) == 0) { // 이름이 같을 시
            if (current->file_index < MAX_FILES) { // 최대 파일 개수 확인
                strcpy(current->FileName[current->file_index], file_name); // FileName에 파일 이름 저장
                current->file_index++; // 인덱스를 하나 증가
            } else {
                printf("Maximum file limit reached for user %s.\n", username);
            }
            break;
        }
        current = current->next; // 다음 노드로 이동
    }
    saveUserFileData(current); // 유저의 파일 이름을 저장함

    fclose(file); // 파일 닫기 
    printf("%s User File Upload SUCCESS\n", username); // 해당 유저의 파일 업로드 성공 출력
    sprintf(result, "File Upload SUCCESS\n"); // result에 성공 메시지 대입
    send(cliSock, result, strlen(result), 0); // client 에 전달

    return 1;
}
