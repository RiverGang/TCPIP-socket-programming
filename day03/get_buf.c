#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	int snd_buf, rcv_buf, state;
	socklen_t len;

	sock=socket(PF_INET, SOCK_STREAM, 0);
	len=sizeof(snd_buf);
	state=getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len); // 출력버퍼 크기
	if(state)
		error_handling("getsockopt() error");

	len=sizeof(rcv_buf);
	state=getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len); // 입력버퍼 크기
	if(state)
		error_handling("getsockopt() error");

	printf("Input buffer size: %d \n", rcv_buf); // 입력버퍼
	printf("Output buffer size: %d \n", snd_buf); // 출력버퍼
	return 0;

}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
