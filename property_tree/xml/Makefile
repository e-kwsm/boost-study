CXX      := clang++
CXXFLAGS := -std=c++14
CPPFLAGS :=
LDFLAGS  :=
LDLIBS   :=
SRCS     := $(wildcard *.cpp)
TARGET   := $(SRCS:.cpp=)

.PHONY: all clean distclean

all: $(TARGET)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<

clean:
	rm -f $(TARGET)

tags: $(SRCS)
	ctags $^

distclean: clean
	rm -f tags
