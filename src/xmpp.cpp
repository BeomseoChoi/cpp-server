#include "xxml.h"
#include "xmpp.h"

std::string xmpp::initiate_stream(
    const std::string_view from,
    const std::string_view to,
    const std::string_view version,
    const std::string_view xml_lang,
    const std::string_view xmlns,
    const std::string_view xmlns_stream)
{
    auto xml = xxml::get_builder();
    xml->declaration("1.0")
        ->tag("stream:stream", xxml::builder::TagType::menual_closing)
        ->attribute("from", from)
        ->attribute("to", to)
        ->attribute("verison", version)
        ->attribute("xml:lang", xml_lang)
        ->attribute("xmlns", xmlns)
        ->attribute("xmlns:stream", xmlns_stream);

    std::string str = xml->build();

    return str;
}

std::string xmpp::reply_stream(const std::string_view from, const std::string_view to, const std::string_view id, const std::string_view version, const std::string_view xml_lang, const std::string_view xmlns, const std::string_view xmlns_stream)
{
    auto xml = xxml::get_builder();
    xml->declaration("1.0")
        ->tag("stream:stream", xxml::builder::TagType::menual_closing)
        ->attribute("from", from)
        ->attribute("to", to)
        ->attribute("id", id)
        ->attribute("verison", version)
        ->attribute("xml:lang", xml_lang)
        ->attribute("xmlns", xmlns)
        ->attribute("xmlns:stream", xmlns_stream);

    std::string str = xml->build();

    return str;
}

std::string xmpp::starttls()
{
    auto xml = xxml::get_builder();
    xml->declaration("1.0")
        ->tag("stream:features", xxml::builder::TagType::regular)
        ->tag("starttls", xxml::builder::TagType::regular)
        ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-tls")
        ->tag("required", xxml::builder::TagType::self_closing)
        ->close()
        ->close();
    std::string str = xml->build();

    return str;
}
