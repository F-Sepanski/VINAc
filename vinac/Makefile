CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/directory.c src/member.c src/archiver.c
OBJ = $(SRC:.c=.o)
TARGET = vinac

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
