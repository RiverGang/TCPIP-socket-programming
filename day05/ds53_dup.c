#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int cfd1, cfd2;
	char str1[]="Hi~\n";
	char str2[]="It's nice day~\n";

	cfd1=dup(1);
	cfd2=dup2(cfd1, 7);

	printf("fd1=%d, fd2=%d \n", cfd1, cfd2);
	write(cfd1, str1, sizeof(str1)); // 출력
	write(cfd2, str2, sizeof(str2)); // 출력

	close(cfd1);
	close(cfd2);
	write(1, str1, sizeof(str1)); // 복사본의 원본 파일디스크립터(1)
	close(1); //  닫기
	write(1, str2, sizeof(str2)); // 앞문장에서 닫혔기에 출력 x
	return 0;
}
