#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void error_handling(char *message);
void send_data(FILE *fp, char *ct, char *file_name);
char* content_type(char *file);
void send_error(FILE *fp);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE];
    char method[10];
    char ct[15];
    char file_name[30];
    FILE *client_read, *client_write;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (1) {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1)
            error_handling("accept() error");

        client_read = fdopen(clnt_sock, "r");
        client_write = fdopen(dup(clnt_sock), "w");
        fgets(buf, BUF_SIZE, client_read);
        if (strstr(buf, "HTTP/") == NULL) {
            send_error(client_write);
            fclose(client_read);
            fclose(client_write);
            continue;
        }

        strcpy(method, strtok(buf, " /"));
        strcpy(file_name, strtok(NULL, " /"));
        strcpy(ct, content_type(file_name));
        if (strcmp(method, "GET") != 0) {
            send_error(client_write);
            fclose(client_read);
            fclose(client_write);
            continue;
        }

        fclose(client_read);
        send_data(client_write, ct, file_name);
    }

    close(serv_sock);
    return 0;
}

void send_data(FILE *fp, char *ct, char *file_name) {
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char serv_name[] = "Server: Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");
    if (send_file == NULL) {
        send_error(fp);
        return;
    }

    /* 전송 헤더 데이터 전송 */
    fputs(protocol, fp);
    fputs(serv_name, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    /* 요청 데이터 전송 */
    while (fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

char* content_type(char *file) {
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/plain";
}

void send_error(FILE *fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char serv_name[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>오류 발생! 요청 파일명 및 요청 방식 확인!"
                     "</font></body></html>";

    fputs(protocol, fp);
    fputs(serv_name, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
    fclose(fp);
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
