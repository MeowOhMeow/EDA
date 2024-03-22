# Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -I"C:/Program Files/boost/boost_1_84_0"
LDFLAGS = -L"C:/Program Files/boost/boost_1_84_0/libs"

# Find all cpp files in the current directory
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
# Create a list of executable names by stripping the .cpp extension
ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXECUTABLES = $(SRCS:.cpp=.exe)
else
	RM = rm -f
	EXECUTABLES = $(SRCS:.cpp=)
endif

# Default target
all: $(EXECUTABLES)

# Linking
%.exe: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	$(RM) $(EXECUTABLES) $(OBJS)

# Prevent make from doing anything with files named 'clean' or 'all'
.PHONY: all clean
