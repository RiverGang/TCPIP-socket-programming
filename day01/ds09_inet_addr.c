#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	char *addr1="1.2.3.4";
	char *addr2="1.2.3.256"; // 1바이트당 표현할 수 있는 최대 크기의 정수 255 => 따라서, addr2는 잘못된 주소 (= 오류발생)

	unsigned long conv_addr=inet_addr(addr1);
	if(conv_addr==INADDR_NONE)
		printf("Error occured! \n");
	else
		printf("Network ordered integer addr: %#lx \n", conv_addr);

	conv_addr=inet_addr(addr2);
  if(conv_addr==INADDR_NONE)
     printf("Error occured! \n"); // addr2의 주소가 잘못되었으므로 오류문장 출력
  else
     printf("Network ordered integer addr: %#lx \n\n", conv_addr);

	return 0;
}
