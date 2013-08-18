# SVN Info: $Id$

#####################################################################
##                                                                 ##
## TAPSsc Makefile                                                 ##
##                                                                 ##
#####################################################################

# --------------------------- System and ROOT variables ---------------------------

S             = src
I             = include
O             = obj
L             = lib
B             = bin

SRC           = $(wildcard $(S)/TT*.cxx) $(S)/G__TAPSsc.cxx
INCD          = $(wildcard $(I)/TT*.h)
INC           = $(notdir $(INCD))
OBJD          = $(patsubst $(S)/%.cxx, $(O)/%.o, $(SRC))
OBJ           = $(notdir $(OBJD))

OSTYPE       := $(subst -,,$(shell uname))

ROOTGLIBS    := $(shell root-config --libs --glibs) -lEG -lFoam -lSpectrum
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)

DEP_LIB      := libHist.so libGui.so libRMySQL.so libSpectrum.so

vpath %.cxx $(S)
vpath %.h  $(I)
vpath %.o  $(O)

# ------------------------ Architecture dependent settings ------------------------

ifeq ($(OSTYPE),Darwin)
	LIB_TAPSsc = $(L)/libTAPSsc.dylib
	SOFLAGS = -dynamiclib -single_module -undefined dynamic_lookup -install_name $(CURDIR)/$(LIB_TAPSsc)
	POST_LIB_BUILD = @ln $(L)/libTAPSsc.dylib $(L)/libTAPSsc.so
endif

ifeq ($(OSTYPE),Linux)
	LIB_TAPSsc = $(L)/libTAPSsc.so
	SOFLAGS = -shared
	POST_LIB_BUILD = 
endif

# -------------------------------- Compile options --------------------------------

CCCOMP      = g++
CXXFLAGS    = -g -O2 -Wall -fPIC $(ROOTCFLAGS) -I./$(I)
LDFLAGS     = -g -O2 $(ROOTLDFLAGS)

# ------------------------------------ targets ------------------------------------

all:	begin $(LIB_TAPSsc) $(L)/libTAPSsc.rootmap $(B)/TAPSServer end

begin:
	@echo
	@echo "-> Building TAPSsc on a $(OSTYPE) system"
	@echo

end:
	@echo
	@echo "-> Finished!"
	@echo

$(B)/TAPSServer: $(LIB_TAPSsc) $(S)/MainTAPSServer.cxx
	@echo "Building the TAPSServer application"
	@mkdir -p $(B)
	@$(CCCOMP) $(CXXFLAGS) $(ROOTGLIBS) $(CURDIR)/$(LIB_TAPSsc) -o $(B)/TAPSServer $(S)/MainTAPSServer.cxx

$(LIB_TAPSsc): $(OBJ)
	@echo
	@echo "Building libTAPSsc"
	@mkdir -p $(L)
	@rm -f $(L)/libTAPSsc.*
	@$(CCCOMP) $(LDFLAGS) $(SOFLAGS) $(OBJD) -o $(LIB_TAPSsc)
	@$(POST_LIB_BUILD)

$(L)/libTAPSsc.rootmap: $(LIB_TAPSsc)
	@echo "Creating ROOT map"
	@rlibmap -o $(L)/libTAPSsc.rootmap -l $(LIB_TAPSsc) -d $(DEP_LIB) -c $(I)/LinkDef.h

$(S)/G__TAPSsc.cxx: $(INC) $(I)/LinkDef.h
	@echo
	@echo "Creating TAPSsc dictionary"
	@rootcint -v -f $@ -c -I./$(I) -p $(INC) $(I)/LinkDef.h

%.o: %.cxx
	@echo "Compiling $(notdir $<)"
	@mkdir -p $(O)
	@$(CCCOMP) $(CXXFLAGS) -o $(O)/$@ -c $< 

docs:
	@echo "Creating HTML documentation"
	@rm -r -f htmldoc
	root -b -n -q $(S)/htmldoc.C
	@echo "Done."

clean:
	@echo "Cleaning TAPSsc distribution"
	rm -f $(S)/G__*
	rm -r -f $(L)
	rm -f -r $(O)
	rm -r -f $(B)
	rm -r -f htmldoc
	@echo "Done."
 
