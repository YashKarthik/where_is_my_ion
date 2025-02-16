CXX = clang++
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags protobuf) $(shell pkg-config --cflags libcurl)
LDFLAGS = $(shell pkg-config --libs protobuf) $(shell pkg-config --libs libcurl)

# Output binary name
TARGET = gtfs_realtime

# Source files
SRCS = gtfs-realtime.pb.cc main.cpp
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)

# Protobuf related
PROTO_SRC = gtfs-realtime.proto
PROTO_CC = gtfs-realtime.pb.cc
PROTO_H = gtfs-realtime.pb.h

.PHONY: all clean

all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile C++ source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile protobuf generated source
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate protobuf files
$(PROTO_CC) $(PROTO_H): $(PROTO_SRC)
	protoc --cpp_out=. $(PROTO_SRC)

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS) $(PROTO_CC) $(PROTO_H)

# Dependencies
gtfs-realtime.pb.o: $(PROTO_CC) $(PROTO_H)
main.o: main.cpp $(PROTO_H)
