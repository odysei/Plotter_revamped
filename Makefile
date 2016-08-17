# System type
UNAME := $(shell uname)

#############################################
### Project location and makefile commons ###
#############################################

PROJECT_DIR=./
BIN_DIR=./bin

include $(PROJECT_DIR)/Makefile_includes
include $(PROJECT_DIR)/Makefile_flags




#######################
### Target location ###
#######################

# Local BIN_DIR
ifeq ($(strip $(BIN_DIR)),)
	BIN_DIRL=.
else
	BIN_DIRL=$(BIN_DIR)
endif




################################
### Include objects to build ###
################################

include Makefile_objects




###################
### All targets ###
###################

plotter=plotter

all: $(plotter)




######################
### Building block ###
######################

$(plotter): $(objects) src/$(plotter).o
	$(CXX) $(CXXFLAGS) -o $(BIN_DIRL)/$(plotter) src/$(plotter).o \
	$(objects) -L. $(LIBFLAGS)

core: clean_core $(objects_AGraph)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIRL)/$(plotter) src/$(plotter).o \
	$(objects) -L. $(LIBFLAGS)

.PHONY: clean

clean:
	rm -rf $(objects); \
	rm -rf src/$(plotter).o

clean_core:
	rm -rf $(objects_AGraph)
