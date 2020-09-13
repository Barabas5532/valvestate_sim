#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Usage: $0 <input file> <output file>"
  exit 1
fi

in_file="$1"
out_file="$2"

magick $in_file -channel A -gamma 0.45 +channel $out_file