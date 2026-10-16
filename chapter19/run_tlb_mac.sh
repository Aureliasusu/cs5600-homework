#!/bin/bash

TRIALS=100000
OUTPUT="mac_output.txt"

echo "Machine: MacBook Air M2" > "$OUTPUT"
echo "Platform: macOS" >> "$OUTPUT"
echo "CPU pinning: strict CPU pinning not available on macOS" >> "$OUTPUT"
echo "Trials: $TRIALS" >> "$OUTPUT"
echo "" >> "$OUTPUT"

for pages in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096
do
    ./tlb "$pages" "$TRIALS" | tee -a "$OUTPUT"
done
