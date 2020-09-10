#!/bin/bash

# Generate converter
make -B
if [ "$?" != "0" ]; then
    exit 1
fi

# Run converter
./mp-to-koa data/logo-topleft.bin logo-topleft.koa
if [ "$?" != "0" ]; then
    exit 1
fi

# Generate C64 binary to show result
make -B show_koala.prg
if [ "$?" != "0" ]; then
    exit 1
fi

# Run x64s to show result
x64sc-gtk3 -autostartprgmode 1 show_koala.prg

