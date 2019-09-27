CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -Wall -pedantic
LDFLAGS=-g
LDLIBS=

SRCS=rv32sim.cpp commands.cpp memory.cpp processor.cpp  
OBJS=$(subst .cpp,.o,$(SRCS))

all: rv32sim

rv32sim: $(OBJS)
	$(CXX) $(LDFLAGS) -o rv32sim $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
