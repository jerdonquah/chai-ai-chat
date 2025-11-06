#pragma once
#include <string>
#include <vector>
#include <utility>
#include <curl/curl.h>
#include <rapidjson/document.h>

std::string parseJsonResponse(const std::string& json);

struct Message {
    std::string sender;
    std::string message;
    
    Message(const std::string& s, const std::string& m) : sender(s), message(m) {}
    std::pair<std::string, std::string> toPair() const;
};

class ChatBot {
private:
    static const size_t CLEANUP_THRESHOLD = 10;
    
protected:
    std::string bot_name;
    std::string user_name;
    // For v1, we store all chat history in memory within the ChatBot object.
    // This may cause memory issues on mobile devices with long conversations.
    // Future versions should consider persistent storage or history limits.
    std::vector<Message> chat_history;
    size_t deleted_count;
    const std::string api_url;
    const std::string auth_token;
    
    void initSafetyPrompt();
    std::string makeHttpRequest(const std::string& json_payload);
    void cleanupDeletedMessages();
    
public:
    ChatBot(const std::string& bot = "Einstein", const std::string& user = "User");
    virtual std::string sendMessage(const std::string& message);
    std::vector<std::pair<std::string, std::string>> getChatHistory() const;
    void clearHistory();
    bool removeMessage(size_t index);
};

class MockChatBot : public ChatBot {
public:
    MockChatBot(const std::string& bot = "Einstein", const std::string& user = "User");
    std::string sendMessage(const std::string& message) override;
};
