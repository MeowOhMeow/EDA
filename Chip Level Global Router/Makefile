CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXECUTABLE = CGR.exe
else
	RM = rm -f
	EXECUTABLE = CGR
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	$(RM) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(EXECUTABLE)