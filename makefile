# Makefile for ERemove

# Compiler settings - Can change cc to g++ for C++ programs
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Target executable name
TARGET = e-remove
TARGET2 = simulate
# Directory containing test files
TEST_DIR = ./test-files/e-remove
TEST_DIR2 = ./test-files/simulate

# Default target
all: $(TARGET) $(TARGET2)

$(TARGET): testing.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) testing.cpp

$(TARGET2): simulate.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET2) Simulate.cpp

# Test target to run the program with a test file
# Example test file name: test-e-NFA1.txt
# Modify the test file name as needed
test: $(TARGET)
	./$(TARGET) $(TEST_DIR)/test-e-NFA5.txt

test2: $(TARGET2)
	./$(TARGET2) $(TEST_DIR2)/testNFA1.txt $(TEST_DIR2)/testNFA1-strings.txt


# Clean up
clean:
	rm -f $(TARGET) $(TARGET2)
