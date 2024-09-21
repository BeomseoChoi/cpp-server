#include <iostream>
#include <expat.h>
#include "xmpp.h"
#include "xxml.h"

// Start element handler
void XMLCALL startElement(void *userData, const char *name, const char **atts);
void XMLCALL endElement(void *userData, const char *name);
void XMLCALL characterData(void *userData, const char *s, int len);

int main()
{
    std::string str = xmpp::initiate_stream(
        "juliet@im.example.com",
        "im.example.com",
        "1.0",
        "en",
        "jabber:client",
        "http://etherx.jabber.org/streams");

    // str = xmpp::features({xmpp::starttls(true), xmpp::starttls(false), xmpp::starttls(true)});
    str = xmpp::failure_starttls();

    std::cout << str << "\n";

    XML_Parser parser = XML_ParserCreate(NULL);
    if (!parser) {
        fprintf(stderr, "Error creating parser\n");
        return 1;
    }
    XML_SetElementHandler(parser, startElement, endElement);
    // XML_SetCharacterDataHandler(parser, characterData);
    int done;

    do {

        if (XML_Parse(parser, str.c_str(), str.length(), done) == XML_STATUS_ERROR) {
            fprintf(stderr, "Parse error at line %lu:\n%s\n",
                    XML_GetCurrentLineNumber(parser),
                    XML_ErrorString(XML_GetErrorCode(parser)));
            XML_ParserFree(parser);
            return 1;
        }
    } while (!done);

    XML_ParserFree(parser);

    return 0;
}

// Start element handler
void XMLCALL startElement(void *userData, const char *name, const char **atts) {
    printf("Start element: %s\n", name);
    if (atts[0] != NULL) {
        printf("Attributes:\n");
        for (int i = 0; atts[i]; i += 2) {
            printf("  %s='%s'\n", atts[i], atts[i + 1]);
        }
    }
}

// End element handler
void XMLCALL endElement(void *userData, const char *name) {
    printf("End element: %s\n", name);
}

// Character data handler
void XMLCALL characterData(void *userData, const char *s, int len) {
    char data[len + 1];
    snprintf(data, len + 1, "%s", s);
    printf("Character data: %s\n", data);
}