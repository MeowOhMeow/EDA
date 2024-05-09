CPLEX_PATH = $(shell dirname $(shell dirname $(shell dirname $(shell dirname $(shell which cplex)))))
# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -I$(CPLEX_PATH)/concert/include -I$(CPLEX_PATH)/cplex/include   #tell the compiler where to find the library when you include the .h file in your code.  concert is the library which support c++ in cplex 
# Linker flags
LDFLAGS = -L$(CPLEX_PATH)/cplex/lib/x86-64_linux/static_pic -L$(CPLEX_PATH)/concert/lib/x86-64_linux/static_pic -lilocplex -lcplex -lconcert -lm -lpthread -ldl   #specify the path of library archives 
# Executable nameS
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

