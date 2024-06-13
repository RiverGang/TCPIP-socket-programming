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