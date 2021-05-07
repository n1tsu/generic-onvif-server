CXX ?= g++
CXXFLAGS = -std=c++11 -O2 -Wall -pipe -I.

DUMMY_TARGET = libdummycam.so
V4L2_TARGET = libv4l2cam.so
OUT_DIR = lib

%.o: %.cpp
	$(CXX) -fPIC -c -o $@ $< $(CXXFLAGS) $(LIBS)


.PHONY: all
all: $(DUMMY_TARGET) $(V4L2_TARGET)


$(DUMMY_TARGET): camera_dummy/camera_dummy.cpp
	@mkdir -p $(OUT_DIR)
	$(CXX) $^ -o $(OUT_DIR)/$@ -shared -fPIC $(CXXFLAGS)

$(V4L2_TARGET): camera_v4l2/camera_v4l2.cpp
	@mkdir -p $(OUT_DIR)
	$(CXX) $^ -o $(OUT_DIR)/$@ -shared -fPIC $(CXXFLAGS)

.PHONY: clean
clean:
	$(RM) $(OUT_DIR)/$(DUMMY_TARGET) $(OBJS) $(OUT_DIR)/$(V4L2_TARGET)
