#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	FILE *fp;
	int fd=open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
	if(fd==-1)
	{
		fputs("file open error", stdout);
		return -1;
	}

	fp=fdopen(fd, "w"); // 파일디스크립터 fd를 쓰기 모드 파일포인터로 변환
	fputs("Network C programming \n", fp);
	fclose(fp);
	return 0;
}
