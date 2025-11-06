#include "chatbot.hpp"
#include <iostream>

int main() {
    MockChatBot bot;
    
    std::string response1 = bot.sendMessage("Hello!");
    std::cout << "Bot: " << response1 << std::endl;
    
    std::string response2 = bot.sendMessage("How are you?");
    std::cout << "Bot: " << response2 << std::endl;
    
    std::cout << "\nChat History:" << std::endl;
    auto history = bot.getChatHistory();
    for (const auto& msg : history) {
        std::cout << msg.first << ": " << msg.second << std::endl;
    }
    
    return 0;
}
