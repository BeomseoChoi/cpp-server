#include <iostream>
#include "xxml.h"

int main()
{

    std::string from = "juliet@im.example.com";
    std::string to = "im.example.com";
    std::string version = "1.0";
    std::string xml_lang = "en";
    std::string xmlns = "jabber:client";
    std::string xmlns_stream = "http://etherx.jabber.org/streams";

    std::shared_ptr<xxml::builder::XmlDoc> xml = std::make_shared<xxml::builder::XmlDoc>();
    xml->declaration("1.0")
    ->tag("stream:stream", xxml::builder::TagType::menual_closing)
    ->attribute("from", from)
    ->attribute("to", to)
    ->attribute("verison", version)
    ->attribute("xml:lang", xml_lang)
    ->attribute("xmlns", xmlns)
    ->attribute("xmlns:stream", xmlns_stream);

    std::string str = xml->build();

    std::cout << str << "\n";

    return 0;
}
