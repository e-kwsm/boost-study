CXX      := clang++
CXXFLAGS := -std=c++14
CPPFLAGS :=
LDFLAGS  :=
LDLIBS   := -lboost_serialization
TARGET   := a.out
SRCS     := $(wildcard *.cpp)
OBJS     := $(SRCS:.cpp=.o)
DEPS     := $(SRCS:.cpp=.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $< -MMD -MF $*.d

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(TARGET)

.PHONY: distclean
distclean: clean
	rm -f tags

tags: $(SRCS)
	ctags $^

-include $(DEPS)
