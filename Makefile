CFLAGS = -Wall
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	g++ $(CFLAGS) -o RayTracer.exe $(TARGET).cpp

.PHONY clean:
clean:
	-rm -rf *.ppm *.exe