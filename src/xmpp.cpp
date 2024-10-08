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

std::string xmpp::initiate_starttls(bool required)
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

std::string xmpp::close_stream()
{
    auto xml = xxml::get_builder();
    xml->close("stream:stream");

    std::string result = xml->build();

    return result;
}

std::string xmpp::proceed_starttls()
{
    auto xml = xxml::get_builder();
    xml->tag("proceed", xxml::builder::TagType::self_closing)
        ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-tls")
        ->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::failure_starttls()
{
    auto xml = xxml::get_builder();
    xml->tag("failure ", xxml::builder::TagType::self_closing)
        ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-tls")
        ->close("stream:features");

    std::string result = xml->build();

    return result;
}

std::string xmpp::info_query(const std::string_view id, const std::string_view type, const std::vector<std::string_view> feats)
{
    auto xml = xxml::get_builder();
    xml->tag("iq", xxml::builder::TagType::regular);

    for (const std::string_view feat : feats)
    {
        xml->pre_builded_xml(feat);
    }

    xml->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::message(const std::string_view to, const std::string_view message, const std::vector<std::string_view> feats)
{
    auto xml = xxml::get_builder();
    xml->tag("message", xxml::builder::TagType::regular);

    for (const std::string_view feat : feats)
    {
        xml->pre_builded_xml(feat);
    }

    xml->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::bind()
{
    auto xml = xxml::get_builder();
    xml->tag("bind", xxml::builder::TagType::self_closing)
        ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-bind")
        ->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::bind(const std::vector<std::string_view> feats)
{
    auto xml = xxml::get_builder();
    xml->tag("bind", xxml::builder::TagType::regular);

    for (const std::string_view feat : feats)
    {
        xml->pre_builded_xml(feat);
    }

    xml->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::jid(const std::string_view jid)
{
    auto xml = xxml::get_builder();
    xml->tag("bind", xxml::builder::TagType::regular)
        ->text(jid)
        ->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::jid(const std::string_view local, const std::string_view domain, const std::string_view resource)
{
    std::string full_jid = "";
    full_jid += local;
    full_jid += "@";
    full_jid += domain;
    full_jid += "/";
    full_jid += resource;

    return jid(full_jid);
}

std::string xmpp::jid(const std::string_view local_domain, const std::string_view resource)
{
    std::string full_jid = "";
    full_jid += local_domain;
    full_jid += "/";
    full_jid += resource;

    return jid(full_jid);
}

std::string xmpp::error(const std::string_view type)
{
    auto xml = xxml::get_builder();
    xml->tag("error", xxml::builder::TagType::regular)
        ->attribute("type", type)
        ->tag("conflict", xxml::builder::TagType::regular)
        ->attribute("xmlns", "urn:ietf:params:xml:ns:xmpp-stanzas")
        ->close()
        ->close();

    std::string result = xml->build();

    return result;
}

std::string xmpp::body(const std::string_view text)
{
    auto xml = xxml::get_builder();
    xml->tag("body", xxml::builder::TagType::regular)
        ->text(text)
        ->close();

    std::string result = xml->build();

    return result;
}
