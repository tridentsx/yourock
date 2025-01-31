CC = g++
CFLAGS = -std=c++11 -I/usr/include/imgui -I/usr/include/imgui/backends -I/usr/include -I/usr/include/GL -I/usr/local/include -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LDFLAGS = -lglfw -lGLEW -lGL -lsigrok -limgui -lglib-2.0 -lstb

SRC = yourock.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = imgui_overlay

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

