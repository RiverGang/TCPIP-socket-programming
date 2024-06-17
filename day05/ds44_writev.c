#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
	struct iovec vec[2];
	char buf1[]="ABCDEFG";
	char buf2[]="1234567";
	int str_len;

	vec[0].iov_base=buf1;
	vec[0].iov_len=3; // ABC
	vec[1].iov_base=buf2;
	vec[1].iov_len=4; // 1234

	str_len=writev(1, vec, 2); // 성공 시, 전송된 바이트수를 반환하므로 int type의 변수에 정의
	//ABC1234 출력
	puts("");
	printf("Write bytes: %d \n", str_len); // 7 Byte
	return 0;
}
