#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid=fork();

	if(pid==0){ return 3;} // pid == 0은 자식
	else // 부모 프로세스
	{
		printf("Child PID: %d \n", pid);
		pid=fork();
		if(pid==0) // 15행에서 생성된 두번째 자식프로세스는 별다른 것 없이 바로 종료
		{
			exit(7);
		}
		else
		{
			printf("Child PID: %d \n", pid);
			wait(&stauts);
			if(WIFEXITED(stauts)) // WIFEXITED -> 자식 프로세스가 정상 종료되었을 경우 참(True) 반환
			// 자식 프로세스(stauts) 정상 종료 시, printf() 출력
				printf("Child send one: %d \n", WEXITSTATUS(status)); 
				// 정상 종료시, 부모프로세스에게 값 반환(return, exit한 값) -> WEXITSTATUS


			wait(&status);
			if(WIFEXITED(stauts))
				printf("Child send two: %d \n", WEXITSTATUS(status));
			sleep(30); // Sleep 30 sec;
		}
	}

	return 0;
}

