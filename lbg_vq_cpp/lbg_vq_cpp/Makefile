
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

CC = g++
LD = g++

LDLIBS = -lm
CFLAGS =
COMP_FLAGS = -c -Wall -g
LINK_FLAGS = -g


TARGET = lbg_vq_cpp

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) core.* *.log

$(TARGET) : $(OBJECTS)
	$(LD) $^ -o $@ $(LDLIBS) $(LINK_FLAGS)

%.o : %.cpp
	$(CC) $(CFLAGS) $(COMP_FLAGS) $< -o $@

