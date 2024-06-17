#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	FILE *fp;
	int fd=open("data.dat", O_WRONLY|O_CREAT|O_TRUNC); // 파일디스크립터 생성
	if(fd==-1)
	{
		fputs("file open error", stdout);
		return -1;
	}

	printf("First file descriptor: %d \n", fd); // 파일디스크립터 fd 출력
	fp=fdopen(fd, "w"); // 파일디스크립터 fd를 쓰기모드로 파일포인터 fp 변환
	fputs("TCP/IP SOCKET PROGRAMMING \n", fp); // 파일포인터 fp가 가리키는 data.dat에 문자열 집어 넣기
	printf("Second file descriptor: %d \n", fileno(fp)); // 파일포인터 fp를 파일디스크립터(=정수)로 다시 변환
	fclose(fp); // 파일포인터 닫기
	return 0;
}
