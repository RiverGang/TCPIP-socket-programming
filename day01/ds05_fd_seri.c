#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int fd1, fd2, fd3;
	fd1=socket(PF_INET, SOCK_STREAM, 0);
	fd2=open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
	fd3=socket(PF_INET, SOCK_DGRAM, 0);

 // 운영체제가 표준으로 파일디스크립터 번호 0,1,2를 사용하기에 3,4,5 순서로 출력됨
	printf("file descriptor 1: %d\n", fd1);
	printf("file descriptor 2: %d\n", fd2);
 	printf("file descriptor 3: %d\n", fd3);
 	close(fd1); close(fd2); close(fd3);
 	return 0;
}

