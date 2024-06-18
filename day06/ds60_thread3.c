#include <stdio.h>
#include <pthread.h>
void * thread_summation(void * arg);
int sum = 0; // 메인함수, 쓰레드함수 모두 접근 가능한 전역변수

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;
	int range1[]={1, 5};
	int range2[]={6, 10};

	// 두개의 쓰레드 (id_t1, id_t2)가 하나의 전역변수(sum)에 접근
	pthread_create(&id_t1, NULL, thread_summation, (void *)range1); 
	pthread_create(&id_t2, NULL, thread_summation, (void *)range2);
	// pthread_create의 두번째 인자 -> 쓰레드에 부여할 특성 정보, NULL 입력 시 기본특성으로 쓰레드 생성

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);
	printf("result: %d \n", sum);
	return 0;
}

void * thread_summation(void * arg)
{
	int start=((int*)arg)[0];
	int end=((int*)arg)[1];

	while(start<=end)
	{
		sum+=start; 
		start++;
	}
	return NULL;
}
