#!/bin/sh
# Abel-on-C directories
ABEL_ON_C_ROOT=${ABELC_ROOT}
INCDIR="$ABEL_ON_C_ROOT/include"
SRCDIR="$ABEL_ON_C_ROOT/src"
LIBDIR="$ABEL_ON_C_ROOT/build"
# directory for examples
EXAMPLE_ROOT=${ABELC_ROOT}/examples
BLDDIR="$EXAMPLE_ROOT/build"

echo "************************"
echo "* Abel-on-C : Examples *"
echo "************************"

echo "-- Compile library source files --"
# common
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/error.c -o $BLDDIR/error.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/generic.c -o $BLDDIR/generic.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/option.c -o $BLDDIR/option.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/astring.c -o $BLDDIR/astring.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/vector.c -o $BLDDIR/vector.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/linked_list.c -o $BLDDIR/linked_list.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/map.c -o $BLDDIR/map.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/common.c -o $BLDDIR/common.o -I $INCDIR
# typefied 
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/typefy.c -o $BLDDIR/typefy.o -I $INCDIR
# parser side 
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/symbol.c -o $BLDDIR/symbol.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/util.c -o $BLDDIR/util.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/converter.c -o $BLDDIR/converter.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/json_token.c -o $BLDDIR/json_token.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/json_parser.c -o $BLDDIR/json_parser.o -I $INCDIR
# loader side
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/object.c -o $BLDDIR/object.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/list.c -o $BLDDIR/list.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/dict.c -o $BLDDIR/dict.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/container.c -o $BLDDIR/container.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/json_loader.c -o $BLDDIR/json_loader.o -I $INCDIR
gcc -g -std=c17 -Wall -fPIC -c $SRCDIR/abelc.c -o $BLDDIR/abelc.o -I $INCDIR

echo "-- Compile local examples --"
gcc -std=c17 -g -Wall -fPIC -c ./examples.c -o ./examples.o -I $INCDIR

echo "-- Create shared library --"
 gcc -shared -o $BLDDIR/libabelc.so \
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
     $BLDDIR/util.o \
     $BLDDIR/converter.o \
     $BLDDIR/json_token.o \
     $BLDDIR/json_parser.o \
     $BLDDIR/object.o \
     $BLDDIR/list.o \
     $BLDDIR/dict.o \
     $BLDDIR/container.o \
     $BLDDIR/json_loader.o \
     $BLDDIR/abelc.o
 
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
    $BLDDIR/util.o \
    $BLDDIR/converter.o \
    $BLDDIR/json_token.o \
    $BLDDIR/json_parser.o \
    $BLDDIR/object.o \
    $BLDDIR/list.o \
    $BLDDIR/dict.o \
    $BLDDIR/container.o \
    $BLDDIR/json_loader.o \
    $BLDDIR/abelc.o \
    ./examples.o -o ./examples.out

echo "-- Run executable --"
if [ "$1" = "leak-check" ]; then
    valgrind --leak-check=yes ./examples.out
else
    ./examples.out
fi;