CXX      := clang++
CXXFLAGS := -std=c++14
CPPFLAGS := #-DGRAPHVIZ
LDFLAGS  :=
LDLIBS   :=
SRCS     := $(wildcard *.cpp)
TARGET   := $(SRCS:.cpp=)

.PHONY: all
all: $(TARGET)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: distclean
distclean: clean
	rm -f tags

tags: $(SRCS)
	ctags $^
