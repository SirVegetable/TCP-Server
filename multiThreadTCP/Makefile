CC = g++
CFLAGS = -g -Wall -Wextra -pedantic -std=c++17

# Source files
SRCS = tcpserver.cpp connections.cpp main.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Header files
HDRS = tcpserver.hpp connections.hpp
# Executable name
TARGET = tcpserver

# Default target
all: $(TARGET)

# Linking object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compiling source files into object files
%.o: %.cpp $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning up
clean:
	rm -f $(OBJS) $(TARGET)