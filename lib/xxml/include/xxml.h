#pragma once

namespace xxml
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
        XmlContent(xxml::XmlContent::Type type, int level);
        virtual ~XmlContent() = 0;

    public:
        virtual std::string build() = 0;

    protected:
        xxml::XmlContent::Type get_type();
        int get_level();

        void align_tab(std::stringstream &xml);
        bool validate_no_space(const std::string &input);
        std::string replace_to_entity_reference(const std::string &input);

    private:
        xxml::XmlContent::Type type;
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
        Tag(std::shared_ptr<xxml::Tag> parents, const char *name, xxml::Tag::End end, int level);
        virtual ~Tag() override;

    public:
        std::shared_ptr<xxml::Tag> tag(const char *name, xxml::Tag::End end);
        std::shared_ptr<xxml::Tag> attribute(const char *attr, const char *value);
        std::shared_ptr<xxml::Tag> text(const char *value);
        std::shared_ptr<xxml::Tag> close();

        virtual std::string build() override;

    private:
    protected:
        std::weak_ptr<xxml::Tag> parents;
        std::vector<std::shared_ptr<xxml::XmlContent>> children;

    private:
        std::string name;
        xxml::Tag::End end;
        std::vector<std::pair<std::string, std::string>> attributes;
    };

    //////////////////////////////
    // BUILDER
    //////////////////////////////
    class Builder : public xxml::Tag
    {
    public:
        Builder();
        virtual ~Builder() override = default;

    public:
        std::shared_ptr<xxml::Tag> attribute(const char *attr, const char *value) = delete;
        std::shared_ptr<xxml::Tag> value(const char *value) = delete;
        std::shared_ptr<xxml::Tag> close() = delete;

    public:
        virtual std::string build() override;
    };
}