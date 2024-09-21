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

    std::string result = xml->build();

    return result;
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

    std::string result = xml->build();

    return result;
}

std::string xmpp::features(const std::vector<std::string_view> feats)
{
    auto xml = xxml::get_builder();
    xml->tag("stream:feature", xxml::builder::TagType::regular);

    for (const std::string_view feat : feats)
    {
        xml->pre_builded_xml(feat);
    }

    xml->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::starttls(bool required)
{
    auto xml = xxml::get_builder();

    if (required)
    {
        xml->tag("starttls", xxml::builder::TagType::regular)
            ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-tls")
            ->tag("required", xxml::builder::TagType::self_closing)
            ->close()
            ->close();
    }
    else
    {
        xml->tag("starttls", xxml::builder::TagType::self_closing)
            ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-tls")
            ->close();
    }

    std::string result = xml->build();

    return result;
}
