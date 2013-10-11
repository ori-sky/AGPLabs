CXX=g++
CXXFLAGS=-g -c -Wall -static-libstdc++ -I../include
LIBPATHS=-L../lib/x86 -L../lib/x64
LDFLAGS=-g $(LIBPATHS) -lSDL2 -lGLEW -lrt -ldl -lpthread -lGL

SRCS=main.cpp BlockGame.cpp ResourceManager.cpp Block.cpp
OBJS=$(SRCS:.cpp=.o)
EXECUTABLE=BlockGame

all: $(SRCS) $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
