CPPFLAGS = -std=c++17 -pthread
OPTFLAGS = -O3
DEBUGFLAGS = -g

all: go.cpp
	g++ $(CPPFLAGS) $(OPTFLAGS) go.cpp
debug: go.cpp
	g++ $(CPPFLAGS) $(DEBUGFLAGS) go.cpp
profile: go.cpp
	g++ $(CPPFLAGS) -pg go.cpp
