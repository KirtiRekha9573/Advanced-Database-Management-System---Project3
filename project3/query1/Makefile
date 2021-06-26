
CXX=g++
RM=rm -f
TARGET=query1
OBJS=query1.cpp

all: clean \
	tool

tool: $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(CFLAGS)

clean:
	$(RM) $(TARGET)
