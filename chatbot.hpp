#pragma once
#include <string>
#include <vector>
#include <utility>
#include <curl/curl.h>

struct Message {
    std::string sender;
    std::string message;
    
    Message(const std::string& s, const std::string& m) : sender(s), message(m) {}
    std::pair<std::string, std::string> toPair() const;
};

class ChatBot {
protected:
    std::string bot_name;
    std::string user_name;
    std::vector<Message> chat_history;
    std::string api_url;
    std::string auth_token;
    
    void initSafetyPrompt();
    std::string makeHttpRequest(const std::string& json_payload);
    
public:
    ChatBot(const std::string& bot = "Einstein", const std::string& user = "User");
    virtual std::string sendMessage(const std::string& message);
    std::vector<std::pair<std::string, std::string>> getChatHistory() const;
    void clearHistory();
};

class MockChatBot : public ChatBot {
public:
    MockChatBot(const std::string& bot = "Einstein", const std::string& user = "User");
    std::string sendMessage(const std::string& message) override;
};
