#!/bin/bash

TRIALS=100000
OUTPUT="vm_output.txt"

echo "Machine: Ubuntu VM" > "$OUTPUT"
echo "CPU pinning: CPU 0 using taskset" >> "$OUTPUT"
echo "Trials: $TRIALS" >> "$OUTPUT"
echo "" >> "$OUTPUT"

for pages in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096
do
    taskset -c 0 ./tlb "$pages" "$TRIALS" | tee -a "$OUTPUT"
done
