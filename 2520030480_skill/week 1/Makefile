CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c
OBJ = build/main.o
TARGET = build/week1

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf build/*