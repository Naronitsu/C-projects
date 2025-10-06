#!/bin/bash
# filepath: /home/damiann/Documents/C projects/build.sh

SRC_DIR="C Programming"
OUT_DIR="Compilations"

mkdir -p "$OUT_DIR"

shopt -s nullglob
for file in "$SRC_DIR"/*.c; do
    filename=$(basename "$file" .c)
    echo "Compiling $file..."
    gcc "$file" -o "$OUT_DIR/$filename"
    if [ $? -ne 0 ]; then
        echo "Failed to compile $file"
    fi
done

echo "Compilation complete. Binaries are in $OUT_DIR/"