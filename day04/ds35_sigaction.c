#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	alarm(2);
}


int main(int argc, char *argv[])
{
	int i;
	
	struct sigaction art; // 구조체변수 sigaction(=art) 초기화
	art.sa_handler=timeout; // 선언된 구조체 변수 art의 멤버 sa_handler에 함수 포인터 저장
	sigemptyset(&art.sa_mask); // act의 멤버 sa_mask 초기화를 위해 호출되는 함수 sigemptyset();
	art.sa_flags=0;

	sigaction(SIGALRM, &art, 0); // 시그널 핸들러 지정: 이벤트 => SIGALRM, 함수 => 구조체 act에 저장되어있는 sa_handler가 가리키는 함수(=timeout)

	alarm(2); // alarm 이벤트 2초 뒤 실행, => timeout 함수 호출 =>  timeout 함수 호출 내 alarm 시그널 2초 뒤 실행 => 반복

	for(i=0; i<3; i++)
	{
		puts("wait...");
		sleep(100);
	}
	
	return 0;
}
