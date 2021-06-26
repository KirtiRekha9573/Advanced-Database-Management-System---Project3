
CXX=g++
CFLAGS=-g -std=c++17
LDFLAGS=-lgdbm
RM=rm -f
TARGET=query3
IDX_TARGET=buildindex
GDB_FILE=index.gdb
OBJS=query3.cpp database.cpp
INDEX_OBJS=build_index.cpp database.cpp

all: clean \
	tool

tool: $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(CFLAGS)

index:
	$(CXX) -o $(IDX_TARGET) $(INDEX_OBJS) $(LDFLAGS) $(CFLAGS)

clean:
	$(RM) $(TARGET) \
	$(RM) $(IDX_TARGET) \
	$(RM) $(GDB_FILE)