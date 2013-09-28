CPPC=g++
CPPFLAGS=-g -c -Wall -I../include
LIBPATHS=-L../lib/x86 -L../lib/x64
LDFLAGS=-g $(LIBPATHS) -lSDL2 -lGLEW -lrt -ldl -lpthread -lGL

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
