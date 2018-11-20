CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
TST_DIR=./teste
M_DIR=./

all:  clean t2f2 t2fs api main

t2f2:
	$(CC) -g -m32 -Wall -c $(SRC_DIR)/t2f2_aux.c -o $(BIN_DIR)/t2f2_aux.o -Wall

t2fs:
	$(CC) -g -m32  -Wall -c $(SRC_DIR)/t2fs.c -o $(BIN_DIR)/t2fs.o -Wall
	
main:
	$(CC) -g -m32 -o $(M_DIR)/main $(TST_DIR)/main.c $(LIB_DIR)/lib2fs.a -Wall

api:
	ar  crs  $(LIB_DIR)/lib2fs.a $(LIB_DIR)/apidisk.o $(BIN_DIR)/*.o 

clean:
	find $(BIN_DIR) -name ".o" | grep -v "support" | xargs rm -rf
	rm -rf $(BIN_DIR)/*.a $(LIB_DIR)/*~ $(INC_DIR)/*~ $(BIN_DIR)/*~ $(SRC_DIR)/*~ *~
