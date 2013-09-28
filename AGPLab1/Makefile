CPPC=g++
CPPFLAGS=-g -c -Wall -I../include
LDFLAGS=-g -L../lib -lSDL2 -lrt -ldl -lGL -lGLEW

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
