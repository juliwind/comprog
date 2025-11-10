#!/usr/bin/env bash
set -euo pipefail

SRC="Reset"
DEST="Current"
N=7

mkdir -p "$DEST"

cp -f "$SRC/input.txt" "$DEST/input.txt"
cp -f "$SRC/output.txt" "$DEST/output.txt"

for i in $(seq 1 "$N"); do
  cp -f "$SRC/template.cpp" "$DEST/$i.cpp"
done