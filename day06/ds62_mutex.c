#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

void * thread_inc(void * arg);
void * thread_des(void * arg);

long long num = 0; // long long 형: 64비트 정수 자료형
pthread_mutex_t mutex; // 뮤텍스 변수

int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	pthread_mutex_init(&mutex, NULL); // 10행에서 선언한 뮤텍스변수 함수를 이용해 초기화
	
	for(i=0; i<NUM_THREAD; i++)
	{
		if(i%2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for(i=0; i<NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %lld \n", num);
	pthread_mutex_destroy(&mutex); // 뮤텍스 소멸
	return 0;
}

void * thread_inc(void * arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0; i<50000000; i++)
		num+=1; // 임계영역
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void * thread_des(void * arg)
{
	int i;
	for(i=0; i<50000000; i++)
	{
		pthread_mutex_lock(&mutex); // 임계영역 시작 -> lock
		num-=1; // 임계영역
		pthread_mutex_unlock(&mutex); // 임계영역 종료 -> unlock
	}
	return NULL;
}
