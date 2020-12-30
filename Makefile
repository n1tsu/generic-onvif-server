CXX      ?= g++
CXXFLAGS := -std=c++11 -O2  -Wall  -pipe


# gSOAP

WSDL_FILES       := $(wildcard wsdl/*.wsdl wsdl/*.xsd)
GENERATED_DIR    := generated
GSOAP_DIR        := gsoap
GSOAP_IMPORT_DIR := $(GSOAP_DIR)/import


all: $(GENERATED_DIR)/onvif.h $(GENERATED_DIR)/soapC.cpp $(GENERATED_DIR)/wsddClient.cpp


# gSOAP

$(GENERATED_DIR)/onvif.h:
	@mkdir -p $(GENERATED_DIR)
	wsdl2h -d -t ./wsdl/typemap.dat -o $@ $(WSDL_FILES)

$(GENERATED_DIR)/soapC.cpp: $(GENERATED_DIR)/onvif.h
	soapcpp2 -a -j -L -x -S -d $(GENERATED_DIR) -I$(GSOAP_DIR):$(GSOAP_IMPORT_DIR) $<

$(GENERATED_DIR)/wsddClient.cpp: $(GSOAP_IMPORT_DIR)/wsdd.h
	soapcpp2 -a -L -x -pwsdd -d $(GENERATED_DIR) -I$(GSOAP_DIR):$(GSOAP_IMPORT_DIR) $<


clean:
	@rm -rf $(GENERATED_DIR)

.PHONY: all clean
