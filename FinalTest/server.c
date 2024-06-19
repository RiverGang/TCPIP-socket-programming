#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define TRUE 1
#define BUF_SIZE 1024

 char webpage[] = "HTTP/1.1 200 OK\r\n"
                   "Server:Linux Web Server\r\n"
                  "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                  "<!DOCTYPE html>\r\n"
                  "<html><head><title> My Web Page </title>\r\n"
                   "<link rel=\"icon\" href=\"data:,\">\r\n"
                  "<style>body {background-color: #FFFAB5 }</style></head>\r\n"
                  "<body><center><h1>Hello! I'm Choonsik!</h1><br>\r\n"
                   "<img src=\"choonsik01.png\"></center></body></html>\r\n";

 int main(int argc, char *argv[])
 {
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sin_len = sizeof(clnt_addr);
    int serv_sock, clnt_sock;
    char buf[2048];
    int fdimg, img_size;
    int option = TRUE;
    char img_buf[500000];

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) perror("bind() error!!\n");
    if(listen(serv_sock, 5) == -1) perror("listen() error!!\n");

    while(1)
    {
       clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &sin_len); // 클라이언트 소켓
       puts("connection.....");
       read(clnt_sock, buf, BUF_SIZE); // 클라이언트에서 수신된 정보
       printf("%s\n", buf); // 클라이언트에 대한 정보 출력

       if(strstr(buf, "choonsik01.png") != NULL) // 클라이언트에서 수신된 정보에 사진이 포함되어 있으면
       {
       		fdimg = open("choonsik01.png", O_RDONLY);  // 서버에 있는 그림 파일 열기
          if((img_size = read(fdimg, img_buf, sizeof(img_buf))) == -1) puts("file read error!!");
          // img_buf에 해당 이미지 정보 담기
          close(fdimg); // 파일디스크립터 닫기

          sprintf(buf, "HTTP/1.1 200 OK\r\n"
           "Server: Linux Web Server\r\n"
           "Content-Type: image/jpeg\r\n"
           "Content-Length: %ld\r\n\r\n", img_size); // 헤더 정보 buf에 담기

       		if(write(clnt_sock, buf, strlen(buf)) < 0) puts("write error"); // 헤더 송신

          if(write(clnt_sock, img_buf, img_size) < 0) puts("write error"); // 이미지 정보 전송/

          close(clnt_sock);
       }

       else // 클라이언트에서 수신된 정보가 따로 없음 ==> 최초 접속
          if(write(clnt_sock, webpage, sizeof(webpage)) == -1) puts("write error!!");
       puts("closing");
       close(clnt_sock);
   }
       close(serv_sock);
       return 0;
}
