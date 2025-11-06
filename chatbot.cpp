#include "chatbot.hpp"
#include <iostream>
#include <sstream>

struct CurlResponse {
    std::string data;
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, CurlResponse* response) {
    size_t total_size = size * nmemb;
    response->data.append((char*)contents, total_size);
    return total_size;
}

std::string parseJsonResponse(const std::string& json) {
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    
    if (doc.HasParseError()) {
        return json.find("Error:") != std::string::npos ? json : "Error: Invalid JSON response";
    }
    
    if (doc.HasMember("chat_history") && doc["chat_history"].IsArray()) {
        const auto& history = doc["chat_history"].GetArray();
        if (history.Size() > 0) {
            const auto& lastMsg = history[history.Size() - 1];
            if (lastMsg.HasMember("message") && lastMsg["message"].IsString()) {
                return lastMsg["message"].GetString();
            }
        }
    }
    
    if (doc.HasMember("response") && doc["response"].IsString()) {
        return doc["response"].GetString();
    }
    
    return "Error: No valid response found";
}

std::pair<std::string, std::string> Message::toPair() const {
    return {sender, message};
}

ChatBot::ChatBot(const std::string& bot, const std::string& user) 
    : bot_name(bot), user_name(user),
      api_url("http://guanaco-submitter.guanaco-backend.k2.chaiverse.com/endpoints/onsite/chat"),
      auth_token("Bearer CR_14d43f2bf78b4b0590c2a8b87f354746") {
    initSafetyPrompt();
}

void ChatBot::initSafetyPrompt() {
    chat_history.emplace_back("Bot", "Please avoid using profanity, or being rude. Be courteous and use language which is appropriate for any audience.");
    chat_history.emplace_back("User", "Alright");
}

std::string ChatBot::sendMessage(const std::string& message) {
    chat_history.emplace_back(user_name, message);
    
    // Build JSON payload
    std::ostringstream json;
    json << "{\n";
    json << "  \"memory\": \"\",\n";
    json << "  \"prompt\": \"\",\n";
    json << "  \"bot_name\": \"" << bot_name << "\",\n";
    json << "  \"user_name\": \"" << user_name << "\",\n";
    json << "  \"chat_history\": [\n";
    
    for (size_t i = 0; i < chat_history.size(); ++i) {
        json << "    {\"sender\": \"" << chat_history[i].sender 
             << "\", \"message\": \"" << chat_history[i].message << "\"}";
        if (i < chat_history.size() - 1) json << ",";
        json << "\n";
    }
    json << "  ]\n}";
    
    std::string response = makeHttpRequest(json.str());
    
    std::string bot_response = parseJsonResponse(response);
    
    chat_history.emplace_back(bot_name, bot_response);
    return bot_response;
}

std::vector<std::pair<std::string, std::string>> ChatBot::getChatHistory() const {
    std::vector<std::pair<std::string, std::string>> history;
    for (const auto& msg : chat_history) {
        history.push_back(msg.toPair());
    }
    return history;
}

std::string ChatBot::makeHttpRequest(const std::string& json_payload) {
    CURL* curl;
    CURLcode res;
    CurlResponse response;
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string auth_header = "Authorization: " + auth_token;
        headers = curl_slist_append(headers, auth_header.c_str());
        
        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
        if(res != CURLE_OK) {
            return "Error: " + std::string(curl_easy_strerror(res));
        }
    }
    
    return response.data;
}

void ChatBot::clearHistory() {
    chat_history.clear();
    initSafetyPrompt();
}

MockChatBot::MockChatBot(const std::string& bot, const std::string& user) 
    : ChatBot(bot, user) {}

std::string MockChatBot::sendMessage(const std::string& message) {
    chat_history.emplace_back(user_name, message);
    
    std::string mock_response = "Mock response to: " + message;
    chat_history.emplace_back(bot_name, mock_response);
    
    return mock_response;
}
