# TCPIP-socket-programming

- 라즈베리파이 주기적 업데이트/업그레이드
    - sudo apt update
    - sudo apt upgrade -y

- 다중접속 서버 구현 방법
    1. 멀티프로세스
        - fork()
    2. IO 멀티플렉싱
        - select() -> 기능은 떨어지지만(동시접속자 수 100 이하), 윈도우/리눅스 모두 가능(이식성이 좋음)
        - epoll -> 리눅스 운영체제
    3. 멀티쓰레드

## 1일차 (20240611)
- IP
    - 외부에서 해당컴퓨터를 찾아올 때 필요한 주소 (컴퓨터를 구분)
    - IPv4: 4Byte -> ex) 192.168.5.2
    - IPv6: IPv4 주소 모두 할당 시, 이후 사용

- 데이터 전송방식
    - TCP: 정확한 데이터 전송
    - UDP: 속도에 초점, 데이터 유실 가능성 존재
        - ex) 압축 파일, 미디어 전송(영화, 영상)

- 네트워크 프로세스
    - 소 : 소켓(socket) → 휴대폰
    - 말 : 바인더(bind) → 전화번호 할당
    - 리 : 리슨(listen) → 개통
    - 아 : 엑셉트(accept) → 통화기능

    ```c
        #include <sys/socket.h>

        int socket(int domain, int type, int protocol); // 성공 시 파일디스크립터, 실패시 -1 반환
        int bind(int sockfd, struct sockaddr *myaddr, socklent_t addrlen); // 성공-> 0, 실패-> -1
        int listen(int sockfd, int backlog); // 성공-> 0, 실패-> -1
        /*
            sockfd: 연결요청 대기상태에 두는 소켓의 파일디스크립터 전달, 여기서 인가된 디스크립터의 소켓이 서버 소켓
            backlog: 연결 요청 대기 큐(Queue)의 크기 정보
            ex) 5 전달 -> 클라이언트의 연결 요청 5개까지 대기 가능
        */
        int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); // 성공-> 파일디스크립터, 실패-> -1 
    ```

 # 2일차
- TCP 서버의 함수호출 순서
    - 소켓생성 -> 소켓 주소할당 -> 연결요청 대기상태 -> 연결허용 -> 데이터 송수신 -> 연결종료
    - socket() -> bind() -> listen() -> accept() -> read()/writh() -> close()

    - 

# 3일차
- UDP 기반의 데이터 입출력 함수
        ```c
        #include <sys/socket.h>
        //데이터를 전송할 때 호출하는 UDP 함수
        ssize_t sendto(int sock, void *buff, size_t nbyte, int flags,
                                struct sockaddr *to, socklen_t addrlen);

        //데이터를 수신할 때 호출하는 UDP 함수
        ssize
        ```

        - 전통적인 UDP 방식(unconnected)은 소켓에 데이터를 전송할 목적지의 IP와 PORT번호를 등록하지 않음 -> sendto/recvfrom 함수호출 마다 IP와 PORT 번호 등록 필요 
        - connected UDP 소켓 생성 시, 송수신 대상이 정해지기 때문에 write,read 함수의 호출로도 데이터 송수신 가능

- 소켓의 종료
    - 일반적인 종료의 문제점: 한쪽에서 일방적으로 종료 시, 전송해야 하는 데이터가 남아있을 경우 수신 불가 (소켓 자체가 닫히기때문)
    - **Half-close**: 양방향 통신을 위해서는 양방향 스트림이 필요하고, 이 중 하나의 스트림만 끊는 것
        
        ```c
        #include <sys/socket.h>

        int shutdow(int sock, int howto)
        // sock -> 종료할 소켓의 파일 디스크립터
        /* howto -> 종료 방법
             SHUT_RD -> 입력 스트림 종료  
             SHUT_WR -> 출력 스트림 종료 : 일반적으로 종료할 때 출력스트림 종료, 내가 가진 데이터 전송은 중단하고 상대방이 나에게 전송하고 있는 데이터가 있을 수 있으니 입력 스트림은 열어놓기
             SHUT_RDWR -> 입출력 스트림 종료
        */
        ```
    - 서버는 출력스트림 종료 시, 클라이언트에게 파일의 전송이 끝났음을 알리는 EOF 전송. 

- 도메인
    - IP주소와 포트번호를 기억하기 쉽게 부여한 문자열 주소
    
    - DNS(Domain Name Server): 도메인 문자열 주소를 IP주소로 변환해주는 서버

- 소켓의 옵션과 입출력 버퍼의 크기 p201
    - getsockopt
        - SO_TYPE: 소켓의 타입정보 (TCP or UDP)
        - SO_SNDBUF/SO_RCVBUF: 입/출력 버퍼 크기확인
     
    - setsockopt
        - SO_SNDBUF/SO_RCVBUF: 입/출력 버퍼 크기변경

- Time-wait 상태 p209

- 멀티프로세스기반의 서버구현 p221
    - 프로세스(Process)의 이해
    - fork 함수
        ```c
        #inlcude<unistd.h>

        pid_t fork(void);
        // 성공 -> 프로세스 ID, 실패 -> -1 반환
        ```

## 4일차
- wait() ds32
- waitpid()
- 시그널핸들링
    - 자식 프로세스에서 특정 이벤트 발생 시, 특정 함수의 호출을 운영체제에게 요구
    - 함수포인터 형태 p235, ds35
    ```c
        #include<signal.h>
        void(*signal(int signo, void (*func)(int)))(int);
        // 함수 이름 signal
        // 매개변수 선언 int signo, void(*func)(int)
        // 반환형 매개변수형이 int이고 반환형이 void인 함수 포인터

        /* signal 함수는 운영체제 별 동작에 차이가 있을 수 있어서 거의  sigaction 함수로 작성되고 있음*/

        int sigaction(int signo, const truct sigaction * act, struct sigaction * oldact);
        // 성공 -> 0, 실패 -> -1 
        // * singaction 구조체 변수 = 시그널 발생시 호출될 함수 정보 전달

        struct sigaction 
        {
            void (*sa_handler)(int); // 함수 포인터 => 함수 이름
            sigset_t sa_mask; // 0으로 초기화
            int sa_flags; // 0으로 초기화
        }
    ```

- 멀티태스킹 기반의 다중접속 서버
    - 클라이언트의 서비스 요청(연결요청)이 있을 때 마다 자식 프로세스 생성하여 서비스 제공
        1. 에코 서버(부모 프로세스)는 accept 함수호출을 통해 연결 요청 수락
        2. 이때 얻게 되는 소켓의 파일 디스크립터를 자식 프로세스를 생성해서 넘김
        3. 자식 프로세스는 전달받은 파일 디스크립터를 바탕으로 서비스 제공
            - 자식 프로세스는 부모 프로세스가 소유하고 있는 것을 모두 복사하기에 파일 디스크립터를 넘기는 과정은 실질적으로 없음

- TCP의 입출력 루틴(Routine) 분할

- 멀티플렉싱 기반의 서버구현 258p ~ 284p
    - select 함수
    - 프로세스 하나 (메인 프로세스 = select)
    - 자원의 사용이 적고, 구현이 쉬움


## 5일차
- 리눅스에서의 send & recv 입출력 함수
    - 윈도우의 send 함수와 선언된 자료형 이름에서 차이가 조금 나지만, 매개변수 순서와 사용법까지 완전히 동일
    - 옵션정보 -> 비트 OR 연산자(|)를 이용해 둘 이상 함께 전달 가능 (Notion 참고)
    
    - TCP에서는 긴급메세지라고 전송속도/경로가 달라지진 않음 -> 단지 긴급메세지라는 표기가 추가되는 것

    - readv & writev 입출력 함수 (ds44)
        - 여러 버퍼에 나눠 저장되어 있는 데이터를 한번에 전송/수신

        ```c
            #include <sys/uio.h>

            ssize_t write(int filedes, const struct iovec * iov, int iovcnt);
            // 성공 -> 전송된 바이트수, 실패 -> -1 반환

            // filedes: 데이터 전송의 목적지를 나타내는 소켓의 파일디스크립터
            // iov: 구조체 iovec "배열"의 주소 값
            // iovcnt: 두번째 인자로 전달된 주소 값이 가리키는 배열의 길이

            struct iovec
            {
                void * iov_base; // 버퍼의 주소 정
                size_t iov_len; // 버퍼의 크기 정보
            }

            writev(1, ptr, 2);
            /*
                ptr[0] -> iov_base => | A | B | C | . | . |
                          iov_len=3
                ptr[1] -> iov_base => | 1 | 2 | 3 | 4 | . |
                          iov_len=4
            */
        ```

- C언어 표준 입출력 함수 (p338, ds46)
    - 장점
        - 이식성(Portability) -> ANSI C에서 표준으로 정의했기에 모든 운영체제가 지원
        - 버퍼링을 통한 성능 향상
            - ex) 1 Byte 데이터를 열 번에 걸쳐 (= 열 개의 패킷) 전송 or 10 Byte로 묵어 한번에 전송
                  패킷 당 40바이트만 잡아도 (실제로 패킷 크기는 이보다 더 큰경우가 많음) 전송하는 데이터가 유의미하게 차이남
                  40(패킷) x 10(회) = 400 byte
                  40(패킷) x 1(회) = 40 byte
        
        - fgets()/fputs()
            ```c
            char *fgets(char *str, int numChars, FILE *stream);
            // str: 파일에서 읽어 온 문자열 데이터를 저장할 문자열 변수의 주소
            // numChars: 저장할 문자의 최대 크기 -> 일반적으로 첫번째 매개변수 str의 크기
            // stream: 해당 파일의 파일포인터
            // 성공 -> 첫번째 매개 변수의 주소(str), 실패 -> NULL

            int fputs(const char*str, FILE *stream)
            // file+put+string -> 파일에 문자열 데이터를 집어 넣는 함수

            // str: 파일에 집어 넣을 문자열
            // stream: 파일의 파일포인터
            // 성공 -> 0 이상의 수 (양수 or 0), 실패 -> EOF
            ```

        - fdopen()/fileno()
            ```c
            #include <stdio.h>
            
            FILE * fdopen(int fildes, const char * mode);
            // 파일 디스크립터를 파일포인터로 변환
            // files: 변환할 파일 디스크립터 인자
            // mode: 생성할 FILE구조체 포인터의 모드(mode) 정보 -> "r", "w" 등
            // 성공 -> 반환된 파일포인터, 실패 -> NULL

            int fileno(FILE * stream);
            // 파일포인터를 파일디스크립터로 변환
            // 성공 -> 파일디스크립터 인자, 실패 -> -1
            ```

        - fflush()
            ```c
            #include <stdio.h>
            int fflush(FILE* file);
            // 출력 stream 버퍼를 비워준다 (입력 버퍼는 X)
            // 성공 -> 0, 실패 -> EOF(-1)
            ```

        - 입력 스트림과 출력 스트림의 분리 (p353)
            - 파일포인터 사용 시, Half-close 이용에 차질이 생김

            - 파일디스크립터의 복사와 Half-close
                - 모든 파일 디스크립터가 소멸되어야 소켓도 소멸
                
## 6일차
- select 보다 나은 epoll
    - epoll_create: epoll 파일 디스크립터 저장소(= epoll 인스턴스) 생성을 운영체제에게 요청
    - epoll_ctl: 저장소에 파일 디스크립터 등록 및 삭제
    - epoll_wait: select 함수와 마찬가지로 파일 디스크립터의 상태 변화를 대기/확인

    - p368~p371
    ```c
    #include <sys/epoll.h>

    int epoll_create(int size);// close 종료 필요
    // 성공 -> epoll 파일 디스크립터, 실패 -> -1
    // size: 인스턴스 크기를 결정하는 인자, 참고로만 사용 (직접적인 결정x)

    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    // 성공 -> 0, 실패 -> -1
    // epfd: 관찰대상을 등록할 저장소 = epoll 인스턴스 파일 디스크립터
    // op: 관찰대상의 추가/삭제/변경여부 옵션
    // fd: 관찰대상의 파일 디스크립터
    // event: 관찰대상의 관찰 이벤트 유형

    struct epoll_event // 구조체 배열
    {
        __unit32_t events;
        epoll_data_t data;
        ...
    }

    int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
    // events: 이벤트가 발생한 파일 디스크립터가 채워질 버퍼의 주소 값 
    // ** 주의 ** epoll_event 구조체 동적할당 필수
    // maxevents: 두번째 인자로 전달된 주소 값의 버퍼에 등록 가능한 최대 이벤트 수
    // timeout: 1/1000초 단위 대기시간, -1 전달 => 이벤트 발생까지 무한 대기
    // 성공-> 이벤트가 발생한 파일 디스크립터의 수, 실패 -> -1
    ```
        - ex) epoll_ctl(A, EPOLL_CTL_ADD, B, C)
        - epoll 인스턴스 A에 파일 디스크립터 B를 등록(ADD)하되 C를 통해 전달된 이벤트의 관찰이 목적

- 레벨트리거(Level Trigger)
    - 입력 버퍼에 데이트가 남아있는 동안 계속해서 이벤트 등록
- 엣지트리거(Edge Trigger) 
    - 데이터가 수신된 상황에서 한번만 이벤트 등록

- 멀티쓰레드 기반의 서버구현
    - 멀티프로세스의 단점: 컨텍스트 스위칭에 따른 부담
    - 컨텍스트 스위칭(Context Switching): 프로세스가 RAM에 올라오고 ROM에 다시 내려가는 일련의 동작
        - 프로세스 실행의 의미 => RAM에 올라와서 실행
        - 실행이 되지 않는 나머지 프로세스 => 하드디스크 ROM에 저장

    - 쓰레드(Thread)
        - 장점: 프로세스에 비해 생성 및 컨텍스트 스위칭이 빠르다
        - 하나의 프로세스 내에 여러개의 쓰레드
        - 쓰레드는 스택 영역만 각자 독립적으로 유지
        - 데이터&힙 영역 공유 => 컨텍스트 스위칭 시, 데이터 영역과 힙은 올리고 내릴 필요 X => 이를 이용해 데이터 교환 가능
        - 프로세스 관점에서 별도의 실행흐름을 구성하는 단위

        ```c
        int pthread_create (pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void*), void *restrict arg)
        ```

        - 쓰레드가 정상적으로 종료될 때까지 실행흐름 조절

        ```c
        pthread_join()
        ```

        - ds61, p410
        - 임계영역: 함수 내에 둘 이상의 쓰레드가 동시에 실행하면 문제를 일으키는 하나 이상의 문장으로 묶여있는 코드
            - 두 쓰레드가 동시에 동일한 메모리 공간에 접근을 하는 경우

        - 쓰레드 동기화
            - 뮤텍스(Mutex, Mutual Exclusion): 쓰레드의 동시접근을 허용하지 않는다
            
            ```c
            pthread_mutex_t mutex; // 변수

            int pthread_mutex_init() // 초기화
            int pthread_mutex_destroy() // 소멸

            // 아래와 같이 하나의 세트처럼 묶어서 사용 (시작과 끝을 감싸기)
            int pthread_mutex_lock() // 임계영역 잠그기
            // 임계영역 시작
            //.................
            // 임계영역 끝
            int pthread_mutex_unlock() // 임계영역 풀기
           ```

           - 세마포어(Semaphore): 세마포어 값(Semaphore Value) 정수로 기록되는 키 값을 통해 '실행순서 컨트롤' 중심의 동기화

           ```c
            #include <semaphore.h>
            
            int sem_init(sem_t *sem, int pshared, unsigned int value);
            int sem_destroy(sem_t *sem);
            // 성공 -> 0, 실패 -> 0이 아닌 값
            // sem: 세마포어 생성/소멸 하고자 하는 세마포어 참조 값을 저장하는 변수의 주소
            // pshared: 0 -> 하나의 프로세스에 의한 세마포어, 0 이외 -> 둘 이상의 프로세스에 의해 접근될 때
            // value: 생성되는 세마포어 초기 값 설정 1 -> 키 값을 가짐(우선), 0 -> 키 값 x

            int sem_post(sem_t *sem); // 세마포어 값 1 증가
            int sem_wait(sem_t *sem); // 호출 시, 세마포어 값 1 감소
           ```

## 7일차
- Final Test
    - HTTP 서버 구현