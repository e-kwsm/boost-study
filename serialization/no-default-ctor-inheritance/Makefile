CXX      := clang++
CXXFLAGS := -std=c++14
CPPFLAGS :=
LDFLAGS  :=
LDLIBS   := -lboost_serialization
TARGET   := a.out
SRCS     := $(wildcard *.cpp)
OBJS     := $(SRCS:.cpp=.o)
DEPS     := $(SRCS:.cpp=.d)

.PHONY: all clean distclean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $< -MMD -MF $*.d

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(TARGET)

tags: *.h $(SRCS)
	ctags $^

distclean: clean
	rm -f tags

-include $(DEPS)
