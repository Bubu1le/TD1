CC = gcc
CFLAGS = -Wall -c -std=c99
SRC_DIR = src/
OBJ_DIR = obj
INC = -I include/
EXE = build

all: $(EXE)

$(OBJ_DIR)/pgm.o: $(SRC_DIR)pgm.c | $(OBJ_DIR)
	$(CC) $(INC) $(CFLAGS)  $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXE) : $(OBJ_DIR)/pgm.o
	$(CC) $^ -o $@ 

clean : 
	rm -f $(EXE)
	rm -rf $(OBJ_DIR)
