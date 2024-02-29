CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = genPlot

ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXECUTABLE = genPlot.exe
else
	RM = rm -f
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) -f $(OBJS) $(EXECUTABLE)