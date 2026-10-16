#!/bin/bash

SIM=~/ostep-homework/vm-paging/paging-linear-translate.py

echo "=== Original case 1: extremely small 8-byte pages ==="
python3 "$SIM" -P 8 -a 32 -p 1024 -v -s 1 -c

echo "=== Original case 2: 8-KB pages ==="
python3 "$SIM" -P 8k -a 32k -p 1m -v -s 2 -c

echo "=== Original case 3: very large 1-MB pages ==="
python3 "$SIM" -P 1m -a 256m -p 512m -v -s 3 -c

echo "=== Edge case: one page covers entire address space ==="
python3 "$SIM" -P 32k -a 32k -p 1m -v -s 4 -c

echo "=== Edge case: very small pages ==="
python3 "$SIM" -P 16 -a 32k -p 1m -v -s 5 -c

echo "=== Edge case: large pages ==="
python3 "$SIM" -P 16k -a 32k -p 1m -v -s 6 -c
