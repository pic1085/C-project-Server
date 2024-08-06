#include "download.h"

int Download(SOCKET cliSock) {
    char file_name[BUF_SIZE] = ""; // 파일 이름을 저장할 버퍼 초기화
    char recvmsg[BUF_SIZE] = ""; // 수신 메시지를 저장할 버퍼 초기화
    char buffer[BUF_SIZE] = ""; // 파일 내용을 저장할 버퍼 초기화 
    char userId[BUF_SIZE] = ""; // 사용자 ID를 저장할 버퍼 초기화
    FILE* file; // 파일 포인터 선언

    int bytes_received = recv(cliSock, recvmsg, BUF_SIZE, 0); // 클라이언트로부터 파일명 요청 수신
    recvmsg[bytes_received] = '\0'; // 수신된 데이터를 문자열로 만듦 
    sscanf(recvmsg, "%s %s", file_name, userId); // 받은 값을 file_name과 userId로 각각 나누어 받음

    char file_path[BUF_SIZE]; // 파일 경로를 저장할 버퍼 선언
    snprintf(file_path, BUF_SIZE, "C:\\server\\server\\%s\\%s", userId, file_name); // 파일 경로를 설정

    sprintf(buffer, "Check The Path"); // buffer에 값을 저장 
    send(cliSock, buffer, strlen(buffer), 0); // 클라이언트에 buffer 전달

    send(cliSock, file_name, strlen(file_name), 0); // 클라이언트에 파일 이름 전달

    file = fopen(file_path, "rb"); // 파일을 바이너리 읽기 모드로 열기
    if (file == NULL) {  // 파일이 없을 경우 에러 처리
        perror("File open error...");
        sprintf(buffer, "NO DATA");
        send(cliSock, buffer, strlen(buffer), 0);
        return 0;
    }

    while (!feof(file)) {    // 파일의 끝까지 반복하여 내용을 읽고 클라이언트로 전송
        int bytes_read = fread(buffer, 1, BUF_SIZE, file); // 파일에서 BUF_SIZE만큼 데이터를 읽어 buffer에 저장
        if (bytes_read <= 0) { // 데이터가 없을 경우 종료
            break;
        }
        int bytes_sent = send(cliSock, buffer, bytes_read, 0); // 읽은 데이터를 클라이언트로 전송
        if (bytes_sent != bytes_read) { // 전송된 바이트 수가 다를 경우 에러 처리
            printf("File Send Error\n"); // 에러 메시지 출력
            fclose(file); // 파일 닫기
            return 0;
        }
    }

    fclose(file); // 파일 닫기 
    printf("File Download SUCCESS\n"); // 성공 메시지 출력
    sprintf(buffer, "File Download SUCCESS\n"); // buffer에 성공 메시지 대입
    send(cliSock, buffer, strlen(buffer), 0); // client 에 전달
    return 1;
}
