# TCPIP-socket-programming

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