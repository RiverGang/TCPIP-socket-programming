#include <stdio.h>
#define BUF_SIZE 3

int main(int argc, char *argv[])
{
	FILE *fp1; // fp1에 저장되는 것은 FILE 구조체의 포인터
	FILE *fp2;

	char buf[BUF_SIZE];

	fp1=fopen("news.txt", "r"); // news.txt 파일 읽기 모드
	fp2=fopen("cpy.txt", "w"); // cpy.txt 파일 쓰기 모드

// fputs & fgets를 이용해(= 표준 입출력 함수) 파일 복사 -> 버퍼링 기반의 복사
	while(fgets(buf, BUF_SIZE, fp1)!=NULL)
		fputs(buf, fp2);

	fclose(fp1);
	fclose(fp2);
	return 0;
}
