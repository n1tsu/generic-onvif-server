CXX      ?= g++
CXXFLAGS  = -std=c++11 -O2  -Wall  -pipe
CXXFLAGS += -I$(GENERATED_DIR) -I$(GSOAP_IMPORT_DIR) -I$(GSOAP_DIR) \
            -I$(GSOAP_CUSTOM_DIR) -I$(GSOAP_PLUGIN_DIR)

TARGET     := onvif-server
SRC_DIR     = src
SERVICE_DIR = $(SRC_DIR)/services
WSDD_DIR    = $(SRC_DIR)/wsdd

# gSOAP

WSDL_FILES       := $(wildcard wsdl/*.wsdl wsdl/*.xsd)
GENERATED_DIR    := generated
GSOAP_DIR        := gsoap
GSOAP_IMPORT_DIR := $(GSOAP_DIR)/import
GSOAP_CUSTOM_DIR := $(GSOAP_DIR)/custom
GSOAP_PLUGIN_DIR := $(GSOAP_DIR)/plugin


SRC  = $(SRC_DIR)/main.cpp                 \
       $(SERVICE_DIR)/ServiceDevice.cpp    \
       $(SERVICE_DIR)/ServiceMedia.cpp     \
       $(SERVICE_DIR)/ServicePTZ.cpp       \
       $(SERVICE_DIR)/ServiceImaging.cpp   \
#       $(SERVICE_DIR)/ServiceDiscovery.cpp \
#       $(WSDD_DIR)/wsdd.cpp                \

# gSOAP sources
SRC += $(GSOAP_DIR)/stdsoap2.cpp            \
       $(GSOAP_DIR)/dom.cpp                 \
       $(GSOAP_CUSTOM_DIR)/duration.c       \
       $(GSOAP_CUSTOM_DIR)/struct_timeval.c

# generated sources
SRC += $(GENERATED_DIR)/soapDeviceBindingService.cpp           \
       $(GENERATED_DIR)/soapImagingBindingService.cpp          \
       $(GENERATED_DIR)/soapMediaBindingService.cpp            \
       $(GENERATED_DIR)/soapPTZBindingService.cpp              \
       $(GENERATED_DIR)/soapC.cpp                              \
#       $(GENERATED_DIR)/soapRemoteDiscoveryBindingService.cpp  \
#       $(GENERATED_DIR)/soapwsddService.cpp                    \
#       $(GENERATED_DIR)/wsddClient.cpp                         \


# objects

OBJS := $(patsubst %.cpp, %.o, $(SRC))
OBJS := $(patsubst %.c,   %.o, $(OBJS))

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)


# targets

all: $(TARGET)

# Little trick to ensure gsoap files will get generated before
# compilation even with parallelism (-jN)
$(TARGET):
	$(MAKE) gsoap_target
	$(MAKE) compilation

compilation: $(OBJS)
	$(CXX) -o $(TARGET) $^ $(CXXFLAGS)

gsoap_target: $(GENERATED_DIR)/soapC.cpp $(GENERATED_DIR)/wsddClient.cpp


$(GENERATED_DIR)/onvif.h:
	@mkdir -p $(GENERATED_DIR)
	wsdl2h -d -t ./wsdl/typemap.dat -o $@ $(WSDL_FILES)
	echo '#import "wsse.h" ' >> $@

$(GENERATED_DIR)/soapC.cpp: $(GENERATED_DIR)/onvif.h
	soapcpp2 -a -j -L -x -S -d $(GENERATED_DIR) -I$(GSOAP_DIR):$(GSOAP_IMPORT_DIR) $<

$(GENERATED_DIR)/wsddClient.cpp: $(GSOAP_IMPORT_DIR)/wsdd.h
	soapcpp2 -a -L -x -pwsdd -d $(GENERATED_DIR) -I$(GSOAP_DIR):$(GSOAP_IMPORT_DIR) $<



clean:
	@rm -rf $(GENERATED_DIR)
	@rm -f $(OBJS)

.PHONY: all clean gsoap_target
