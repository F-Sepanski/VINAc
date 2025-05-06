# Diretórios
SRC_DIR = src
INC_DIR = include
BIN_DIR = $(HOME)/login

# Arquivos fonte
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/archiver.c \
       $(SRC_DIR)/directory.c \
       $(SRC_DIR)/member.c

# Objetos
OBJS = $(SRCS:.c=.o)

# Executável
EXEC = vinac

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR)
LDFLAGS =

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)

.PHONY: all clean
