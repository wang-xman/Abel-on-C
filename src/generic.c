/** Source generic.c */
#include "generic.h"

/*
 * For a given 32-bit integer, it computes the next highest power
 * of 2. For example, given 9, the next highest power of 2 is 16.
 */
unsigned int next_power_of_two(unsigned int src_int)
{
    if (src_int == 0) {
        return 1;
    } else {
        src_int--;
        src_int |= src_int >> 1;
        src_int |= src_int >> 2;
        src_int |= src_int >> 4;
        src_int |= src_int >> 8;
        src_int |= src_int >> 16;
        src_int++;
        return src_int;
    }
}