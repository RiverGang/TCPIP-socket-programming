#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
void* thread_main(void *arg);

int main(int argc, char *argv[])
{
	pthread_t t_id;
	int thread_param=5;


	// 쓰레드 생성요청(pthread_create)
	// thread_main 함수 호출을 위한 인자 (void*)&thread_param
	if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0)
	{
		puts("pthread_create() error");
		return -1;
	}

	sleep(10); puts("end of main"); // 프로세스 종료 시기 늦추기
	// 프로세스(=main 함수)가 종료되면 쓰레드도 종료되기 때문에 쓰레드의 실행을 보장하기 위한 문장
	// 충분한 시간이 없으면, 쓰레드가 제대로 작동하지 않고(= running thread 문장이 5번 출력되지 않음) 종료되어버림
	return 0;
}

void* thread_main(void *arg)
{
	int i;
	int cnt=*((int*)arg);
	for(i=0; i<cnt; i++)
	{
		sleep(1); puts("running thread");
	}
	return NULL;
}
