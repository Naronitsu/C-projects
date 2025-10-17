# Simple Makefile for stack program
# Simple Makefile for stack program

COMPILATIONS_DIR = Compilations
TARGET = $(COMPILATIONS_DIR)/main

OBJS = $(COMPILATIONS_DIR)/main.o $(COMPILATIONS_DIR)/stack.o

all: $(TARGET)

$(TARGET): $(COMPILATIONS_DIR) $(OBJS)
	gcc -o $(TARGET) $(OBJS)

$(COMPILATIONS_DIR):
	mkdir -p $(COMPILATIONS_DIR)

$(COMPILATIONS_DIR)/main.o: main.c stack.h
	gcc -c main.c -o $(COMPILATIONS_DIR)/main.o

$(COMPILATIONS_DIR)/stack.o: stack.c stack.h
	gcc -c stack.c -o $(COMPILATIONS_DIR)/stack.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f -r $(COMPILATIONS_DIR)