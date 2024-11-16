#!/usr/bin/bash

make clean

# backgrounds, images, etc

IMAGE_CONV_ARGS=""

for arg in $@; do
  if [[ "$arg" == "-DNO_BACKGROUND" ]]; then
    echo "Building kernel without a main background!"
    IMAGE_CONV_ARGS+="BG"
  fi
done

cd utils/
./build.sh
./convert.sh $IMAGE_CONV_ARGS
cd ..

cd libc/
make
cd ..

cd romdisk/
cd programs
./build_all.sh
cd ..
make
./packer ROMDISK contents/*
cd ..

make test EXTS="$*"