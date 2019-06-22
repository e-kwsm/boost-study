CXX      := clang++
CXXFLAGS := -std=c++17
CPPFLAGS := -DGRAPHVIZ
SRCS     := $(wildcard *.cpp)
TARGET   := $(SRCS:.cpp=)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
