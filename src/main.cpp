// server.cpp
#include "pch.h"
#include <sstream>
#include "chat_server.h"
#include "xxml.h"

int main()
{
#pragma region SERVER OPENING
    // std::unique_ptr<app::ChatServer> server = std::make_unique<app::ChatServer>();
    // server->listening(8080);
#pragma endregion

#pragma region XML BUILDER
    std::shared_ptr<xxml::builder::Builder> builder = std::make_shared<xxml::builder::Builder>();
    std::string xml = builder
        ->tag("tag0", xxml::builder::Tag::End::regular)->attribute("name", "Price is 5€ & © 2024 John Doe. Also ¥, £, and © are symbols.")->attribute("A", "B")
            ->text("Price is 5€ & © 2024 John Doe. Also ¥, £, and © are symbols.")
                ->tag("tag0-1", xxml::builder::Tag::End::regular)
                    ->text("123123")
                ->close()
            ->text("asdfasdfsadfasd")
        ->close()

        ->tag("tag_name", xxml::builder::Tag::End::self_closing)->attribute("name", "no name")->attribute("thisis", "second")
        ->close()
        ->build();

    std::cout << xml << std::endl;
    return 0;
#pragma endregion

}

#pragma region AN EXAMPLE OF EXPAT LIB
// #include <expat.h>
// #include <iostream>
// #include <string.h>

// // 태그가 시작될 때 호출되는 콜백 함수
// void XMLCALL startElement(void *userData, const char *name, const char **atts) {
//     std::cout << "Start tag: " << name << std::endl;
//     for (int i = 0; atts[i]; i += 2) {
//         std::cout << "Attribute: " << atts[i] << " = " << atts[i + 1] << std::endl;
//     }
// }

// // 태그가 끝날 때 호출되는 콜백 함수
// void XMLCALL endElement(void *userData, const char *name) {
//     std::cout << "End tag: " << name << std::endl;
// }

// int main() {
//     // Expat 파서 생성
//     XML_Parser parser = XML_ParserCreate(NULL);
//     if (!parser) {
//         std::cerr << "Couldn't allocate memory for parser\n";
//         return 1;
//     }

//     // 콜백 함수 설정
//     XML_SetElementHandler(parser, startElement, endElement);

//     // XML 데이터 준비
//     const char *xml_data = "<stream:stream xmlns:stream='http://etherx.jabber.org/streams' version='1.0' to='example.com'>";

//     // XML 데이터를 파싱
//     if (XML_Parse(parser, xml_data, strlen(xml_data), XML_TRUE) == XML_STATUS_ERROR) {
//         std::cerr << "XML parse error: " << XML_ErrorString(XML_GetErrorCode(parser)) << std::endl;
//     }

//     // 파서 해제
//     XML_ParserFree(parser);
//     return 0;
// }
#pragma endregion