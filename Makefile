CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I.
LIBS = -lcurl
TARGET = chatbot
TEST_TARGET = test_chatbot
SOURCES = main.cpp chatbot.cpp
TEST_SOURCES = test_chatbot.cpp chatbot.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES) -lgtest -lgtest_main -pthread $(LIBS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: clean test
