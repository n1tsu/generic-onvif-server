CXX      ?= g++

CXXFLAGS  = -std=c++11 -Wall  -pipe -DBIN_NAME=$(TARGET)
# for WS-Security
CXXFLAGS += -DWITH_OPENSSL -lssl -lcrypto -lz -pthread
# Includes
CXXFLAGS += -I$(GENERATED_DIR) -I$(GSOAP_IMPORT_DIR) -I$(GSOAP_DIR) \
            -I$(GSOAP_CUSTOM_DIR) -I$(GSOAP_PLUGIN_DIR) -I$(UTILS_DIR) \
            -I$(DISCOVERY_DIR) -I$(RTSP_DIR) -I$(CAMERA_DIR) -I$(PARSER_DIR)
# Libraries
CXXFLAGS += -ldl                                                    \
            -lgstbase-1.0                                           \
            -lgobject-2.0                                           \
            `pkg-config --cflags --libs glib-2.0`                   \
            `pkg-config --cflags --libs gstreamer-1.0`              \
            `pkg-config --cflags --libs gstreamer-rtsp-server-1.0`  \

TARGET       := onvif-server
SRC_DIR       = src
SERVICE_DIR   = $(SRC_DIR)/services
DISCOVERY_DIR = $(SRC_DIR)/discovery
UTILS_DIR     = $(SRC_DIR)/utils
RTSP_DIR      = $(SRC_DIR)/rtsp
PARSER_DIR    = $(SRC_DIR)/parser
CAMERA_DIR    = camera

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
       $(SERVICE_DIR)/ServiceDiscovery.cpp \
       $(DISCOVERY_DIR)/discovery.cpp      \
       $(UTILS_DIR)/arguments.cpp          \
       $(UTILS_DIR)/utils.cpp              \
       $(UTILS_DIR)/profile.cpp            \
       $(UTILS_DIR)/ptz.cpp                \
       $(UTILS_DIR)/configuration.cpp      \
       $(RTSP_DIR)/pipeline.cpp            \
       $(PARSER_DIR)/config_parser.cpp            \

# gSOAP sources
SRC += $(GSOAP_DIR)/stdsoap2.cpp            \
       $(GSOAP_DIR)/dom.cpp                 \
       $(GSOAP_CUSTOM_DIR)/duration.c       \
       $(GSOAP_CUSTOM_DIR)/struct_timeval.c \

# gSOAP plugins sources (WS-discover and WS-Security)
SRC += $(GSOAP_PLUGIN_DIR)/wsseapi.c \
       $(GSOAP_PLUGIN_DIR)/mecevp.c  \
       $(GSOAP_PLUGIN_DIR)/smdevp.c  \
       $(GSOAP_PLUGIN_DIR)/wsaapi.c  \
       $(GSOAP_PLUGIN_DIR)/wsddapi.c \
       $(GSOAP_PLUGIN_DIR)/threads.c

# generated sources
SRC += $(GENERATED_DIR)/soapDeviceBindingService.cpp           \
       $(GENERATED_DIR)/soapImagingBindingService.cpp          \
       $(GENERATED_DIR)/soapMediaBindingService.cpp            \
       $(GENERATED_DIR)/soapPTZBindingService.cpp              \
       $(GENERATED_DIR)/soapC.cpp                              \
       $(GENERATED_DIR)/soapRemoteDiscoveryBindingService.cpp  \
       $(GENERATED_DIR)/soapwsddService.cpp                    \
       $(GENERATED_DIR)/wsddClient.cpp                         \


# objects

OBJS := $(patsubst %.cpp, %.o, $(SRC))
OBJS := $(patsubst %.c,   %.o, $(OBJS))

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)


# targets

all: $(TARGET)

debug: CXXFLAGS += -g -fsanitize=address -DDEBUG
debug: $(TARGET)

# Little trick to ensure gsoap files will get generated before
# compilation even with parallelism (-jN)
$(TARGET):
	$(MAKE) gsoap_target CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)"
	$(MAKE) compilation CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)"

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
	@rm -f $(OBJS)
	@rm -f $(TARGET)

deep-clean: clean
	@rm -rf $(GENERATED_DIR)
	@rm -f *.log

.PHONY: all clean deep-clean gsoap_target $(TARGET) debug
