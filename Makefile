CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = build/organizer
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC) | build
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

build:
	mkdir -p build

clean:
	rm -rf build

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/organizer

uninstall:
	rm -f /usr/local/bin/organizer

.PHONY: all clean install uninstall
