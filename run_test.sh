#!/bin/bash
#
# Script to test converting a Multipaint .bin file to a Koala file


# Show usage message
usage()
{
    echo "Usage: `basename $0` [<[path-to-x64[sc]>]"
}


# Set x64
X64=x64sc-gtk3

# check for alternate emu binary
if [ ! -z "$1" ]; then
    if [ "$1" = "-h" -o "$1" = "--help" ]; then
        usage
        exit 1
    fi
    echo "Setting emu bin to $1"
    X64SC="$1"
fi


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

# Run x64sc to show result
${X64} -autostartprgmode 1 show_koala.prg

