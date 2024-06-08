CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lbgfx -lglfw3 -framework Cocoa -framework IOKit -Wl,-rpath,@executable_path/dependencies/library
DEP_LIB_DIR = dependencies/library
DEP_INCLUDE_DIR = dependencies/include
INCLUDE_DIR = include
OUTPUT = app.out
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -L $(DEP_LIB_DIR) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR) -I $(DEP_INCLUDE_DIR)

clean:
	rm -rf *.o $(OUTPUT)

run: $(OUTPUT)
	./$(APP)