#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0); // 초기값 0
	sem_init(&sem_two, 0, 1); // 초기값 1 => 키 생성 => sem_two를 통해 시작 가능 =

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void * read(void * arg)
{
	int i;
	for(i=0; i<5; i++)
	{
		fputs("Inputs num: ", stdout);

		sem_wait(&sem_two); // 15행에서 키를 받았기에 wait 호출 가능 -> wait 함수는 키 값 0으로 변경
		scanf("%d", &num);
		sem_post(&sem_one); // sem_post 함수를 통해 sem_one의 키 값을 1증가 시킴 -> 키 반환 ->  accu 함수 쓰레드 실행
	}
	return NULL;
}

void * accu(void * arg)
{
	int sum=0, i;
	for(i=0; i<5; i++)
	{
		sem_wait(&sem_one); // sem_one의 키 값 감소 (=반납)
		sum+=num; // sum에 값을 누적시키고, 다시 입력을 받아야하니
		sem_post(&sem_two); // sem_two의 키 값 증가
	}
	printf("Result: %d \n", sum);
	return NULL;
}
