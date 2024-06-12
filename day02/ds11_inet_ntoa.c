#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in addr1, addr2;
	char *str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr=htonl(0x1020304); 
	// 구조체 addr1의 속성인 구조체 sin_addr의 속성 s_addr
	// 호스트 바이트를 네트워크 바이트 순서로 변환
	addr2.sin_addr.s_addr=htonl(0x1010101);

	str_ptr=inet_ntoa(addr1.sin_addr);
	// 네트워크 바이트 주소를  string(char *) 문자열로 변환
	strcpy(str_arr, str_ptr);
	printf("Dotted-Decimal notation1: %s \n", str_ptr);

	inet_ntoa(addr2.sin_addr); // inet_ntoa 재호출 시, 이전 저장된 정보 소실될 수 있음
	printf("Dotted-Decimal notation2: %s \n", str_ptr); // 1.1.1.1 출력
	printf("Dotted-Decimal notation3: %s \n", str_arr);
	return 0;
}
