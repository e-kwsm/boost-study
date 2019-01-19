CXX      := clang++
CXXFLAGS := -std=c++17
CPPFLAGS :=
LDFLAGS  :=
LDLIBS   :=
SRCS     := $(wildcard *.cpp)
TARGET   := $(SRCS:.cpp=)

.PHONY: all
all: $(TARGET)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)
