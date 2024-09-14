// server.cpp
#include "pch.h"
#include "chat_server.h"

int main() {
    std::unique_ptr<app::ChatServer> server = std::make_unique<app::ChatServer>();
    server->listening(8080);
    



    return 0;
}
