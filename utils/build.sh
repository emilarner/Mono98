#!/usr/bin/bash
CC="gcc"

$CC -O2 -ljpeg -lm -o imgconvert imgconvert.c
$CC -O2 -lm -o pad pad.c