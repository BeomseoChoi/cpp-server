// server.cpp
#include "pch.h"
#include "chat_server.h"
#include "xxml.h"

int main()
{
    // std::unique_ptr<app::ChatServer> server = std::make_unique<app::ChatServer>();
    // server->listening(8080);

    std::shared_ptr<xxml::Builder> builder = std::make_shared<xxml::Builder>();
    std::string xml = builder
        ->tag("tag0", xxml::Tag::End::regular)->attribute("name", "Price is 5€ & © 2024 John Doe. Also ¥, £, and © are symbols.")->attribute("A", "B")
            ->text("Price is 5€ & © 2024 John Doe. Also ¥, £, and © are symbols.")
                ->tag("tag0-1", xxml::Tag::End::regular)
                    ->text("123123")
                ->close()
            ->text("asdfasdfsadfasd")
        ->close()

        ->tag("tag_name", xxml::Tag::End::self_closing)->attribute("name", "no name")->attribute("thisis", "second")
        ->close()
        ->build();

    std::cout << xml << std::endl;
    return 0;
}
