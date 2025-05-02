#!/usr/bin/env bash

set -eo pipefail

if [[ $# -eq 0 ]] ; then
    echo 'Input directory name is required'
    exit 0
fi

input_dir=$1

cp $1/Image0000.png Images/background.png

declare -a knobs=("bass" "contour" "gain" "middle" "treble" "volume")

for knob in "${knobs[@]}"
do
  echo "$knob"
  magick montage -mode concatenate -background transparent -tile 1x $1/$knob/* Images/$knob.png
done
