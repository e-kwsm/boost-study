CXX      := clang++
CXXFLAGS := -std=c++17
LDLIBS   := -lboost_serialization
TARGET   := a.out
SRCS     := $(wildcard *.cpp)
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(TARGET)
