#!/bin/sh
# Abel-on-C directories
ABEL_ON_C_ROOT=${ABELC_ROOT}
INCDIR="$ABEL_ON_C_ROOT/include"
SRCDIR="$ABEL_ON_C_ROOT/src"
BLDDIR="$ABEL_ON_C_ROOT/build"

echo "*********************************************"
echo "* Abel-on-C : Unittest : Header : container *"
echo "*********************************************"

echo "-- Compile library source files --"
gcc -g -std=c17 -Wall -c $SRCDIR/error.c -o $BLDDIR/error.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/option.c -o $BLDDIR/option.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/typefy.c -o $BLDDIR/typefy.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/astring.c -o $BLDDIR/astring.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/object.c -o $BLDDIR/object.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/list.c -o $BLDDIR/list.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/dict.c -o $BLDDIR/dict.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/container.c -o $BLDDIR/container.o -I $INCDIR

echo "-- Compile local unitetst source files --"
gcc -std=c17 -g -Wall -c ./unittest.c -o ./unittest.o -I $INCDIR

echo "-- Link all object files --"
gcc -std=c17 -g -Wall \
    $BLDDIR/error.o \
    $BLDDIR/option.o \
    $BLDDIR/typefy.o \
    $BLDDIR/astring.o \
    $BLDDIR/object.o \
    $BLDDIR/list.o \
    $BLDDIR/dict.o \
    $BLDDIR/container.o \
    ./unittest.o  -o ./unittest.out

echo "-- Run executable --"
if [ "$1" = "leak-check" ] then
    valgrind --leak-check=yes ./unittest.out
else
    ./unittest.out
fi