#!/bin/sh
# Abel-on-C directories
ABEL_ON_C_ROOT=${ABELC_ROOT}
INCDIR="$ABEL_ON_C_ROOT/include"
SRCDIR="$ABEL_ON_C_ROOT/src"
BLDDIR="$ABEL_ON_C_ROOT/build"

echo "******************************************"
echo "* Abel-on-C : Unittest : Header : symbol *"
echo "******************************************"

echo "-- Compile library source files --"
# common
gcc -g -std=c17 -Wall -c $SRCDIR/error.c -o $BLDDIR/error.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/generic.c -o $BLDDIR/generic.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/option.c -o $BLDDIR/option.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/astring.c -o $BLDDIR/astring.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/vector.c -o $BLDDIR/vector.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/linked_list.c -o $BLDDIR/linked_list.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/map.c -o $BLDDIR/map.o -I $INCDIR
gcc -g -std=c17 -Wall -c $SRCDIR/common.c -o $BLDDIR/commom.o -I $INCDIR
# typefied
gcc -g -std=c17 -Wall -c $SRCDIR/typefy.c -o $BLDDIR/typefy.o -I $INCDIR
# symbol
gcc -g -std=c17 -Wall -c $SRCDIR/symbol.c -o $BLDDIR/symbol.o -I $INCDIR

echo "-- Compile local unitetst source files --"
gcc -std=c17 -g -Wall -c ./unittest.c -o ./unittest.o -I $INCDIR

echo "-- Link all object files --"
gcc -std=c17 -g -Wall \
    $BLDDIR/error.o \
    $BLDDIR/generic.o \
    $BLDDIR/option.o \
    $BLDDIR/astring.o \
    $BLDDIR/vector.o \
    $BLDDIR/linked_list.o \
    $BLDDIR/map.o \
    $BLDDIR/common.o \
    $BLDDIR/typefy.o \
    $BLDDIR/symbol.o \
    ./unittest.o  -o ./unittest.out

echo "-- Run executable --"
if [ "$1" = "leak-check" ];then
    valgrind --leak-check=yes ./unittest.out
else
    ./unittest.out
fi