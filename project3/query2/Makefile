
CXX=g++
CFLAGS=-g
LDFLAGS=-lgdbm
RM=rm -f
TARGET=query2
GDB_FILE=person.gdb
OBJS=query2.cpp

all: clean \
	tool

tool: $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) 

clean:
	$(RM) $(TARGET) \
	$(RM) $(GDB_FILE)