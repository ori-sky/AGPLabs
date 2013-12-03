CXX=g++
CXXFLAGS=-g -c -Wall -static-libstdc++ -I../include
LIBPATHS=-L../lib/x86 -L../lib/x64
LDFLAGS=-g $(LIBPATHS) -lSDL2 -lGLEW -lrt -ldl -lpthread -lGL

SRCS=main.cpp Game.cpp ResourceManager.cpp LightingManager.cpp ParticlesDrawable.cpp
OBJS=$(SRCS:.cpp=.o)
EXECUTABLE=Project

all: $(SRCS) $(EXECUTABLE)

again: clean all

clean:
	rm -f $(OBJS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
