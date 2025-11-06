#include <gtest/gtest.h>
#include <memory>
#include "chatbot.hpp"

class ChatBotTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_bot = new MockChatBot();
    }
    
    void TearDown() override {
        delete mock_bot;
    }
    
    MockChatBot* mock_bot;
};

TEST_F(ChatBotTest, InitialHistoryContainsSafetyPrompt) {
    auto history = mock_bot->getChatHistory();
    ASSERT_GE(history.size(), 2);
    EXPECT_EQ(history[0].first, "Bot");
    EXPECT_EQ(history[1].first, "User");
    EXPECT_EQ(history[1].second, "Alright");
}

TEST_F(ChatBotTest, SendMessageAddsToHistory) {
    size_t initial_size = mock_bot->getChatHistory().size();
    
    mock_bot->sendMessage("Hello");
    
    auto history = mock_bot->getChatHistory();
    EXPECT_EQ(history.size(), initial_size + 2); // User message + bot response
    EXPECT_EQ(history[history.size()-2].first, "User");
    EXPECT_EQ(history[history.size()-2].second, "Hello");
    EXPECT_EQ(history[history.size()-1].first, "Einstein");
}

TEST_F(ChatBotTest, MockResponseContainsUserMessage) {
    std::string response = mock_bot->sendMessage("Test message");
    EXPECT_TRUE(response.find("Test message") != std::string::npos);
}

TEST_F(ChatBotTest, ClearHistoryResetsToSafetyPrompt) {
    mock_bot->sendMessage("Hello");
    mock_bot->clearHistory();
    
    auto history = mock_bot->getChatHistory();
    EXPECT_EQ(history.size(), 2);
    EXPECT_EQ(history[0].first, "Bot");
    EXPECT_EQ(history[1].second, "Alright");
}

TEST(MessageTest, ToPairReturnsCorrectValues) {
    Message msg("TestSender", "TestMessage");
    auto pair = msg.toPair();
    
    EXPECT_EQ(pair.first, "TestSender");
    EXPECT_EQ(pair.second, "TestMessage");
}
