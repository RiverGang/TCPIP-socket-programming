#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30
void error_handling(char *message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char *argv[])
{
	struct sockaddr_in recv_adr, serv_adr;
	int str_len, state;
	socklen_t serv_adr_sz;
	struct sigaction act; // sigaction 구조체 변수 -> 운영체제의 시그널 이벤트 처리를 위한 구조체변수
	char buf[BUF_SIZE];
	if(argc!=2){
		printf("Usage: %s <prot>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=urg_handler; // 시그널 발생 시, 작동할 함수 정의
	sigemptyset(&act.sa_mask); // 구조체 변수 act의 멤버 sa_mask 초기화
	act.sa_flags=0; // 멤버 sa_flags 초기화

	acpt_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family=AF_INET;
	recv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	recv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr))==-1)
		error_handling("bind() error");
	listen(acpt_sock, 5);

	serv_adr_sz=sizeof(serv_adr);
	recv_sock=accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

	fcntl(recv_sock, F_SETOWN, getpid()); 
	// fcntl 함수 -> 파일디스크립터 recv_sock이 가리키는 소켓에 의해 발생하는 SIGURG 시그널을 처리하는 프로세스를, 
	// getpi 함수가 반환하는 ID의 프로세스로 변경

	// 왜 fcntl함수로 처리해줘야 하는가?
	// ex) fork 함수 호출을 통해 자식 프로세스 생성 시, 생성과 동시에 파일디스크립터까지 복사 -> 이러한 상황에서 SIGURG 시그널 발생 시 어느 프로세스(부모 or 자식)의 핸들러 함수를 호출해야하는가?
	// 따라서, SIGURG 시그널을 핸들링 할 때에는 반드시 시그널 프로세스를 지정해줘야함
	state=sigaction(SIGURG, &act, 0);

	while((str_len=recv(recv_sock, buf, sizeof(buf), 0))!=0)
	{
		if(str_len==-1)
			continue;
		buf[str_len]=0;
		puts(buf);
	}
	close(recv_sock);
	close(acpt_sock);
	return 0;
}

void urg_handler(int signo)
{
	int str_len;
	char buf[BUF_SIZE];
	str_len=recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
	buf[str_len]=0;
	printf("Urgent message: %s \n", buf);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
