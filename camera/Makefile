CXX ?= g++
CXXFLAGS = -std=c++11 -O2 -Wall -pipe -I.

DUMMY_TARGET = libdummycam.so
V4L2_TARGET = libv4l2cam.so

%.o: %.cpp
	$(CXX) -fPIC -c -o $@ $< $(CXXFLAGS) $(LIBS)


.PHONY: all
all: $(DUMMY_TARGET) $(V4L2_TARGET)


$(DUMMY_TARGET): camera_dummy/camera_dummy.cpp
	$(CXX) $^ -o $@ -shared -fPIC $(CXXFLAGS)

$(V4L2_TARGET): camera_v4l2/camera_v4l2.cpp
	$(CXX) $^ -o $@ -shared -fPIC $(CXXFLAGS)

.PHONY: clean
clean:
	$(RM) $(DUMMY_TARGET) $(OBJS) $(V4L2_TARGET)
