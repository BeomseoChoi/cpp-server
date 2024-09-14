#include "pch.h"
#include "chat_server.h"

uint32 app::ChatServer::generate_session()
{
    static uint32 session_count = 0;

    return session_count++;
}

void app::ChatServer::handle_client_accept(int32 client_fd)
{
    std::printf("handle_client_accept\n");
}
void app::ChatServer::handle_client_read(int32 client_fd)
{
    std::printf("handle_client_read\n");
}
void app::ChatServer::handle_client_write(int32 client_fd)
{
    std::printf("handle_client_write\n");
}
void app::ChatServer::handle_client_close(int32 client_fd)
{
    std::printf("handle_client_close\n");
}