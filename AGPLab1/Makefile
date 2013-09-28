CPPC=g++
CPPFLAGS=-g -c -Wall -I../include
LDFLAGS=-g -lrt -ldl -lGL -lGLEW -lSDL2 -L../lib

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)
EXECUTABLE=AGPLab1

all: $(SRCS) $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CPPC) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CPPC) $(CPPFLAGS) $^ -o $@
