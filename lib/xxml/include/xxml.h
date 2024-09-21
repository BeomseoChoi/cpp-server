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
            xml,
            pre_builded_xml,
            doc,
            decl,
            tag,
            attr,
            text,
            close
        };

    public:
        XmlContent(xxml::builder::XmlContent::Type type)
            : type(type)
        {
        }
        virtual ~XmlContent() = default;

        xxml::builder::XmlContent::Type get_type()
        {
            return type;
        }

    private:
        xxml::builder::XmlContent::Type type;
    };

    //////////////////////////////
    // XML DECLARATION
    //////////////////////////////
    class Declaration : public XmlContent
    {
    public:
        Declaration(const std::string_view version, const std::string_view encoding = "UTF-8", const std::string_view standalone = "no")
            : XmlContent(xxml::builder::XmlContent::Type::decl), version(version), encoding(encoding), standalone(standalone)
        {
        }
        virtual ~Declaration() override = default;

        const std::string_view get_version() const
        {
            return version;
        }
        const std::string_view get_encoding() const
        {
            return encoding;
        }
        const std::string_view get_standalone() const
        {
            return standalone;
        }

    private:
        std::string version;
        std::string encoding;
        std::string standalone;
    };

    //////////////////////////////
    // TAG
    //////////////////////////////
    enum class TagType
    {
        regular,
        self_closing,
        menual_closing
    };

    class Tag : public XmlContent, public std::enable_shared_from_this<xxml::builder::Tag>
    {
    public:
        Tag(const std::string_view name, xxml::builder::TagType type)
            : XmlContent(xxml::builder::XmlContent::Type::tag), name(name), type(type), open(true)
        {
        }
        virtual ~Tag() override = default;

        const std::string_view get_name() const
        {
            return name;
        }
        xxml::builder::TagType get_tag_type() const
        {
            return type;
        }

        bool is_open() const
        {
            return open;
        }

        void close_open_tag()
        {
            open = false;
        }

    private:
        std::string name;
        xxml::builder::TagType type;
        bool open;
    };
    //////////////////////////////
    // ATTRIBUTE
    //////////////////////////////
    class Attribute : public XmlContent, public std::enable_shared_from_this<Attribute>
    {
    public:
        Attribute(const std::string_view attr_name, const std::string_view attr_value)
            : XmlContent(xxml::builder::XmlContent::Type::attr), attr_name(attr_name), attr_value(attr_value)
        {
        }
        virtual ~Attribute() override = default;

        const std::string_view get_name() const
        {
            return attr_name;
        }
        const std::string_view get_value() const
        {
            return attr_value;
        }

    private:
        std::string attr_name;
        std::string attr_value;
    };

    //////////////////////////////
    // TEXT
    //////////////////////////////
    class Text : public XmlContent, public std::enable_shared_from_this<Text>
    {
    public:
        Text(const std::string_view text)
            : XmlContent(xxml::builder::XmlContent::Type::text), text(text)
        {
        }

        virtual ~Text() override = default;

        const std::string_view get_text() const
        {
            return text;
        }

    private:
        std::string text;
    };

    //////////////////////////////
    // CLOSE
    //////////////////////////////
    class Close : public XmlContent, public std::enable_shared_from_this<Close>
    {
    public:
        Close()
            : XmlContent(xxml::builder::XmlContent::Type::close)
        {
        }

        virtual ~Close() override = default;
    };


    //////////////////////////////
    // PRE-BUILDED XML
    //////////////////////////////
    class PreBuildedXml : public XmlContent, public std::enable_shared_from_this<PreBuildedXml>
    {
    public:
        PreBuildedXml(const std::string_view text)
            : XmlContent(xxml::builder::XmlContent::Type::pre_builded_xml), text(text)
        {
        }

        virtual ~PreBuildedXml() override = default;

        const std::string_view get_text() const
        {
            return text;
        }

    private:
        std::string text;
    };


    //////////////////////////////
    // BUILDER
    //////////////////////////////
    class XmlDoc : public XmlContent, public std::enable_shared_from_this<XmlDoc>
    {
    public:
        XmlDoc()
            : XmlContent(xxml::builder::XmlContent::Type::xml)
        {
        }

        virtual ~XmlDoc() override = default;

#pragma region Contents
    public:
        std::shared_ptr<xxml::builder::XmlDoc> declaration(const std::string_view version, const std::string_view encoding = "UTF-8", const std::string_view standalone = "no")
        {
            std::shared_ptr<xxml::builder::XmlContent> new_tag = std::make_shared<xxml::builder::Declaration>(version, encoding, standalone);
            contents.push_back(new_tag);

            return shared_from_this();
        }

        std::shared_ptr<xxml::builder::XmlDoc> tag(const std::string_view tag_name, const xxml::builder::TagType type)
        {
            std::shared_ptr<xxml::builder::XmlContent> new_tag = std::make_shared<xxml::builder::Tag>(tag_name, type);
            contents.push_back(new_tag);
            tag_map[tag_name] = new_tag;

            return shared_from_this();
        }

        std::shared_ptr<xxml::builder::XmlDoc> attribute(const std::string_view attr_name, const std::string_view attr_value)
        {
            std::shared_ptr<xxml::builder::XmlContent> new_attr = std::make_shared<xxml::builder::Attribute>(attr_name, attr_value);
            contents.push_back(new_attr);

            return shared_from_this();
        }

        std::shared_ptr<xxml::builder::XmlDoc> text(const std::string_view text_value)
        {
            std::shared_ptr<xxml::builder::XmlContent> new_text = std::make_shared<xxml::builder::Text>(text_value);
            contents.push_back(new_text);

            return shared_from_this();
        }

        std::shared_ptr<xxml::builder::XmlDoc> close()
        {
            std::shared_ptr<xxml::builder::XmlContent> new_close = std::make_shared<xxml::builder::Close>();
            contents.push_back(new_close);

            return shared_from_this();
        }

        std::shared_ptr<xxml::builder::XmlDoc> xml(std::shared_ptr<xxml::builder::XmlDoc> new_xml)
        {
            contents.push_back(new_xml);

            return shared_from_this();
        }
        
        std::shared_ptr<xxml::builder::XmlDoc> pre_builded_xml(const std::string_view pre_builded_xml)
        {
            std::shared_ptr<xxml::builder::XmlContent> new_pre_builded_xml = std::make_shared<xxml::builder::PreBuildedXml>(pre_builded_xml);
            contents.push_back(new_pre_builded_xml);

            return shared_from_this();
        }

#pragma endregion

    private:
        std::unordered_map<std::string_view, std::shared_ptr<xxml::builder::XmlContent>> tag_map;

    public:
        std::string build(const int base_level = 0);
        void close_open_tag(std::stringstream &ss, std::shared_ptr<xxml::builder::Tag> tag);

    private:
        std::vector<std::shared_ptr<xxml::builder::XmlContent>> contents;
    };
}

namespace xxml
{
    std::shared_ptr<xxml::builder::XmlDoc> get_builder();
    bool validate(const std::string &xml);
}