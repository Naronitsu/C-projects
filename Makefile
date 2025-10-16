SRC_DIR := CProgramming
OUT_DIR := Compilations
SRCS := $(wildcard $(SRC_DIR)/*.c)
BINS := $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%,$(SRCS))

all: $(OUT_DIR) $(BINS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/%: $(SRC_DIR)/%.c
	gcc $< -o $@

clean:
	rm -rf $(OUT_DIR)/*

debug:
	@echo "SRCS: $(SRCS)"
	@echo "BINS: $(BINS)"

run:
	@echo "Available programs:"
	@for bin in $(BINS); do \
		echo "  $$(basename $$bin)"; \
	done
	@read -p "Enter program name to run: " prog; \
	if [ -f "$(OUT_DIR)/$$prog" ]; then \
		echo "Running $$prog..."; \
		./$(OUT_DIR)/$$prog; \
	else \
		echo "Program $$prog not found. Run 'make' first to compile."; \
	fi

run-interactive: all
	@echo "Available programs:"
	@for bin in $(BINS); do \
		echo "  $$(basename $$bin)"; \
	done
	@read -p "Enter program name to run: " prog; \
	if [ -f "$(OUT_DIR)/$$prog" ]; then \
		echo "Running $$prog..."; \
		./$(OUT_DIR)/$$prog; \
	else \
		echo "Program $$prog not found."; \
	fi

.PHONY: all clean debug run run-interactive