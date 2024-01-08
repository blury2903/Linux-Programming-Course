.PHONY := objs shared_lib static_lib
LIB_NAME := callNumber

CUR_DIR := .

BIN_DIR := $(CUR_DIR)/bin
INC_DIR := $(CUR_DIR)/inc
LIB_DIR := $(CUR_DIR)/lib
OBJ_DIR := $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src

LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

CC := gcc

objs:
	$(CC) -c -fPIC $(CUR_DIR)/main.c    -o $(OBJ_DIR)/main.o -I $(INC_DIR)
	$(CC) -c -fPIC $(SRC_DIR)/zero.c    -o $(OBJ_DIR)/zero.o -I $(INC_DIR)
	$(CC) -c -fPIC $(SRC_DIR)/notZero.c    -o $(OBJ_DIR)/notZero.o -I $(INC_DIR)

shared_lib: objs
	$(CC) -shared $(OBJ_DIR)/zero.o $(OBJ_DIR)/notZero.o -o $(LIB_SHARED_DIR)/lib$(LIB_NAME).so

static_lib: objs
	ar rcs $(LIB_STATIC_DIR)/lib$(LIB_NAME).a $(OBJ_DIR)/zero.o $(OBJ_DIR)/notZero.o

install:
	cp -f $(LIB_SHARED_DIR)/lib$(LIB_NAME).so /usr/lib

all: objs shared_lib static_lib install
	$(CC)  $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/use-shared-library
	$(CC)   $(OBJ_DIR)/main.o  -L$(LIB_STATIC_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/statically-linked

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_STATIC_DIR)/*
	rm -rf $(LIB_SHARED_DIR)/*
	rm -rf $(BIN_DIR)/*