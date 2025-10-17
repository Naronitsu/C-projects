#Declaring variables
COMPILATIONS_DIR = Compilations
TARGET = $(COMPILATIONS_DIR)/main
OBJS = $(COMPILATIONS_DIR)/main.o $(COMPILATIONS_DIR)/stack.o

#Default build all
all: $(TARGET)
	@echo "Build complete."

#Checking directory existance and object files
$(TARGET): $(COMPILATIONS_DIR) $(OBJS)
	@echo "Linking..."
	@gcc -o $(TARGET) $(OBJS)

#Creating compilation directory
$(COMPILATIONS_DIR):
	@echo "Creating directory..."
	@mkdir -p $(COMPILATIONS_DIR)

#Compiling main given main.c and stack.h are compiled
$(COMPILATIONS_DIR)/main.o: main.c stack.h
	@echo "Compiling main.c"
	@gcc -c main.c -o $(COMPILATIONS_DIR)/main.o
	@echo "main.c compiled."

#Compiling stack given stack.c and stack.h are compiled
$(COMPILATIONS_DIR)/stack.o: stack.c stack.h
	@echo "Compiling stack.c"
	@gcc -c stack.c -o $(COMPILATIONS_DIR)/stack.o
	@echo "stack.c compiled."

#Running program (buildng if not built)
run: $(TARGET)
	@echo "Running program..."
	@./$(TARGET)
	@echo "Program finished."

#Cleaning by removing compilation directory
clean:
	@echo "Cleaning up..."
	@rm -f -r $(COMPILATIONS_DIR)
	@echo "Cleanup complete."