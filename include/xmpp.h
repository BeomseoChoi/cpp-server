#pragma once


namespace xmpp
{
    /**
     * The initiating entity opens by sending a stream.
     * The receiving entity replies by sending a stream.
     * 
     * If initial stream inlcudes the version attribute at least 1.0, then MUST reply a <feature/> child element to announce any conditions for continuation of stream negoation process.
     * A condition is a <features/> element. A condition can contain zero or more <features/> child.
     * The order of child elements that contains <features/> is not significant.
     * 
     * Mandatory-to-negotiate (m2n)
     * Voluntary-to-negotiate (v2n)
     * 1. 동일 단계에서 m2n이 여러개 전송되면 클라이언트는 옵션으로 받아들임. 그 중 선택해서 협상하는 것.
     * 2. m2n은 단계적으로 진행되어야 함. TLS가 안끝났는데 SALS를 feature element로 동시에 보낼 수 없음.
     * 3. feature에 m2n이랑 v2n이 동시에 존재할 수 있음.
     * 4. feature에 v2n만 있으면 협상은 이미 끝났고, 추가적인 협상을 진행할 수 있음. 그리고 XML stanza를 보낼 수 있는 상황
     * 5. <features/>를 보내면 협상이 완전이 끝나고 stanza를 보내는 차례임을 말함.
     * 
     * 스트림을 재시작해야하는 feature가 협상될 때, 기존 스트림을 버리거나 </stream>을 보내지 않고 새로운 스트림을 생성함. TCP 연결도 끊으면 안됨.
     * xml decl을 포함하는게 좋음.
     * 새로운 스트림 ID를 생성해야함. 이전과 달라야함.
     * 
     * 
     */
    
    std::string initiate_stream(
        const std::string_view from,
        const std::string_view to,
        const std::string_view version = "1.0",
        const std::string_view xml_lang = "en",
        const std::string_view xmlns = "jabber:client",
        const std::string_view xmlns_stream = "http://etherx.jabber.org/streams");
        
    std::string reply_stream(
        const std::string_view from,
        const std::string_view to,
        const std::string_view id,
        const std::string_view version = "1.0",
        const std::string_view xml_lang = "en",
        const std::string_view xmlns = "jabber:client",
        const std::string_view xmlns_stream = "http://etherx.jabber.org/streams");

    std::string starttls();



    std::string message(
        const std::string_view to,
        const std::string_view message);
    
}