#pragma once

namespace core
{

    class Server
    {
    public:
        Server() = default;
        virtual ~Server();

    public:
        ecode listening(const int16 PORT);
        ecode send(byte* buf, int32 len);

    protected:
        virtual void handle_client_accept(int32 client_fd);
        virtual void handle_client_read(int32 client_fd);
        virtual void handle_client_write(int32 client_fd);
        virtual void handle_client_close(int32 client_fd);
    private:
        void set_non_blocking(int32 socket_fd);

        void accept_client_fd(int32 epoll_fd, int32 fd);
        void read_client_fd(int32 epoll_fd, int32 fd);
        void write_client_fd(int32 epoll_fd, int32 fd);
        void close_client_fd(int32 epoll_fd, int32 fd);
        

    protected:

    private:
        int32 server_fd;
        int32 epoll_fd;

        static constexpr int32 MAX_EVENTS = 10;
    };
}