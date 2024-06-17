#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int sock;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_addr;

	FILE * readfp;
	FILE * writefp;

	sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	readfp=fdopen(sock, "r"); // server 소켓으로 부터 오는 정보를 읽어내는 파일포인터
	writefp=fdopen(sock, "w");

	while(1)
	{
		if(fgets(buf, sizeof(buf), readfp)==NULL) // readfp(서버로 부터 받아 온 파일포인터)의 정보를 buf에 저장한다
			break; // fgets == NULL -> EOF 전달됨, EOF 전달되면 반복문(while)을 빠져나간다
		fputs(buf, stdout);
		fflush(stdout);
	}

	fputs("FROM CLIENT: Thank You! \n", writefp); // 서버로 부터 EOF 메세지를 받고난 (수신하고) 후, 서버에게 마지막 문자열을 전송
	fflush(writefp);
	fclose(writefp); fclose(readfp);
	return 0;
}
