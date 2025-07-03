# Makefile for SmartBuffer Library Example

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pedantic
TARGET = example
TEST_TARGET = test
BENCH_TARGET = benchmark
SOURCE = example.cpp
TEST_SOURCE = test.cpp
BENCH_SOURCE = benchmark.cpp
HEADER = smart_buffer.hpp

# Default target
all: $(TARGET)

# Build the example
$(TARGET): $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

# Build the test
$(TEST_TARGET): $(TEST_SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCE) -o $(TEST_TARGET)

# Build the benchmark
$(BENCH_TARGET): $(BENCH_SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) $(BENCH_SOURCE) -o $(BENCH_TARGET)

# Run the example
run: $(TARGET)
	./$(TARGET)

# Run the tests
run-test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run the benchmark
benchmark: $(BENCH_TARGET)
	./$(BENCH_TARGET)

# Run both example and tests
check: run run-test

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET) $(BENCH_TARGET) $(TARGET)-gcc $(TARGET)-clang

# Test with different compilers
test-gcc: $(SOURCE) $(HEADER)
	g++ $(CXXFLAGS) $(SOURCE) -o $(TARGET)-gcc
	./$(TARGET)-gcc

test-clang: $(SOURCE) $(HEADER)
	clang++ $(CXXFLAGS) $(SOURCE) -o $(TARGET)-clang
	./$(TARGET)-clang

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all        - Build the example (default)"
	@echo "  test       - Build the test executable"
	@echo "  run-test   - Build and run unit tests"
	@echo "  benchmark  - Build and run performance benchmark"
	@echo "  run        - Build and run the example"
	@echo "  check      - Run both example and tests"
	@echo "  clean      - Remove build artifacts"
	@echo "  debug      - Build with debug information"
	@echo "  test-gcc   - Test with GCC compiler"
	@echo "  test-clang - Test with Clang compiler"
	@echo "  help       - Show this help message"

.PHONY: all run test run-test benchmark check clean debug test-gcc test-clang help
