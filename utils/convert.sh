#!/usr/bin/bash
IMAGE="../resources/mono98.jpeg"

echo $1

if [ "$1" == "BG" ]; then
    echo "No background"
else
    ./imgconvert $IMAGE ../romdisk/contents/BACKGROUND --colours 256 --lab
fi