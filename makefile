#====== for tfarm =======#
CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
#====== for farm =======#
#CC=gcc
#CXX=g++

#CC=icc
#CXX=icpc
#CFLAGS  = -parallel -par-report3 -par-threshold0 -O3

CFLAGS  = -O2

BINDIR = ./bin
LIBDIR = ./lib

#====== for farm =======#
#ROOT	= /usr/local/cern/root_v5.32.04.x86_64_fc8_gcc4.1.2/bin
#====== for tfarm =======#
#ROOT	= /cern/root/bin/root
ROOT	= /home/dragon/sfw/root-6.08.06-install/bin/root



ROOTFLAGS = $(shell $(ROOT)-config --cflags)
ROOTLIBS = $(shell $(ROOT)-config --libs)
ROOTGLIBS = $(shell $(ROOT)-config --glibs)
CXXFLAGS = -Wall -O2 $(ROOTFLAGS) 
CXXLIBS = $(ROOTLIBS)

TARGET1=     AC_qdc
OBJS1=       AC_qdc.o
TARGET2=     hoge
OBJS2=       hoge.o
TARGET3=     AC_ana2
OBJS3=       AC_ana2.o Setting.o
TARGET9=     AC_ana1
OBJS9=       AC_ana1.o Tree.o ParamMan.o
TARGET10=    AC_calib
OBJS10=      AC_calib.o Tree.o ParamMan.o Setting.o
TARGET11=    AC_tuning
OBJS11=      AC_tuning.o ParamMan.o Setting.o

all: $(TARGET1) \
     $(TARGET3) \
     $(TARGET9) \
     $(TARGET10) \
     $(TARGET11) \

$(LIBDIR)/%.o : %.cc
	$(CXX) $(CFLAGS) -c -o $@ $< $(CXXFLAGS)

$(TARGET1): $(patsubst %,$(LIBDIR)/%,$(OBJS1))
	$(CXX) $(CFLAGS) -o $(BINDIR)/$@ $< $(CXXLIBS) $(CXXFLAGS)
$(TARGET2): $(patsubst %,$(LIBDIR)/%,$(OBJS2))
	$(CXX) $(CFLAGS) -o $(BINDIR)/$@ $< $(CXXLIBS) $(CXXFLAGS)
$(TARGET3): $(patsubst %,$(LIBDIR)/%,$(OBJS3))
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^ $(CXXLIBS)
$(TARGET9): $(patsubst %,$(LIBDIR)/%,$(OBJS9))
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^ $(CXXLIBS)
$(TARGET10): $(patsubst %,$(LIBDIR)/%,$(OBJS10))
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^ $(CXXLIBS)
$(TARGET11): $(patsubst %,$(LIBDIR)/%,$(OBJS11))
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^ $(CXXLIBS)


.PHONY: clean
clean:
	rm -f $(LIBDIR)/*.o core $(BINDIR)/*

