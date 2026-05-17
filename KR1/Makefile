CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
TARGET := hello
SOURCES := main.cpp hello.cpp

ifeq ($(OS),Windows_NT)
TARGET := hello.exe
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) hello.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
ifeq ($(OS),Windows_NT)
	cmd /C "set PATH=C:\msys64\ucrt64\bin;%PATH% && .\$(TARGET)"
else
	./$(TARGET)
endif

clean:
ifeq ($(OS),Windows_NT)
	cmd /C del /Q hello.exe hello 2>NUL
else
	rm -f hello hello.exe
endif
