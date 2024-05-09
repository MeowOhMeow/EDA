# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -I../cplex_studio/concert/include -I../cplex_studio/cplex/include   #tell the compiler where to find the library when you include the .h file in your code.  concert is the library which support c++ in cplex 
# Linker flags
LDFLAGS = -L../cplex_studio/cplex/lib/x86-64_linux/static_pic -L../cplex_studio/concert/lib/x86-64_linux/static_pic -lilocplex -lcplex -lconcert -lm -lpthread -ldl   #specify the path of library archives 
# Executable name
TARGET = my_program

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

