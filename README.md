# Chai AI Chat

AI chat application project using C++ with HTTP API integration.

## Dependencies

- **libcurl-devel** - For HTTP requests
- **gtest-devel** - For unit testing
- **g++** with C++11 support

### Installation (Amazon Linux/RHEL/CentOS)
```bash
sudo yum install -y libcurl-devel gtest-devel
```

### Installation (Ubuntu/Debian)
```bash
sudo apt-get install -y libcurl4-openssl-dev libgtest-dev
```

## API Specification

**Endpoint:** `POST http://guanaco-submitter.guanaco-backend.k2.chaiverse.com/endpoints/onsite/chat`

**Headers:**
- `Authorization: Bearer CR_14d43f2bf78b4b0590c2a8b87f354746`
- `Content-Type: application/json`

**Request Body:**
```json
{
    "memory": "",
    "prompt": "",
    "bot_name": "Einstein",
    "user_name": "User",
    "chat_history": [
        {"sender": "Bot", "message": "Safety prompt message"},
        {"sender": "User", "message": "User response"},
        {"sender": "Bot", "message": "Bot message"},
        {"sender": "User", "message": "Latest user message"}
    ]
}
```

## Build & Run

```bash
# Compile main application
make

# Run application
./chatbot

# Run tests
make test

# Clean build files
make clean
```

## Classes

- **Message** - Represents chat messages with sender/message pairs
- **ChatBot** - Main class with real API integration
- **MockChatBot** - Mock implementation for testing

## Usage Example

```cpp
#include "chatbot.hpp"

// Using mock for testing
MockChatBot mock_bot;
std::string response = mock_bot.sendMessage("Hello!");

// Using real API
ChatBot real_bot("Einstein", "User");
std::string api_response = real_bot.sendMessage("Hello Einstein!");
```