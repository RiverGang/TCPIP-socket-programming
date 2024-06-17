#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	FILE * readfp;
	FILE * writefp;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	char buf[BUF_SIZE]={0,};

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sock, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);
	clnt_sock= accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);

	readfp=fdopen(clnt_sock, "r");
	writefp=fdopen(clnt_sock, "w");

	fputs("FROM SERVER: Hi client?\n", writefp);
	fputs("I love all of the world \n", writefp);
	fputs("You are awesome! \n", writefp);
	fflush(writefp);

	fclose(writefp); // 소켓 종료, 31행에서 생성한 읽기모드 파일포인터(readfp)는 종료하지 않았으나, 이미 같은 소켓이므로 읽기도 불가능한 완전종료가 됨
	fgets(buf, sizeof(buf), readfp); // ds55_sep_clnt가 보낸 마지막 메세지 "FROM CLIENT: Thank you" 출력되지 않음
	fputs(buf, stdout); // 따라서, Half-close 구현되지 않았음을 알 수 있음
	fclose(readfp);
	return 0;
}
