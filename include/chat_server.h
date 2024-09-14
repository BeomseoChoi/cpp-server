#include "core/server.h"

namespace app {

    enum class SessionStatus
    {
        active,
        inactive,
    };

    class ChatServer : public core::Server {
    public:
        ChatServer() = default;
        virtual ~ChatServer() override = default;
    public:
        virtual void handle_client_accept(int32 client_fd) override;
        virtual void handle_client_read(int32 client_fd) override;
        virtual void handle_client_write(int32 client_fd) override;
        virtual void handle_client_close(int32 client_fd) override;

    private:
        uint32 generate_session();

    };
}