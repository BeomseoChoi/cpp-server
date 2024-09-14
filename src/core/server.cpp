#include "pch.h"
#include "core/server.h"

#pragma region etc

core::Server::~Server()
{
    close(server_fd); // 서버 소켓 종료
    close(epoll_fd);  // epoll 인스턴스 종료
}

void core::Server::set_non_blocking(int32 socket_fd)
{
    // 클라이언트 소켓을 논블로킹 모드로 설정
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        close(socket_fd);
        return;
    }

    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL");
        close(socket_fd);
        return;
    }
}

#pragma endregion

ecode core::Server::listening(const int16 PORT)
{
    int32 server_fd;
    struct sockaddr_in address;
    struct epoll_event ev, events[MAX_EVENTS];
    int addrlen = sizeof(address);

    // 1. 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // 2. 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // 4. 연결 대기
    if (listen(server_fd, 1000) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    // epoll 인스턴스 생성
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        close(server_fd);
        return -1;
    }

    // 서버 소켓을 논블로킹으로 설정
    set_non_blocking(server_fd);

    // 서버 소켓을 epoll에 등록 (읽기 이벤트)
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1)
    {
        perror("epoll_ctl");
        close(server_fd);
        close(epoll_fd);
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    int session_id_counter = 0;

    while (true)
    {
        // epoll 이벤트 대기
        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_fds == -1)
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < num_fds; ++i)
        {
            int32 fd = events[i].data.fd;
            uint32 event_flags = events[i].events;

            if (fd == server_fd)
            {
                // 새로운 클라이언트 연결 처리
                accept_client_fd(epoll_fd, server_fd);
            }
            else
            {
                if (event_flags & EPOLLIN)
                {
                    // 클라이언트 데이터 읽기 처리
                    read_client_fd(epoll_fd, fd);
                }

                if (event_flags & EPOLLOUT)
                {
                    // 클라이언트 데이터 쓰기 처리
                    write_client_fd(epoll_fd, fd);
                }

                if (event_flags & (EPOLLHUP | EPOLLERR | EPOLLRDHUP))
                {
                    // 클라이언트 연결 종료 및 에러 처리
                    close_client_fd(epoll_fd, fd);
                }
            }
        }
    }
}

#pragma region handle_fd_in_server

// 새로운 클라이언트 연결 처리
void core::Server::accept_client_fd(int epoll_fd, int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addrlen);
    if (client_fd == -1)
    {
        perror("accept");
        return;
    }

    set_non_blocking(client_fd);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET; // 엣지 트리거로 설정
    ev.data.fd = client_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
    {
        perror("epoll_ctl: client_fd");
        close(client_fd);
    }

    handle_client_accept(client_fd); // OVERRIDE

    std::printf("Accepted new client: %d\n", client_fd);
}

// 클라이언트로부터 데이터를 읽는 함수
void core::Server::read_client_fd(int epoll_fd, int client_fd)
{
    char buffer[1024] = {0};
    int bytes_read = read(client_fd, buffer, sizeof(buffer));

    if (bytes_read > 0)
    {
        std::cout << "Received from client: " << buffer << std::endl;
        handle_client_read(client_fd);
    }
    else if (bytes_read == 0)
    {
        std::cout << "Client disconnected: fd " << client_fd << std::endl;
        close_client_fd(epoll_fd, client_fd); // 클라이언트 연결 종료
    }
    else
    {
        perror("read");
        close_client_fd(epoll_fd, client_fd);
    }
}

void core::Server::write_client_fd(int epoll_fd, int fd)
{
    handle_client_write(fd);
}

void core::Server::close_client_fd(int epoll_fd, int fd)
{
    // 클라이언트 데이터 정리
    handle_client_close(fd);

    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    {
        perror("epoll_ctl: del");
    }

    close(fd);

    printf("Closed connection with client %d\n", fd);
}

#pragma endregion

#pragma region handle_fd_in_client

void core::Server::handle_client_accept(int32 client_fd)
{
}
void core::Server::handle_client_read(int32 client_fd)
{
}
void core::Server::handle_client_write(int32 client_fd)
{
}
void core::Server::handle_client_close(int32 client_fd)
{
}

#pragma endregion