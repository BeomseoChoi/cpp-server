#pragma once

namespace xxml::builder
{
    //////////////////////////////
    // XML CONTENT
    //////////////////////////////
    class XmlContent
    {
    public:
        enum class Type
        {
            text,
            tag
        };

    public:
        XmlContent(xxml::builder::XmlContent::Type type, int level);
        virtual ~XmlContent() = 0;

    public:
        virtual std::string build() = 0;

    protected:
        xxml::builder::XmlContent::Type get_type();
        int get_level();

        void align_tab(std::stringstream &xml);
        bool validate_no_space(const std::string &input);
        std::string replace_to_entity_reference(const std::string &input);

    private:
        xxml::builder::XmlContent::Type type;
        int level;


    };

    //////////////////////////////
    // TEXT
    //////////////////////////////
    class Text : public XmlContent, public std::enable_shared_from_this<Text>
    {
    public:
        Text(const char *text, int level);
        virtual ~Text() override;

    public:
        virtual std::string build() override;

    private:
        std::string text;
    };

    //////////////////////////////
    // TAG
    //////////////////////////////
    class Tag : public XmlContent, public std::enable_shared_from_this<Tag>
    {
    public:
        enum class End
        {
            regular,
            self_closing
        };

    public:
        Tag(std::shared_ptr<xxml::builder::Tag> parents, const char *name, xxml::builder::Tag::End end, int level);
        virtual ~Tag() override;

    public:
        std::shared_ptr<xxml::builder::Tag> tag(const char *name, xxml::builder::Tag::End end);
        std::shared_ptr<xxml::builder::Tag> attribute(const char *attr, const char *value);
        std::shared_ptr<xxml::builder::Tag> text(const char *value);
        std::shared_ptr<xxml::builder::Tag> close();

        virtual std::string build() override;

    private:
    protected:
        std::weak_ptr<xxml::builder::Tag> parents;
        std::vector<std::shared_ptr<xxml::builder::XmlContent>> children;

    private:
        std::string name;
        xxml::builder::Tag::End end;
        std::vector<std::pair<std::string, std::string>> attributes;
    };

    //////////////////////////////
    // BUILDER
    //////////////////////////////
    class Builder : public xxml::builder::Tag
    {
    public:
        Builder();
        virtual ~Builder() override = default;

    public:
        std::shared_ptr<xxml::builder::Tag> attribute(const char *attr, const char *value) = delete;
        std::shared_ptr<xxml::builder::Tag> value(const char *value) = delete;
        std::shared_ptr<xxml::builder::Tag> close() = delete;

    public:
        virtual std::string build() override;
    };
}