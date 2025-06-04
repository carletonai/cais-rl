CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = mt
SOURCE = mt.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean install 