# SVN Info: $Id$

#####################################################################
##                              TAPSsc                             ##
##                                                                 ##
##                     TAPS slowcontrol software                   ##
##                                                                 ##
##                                                                 ##
##                           (C) 2013 by                           ##
##      Dominik Werthmueller <dominik.werthmueller@unibas.ch>      ##
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

ROOTLIBS      = $(shell root-config --libs)
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLDFLAGS   = $(shell root-config --ldflags)
ROOTLIB       = $(shell root-config --libdir)
ROOTLIBA      = $(ROOTLIB)/libRoot.a -lXpm -lXext -lX11 -lm -ldl -pthread \
                -rdynamic $(ROOTLIB)/libpcre.a -lfreetype -lz -lXft -lz $(ROOTLIB)/liblzma.a

DEP_LIB       = libRMySQL.so

LIB_TAPSsc   = $(L)/libTAPSsc.so

vpath %.cxx $(S)
vpath %.h  $(I)
vpath %.o  $(O)

# -------------------------------- Compile options --------------------------------

CCCOMP      = g++
CXXFLAGS    = -g -O3 -Wall -fPIC $(ROOTCFLAGS) -I./$(I)
CXXAFLAGS   = -g -O3 -s -ffunction-sections -fdata-sections -Wl,--no-undefined -Wl,--as-needed $(ROOTCFLAGS) -I./$(I)
LDFLAGS     = -g -O3 -s -ffunction-sections -fdata-sections $(ROOTLDFLAGS)

# ------------------------------------ targets ------------------------------------

all:	begin $(LIB_TAPSsc) $(L)/libTAPSsc.rootmap $(B)/TAPSServer $(B)/HVTalk end

begin:
	@echo
	@echo "-> Building TAPSsc"
	@echo

end:
	@echo
	@echo "-> Finished!"
	@echo

$(B)/TAPSServer: $(LIB_TAPSsc) $(S)/MainTAPSServer.cxx
	@echo "Building the TAPSServer application"
	@mkdir -p $(B)
	@$(CCCOMP) $(CXXFLAGS) $(ROOTLIBS) -L $(CURDIR)/$(L) -lTAPSsc -o $(B)/TAPSServer $(S)/MainTAPSServer.cxx

$(B)/HVTalk: $(LIB_TAPSsc) $(S)/MainHVTalk.cxx
	@echo "Building the HVTalk application"
	@mkdir -p $(B)
	@$(CCCOMP) $(CXXFLAGS) $(ROOTLIBS) -L $(CURDIR)/$(L) -lTAPSsc -o $(B)/HVTalk $(S)/MainHVTalk.cxx

$(LIB_TAPSsc): $(OBJ)
	@echo
	@echo "Building $(LIB_TAPSsc)"
	@mkdir -p $(L)
	@$(CCCOMP) $(LDFLAGS) -shared $(OBJD) -o $(LIB_TAPSsc)

$(L)/libTAPSsc.rootmap: $(LIB_TAPSsc)
	@echo
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
 
