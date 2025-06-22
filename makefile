# Makefile for SFML on Windows with g++

# === CONFIG ===
CXX = g++
CXXFLAGS = -I"C:/SFML-2.6.2/include"
LDFLAGS = -L"C:/SFML-2.6.2/lib"
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC = main.cpp
OBJ = main.o
TARGET = game.exe

# === RULES ===
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	del /Q *.o *.exe 2>nul || true

run: all
	./$(TARGET)
