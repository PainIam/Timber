# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I/usr/include/SFML

# Linker flags (Ensure order is correct)
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# Target executable
TARGET = game

# Source files
SRCS =timber.cpp

# Object files (automatically generated from source files)
OBJS = $(SRCS:.cpp=.o)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
