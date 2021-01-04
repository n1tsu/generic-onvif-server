CXX ?= g++
CXXFLAGS = -std=c++11 -O2 -Wall -pipe -I.

DUMMY_TARGET = libdummycam.so

%.o: %.cpp
	$(CXX) -fPIC -c -o $@ $< $(CXXFLAGS) $(LIBS)


.PHONY: all
all: $(DUMMY_TARGET)


$(DUMMY_TARGET): camera_dummy/camera_dummy.cpp
	$(CXX) $^ -o $@ -shared -fPIC $(CXXFLAGS)

.PHONY: clean
clean:
	$(RM) $(DUMMY_TARGET) $(OBJS)
