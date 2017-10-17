EXEC_NAME=host-tool
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -std=gnu++11
LDFLAGS=-g 
LDLIBS=-pthread -lreadline

SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(EXEC_NAME) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^ >> ./.depend;

.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXEC_NAME)

distclean: clean
	$(RM) *~ .depend

-include .depend
