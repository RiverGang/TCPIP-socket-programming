#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

void * thread_inc(void * arg);
void * thread_des(void * arg);
long long num = 0; // long long 형: 64비트 정수 자료형

int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));
	for(i=0; i<NUM_THREAD; i++)
	{
		if(i%2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for(i=0; i<NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %lld \n", num); // 하지만 0이 아닌 수 출력됨
	return 0;
}

void * thread_inc(void * arg)
{
	int i;
	for(i=0; i<50000000; i++) // 하나의 전역 변수 (num), 같은 크기로 증감이 되므로 num에는 0이 저장되어야함
		num+=i;
	return NULL;
}

void * thread_des(void * arg)
{
	int i;
	for(i=0; i<50000000; i++)
		num-=1;
	return NULL;
}
