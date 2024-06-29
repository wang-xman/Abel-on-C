/**
 * Header generic.h
 *
 * Declarations of generic types used in Abel.
 * 
 * Conventions
 *
 * #1. All enum, struct, and union are given a name in lower
 *     case. Underscore is used to connect two words.
 * #2. A name that is created using `typedef` must be in
 *     capital and without underscore, such that it visually
 *     represents a type.
 * #3. There is but one exception to rule #2. In Abel a type
 *     that end with trailing `_ptr` is a pointer pointing at
 *     an address on heap. This is a visual reminader that the
 *     resource it holds must be freed.
 * #4. Constant must be in capital and contain underscore to
 *     separate words.
 * #5. Variants in enum must be in full capital letters and use 
 *     underscore to separate two words.
 **/
#ifndef ABEL_ON_C_GENERIC_H
#define ABEL_ON_C_GENERIC_H

/**
 * Onsite unittest flag
 *
 * Optionally compile the unit tests for static functions.
 * Remove before final production code.
 */
#define ABEL_ONSITE_UNITTEST 1

#include "error.h"

/**
 * @brief Bool type
 *
 * Boolean type is defined as an enum that consists of
 * two variants false (0) and true (1).
 */
typedef enum abel_bool_type { false, true } Bool;

/**
 * @brief String
 * 
 * String is a general purpose container of C character
 * array.
 * 
 * In accordance with Abel convention #2, type `string_ptr`
 * is reserved for a pointer pointing at a String created
 * on heap.
 * 
 * @see astring.h
 */
struct abel_string;

/**
 * @struct Vector
 *
 * Abel vector is a dynamically allocated structure. It
 * stores an array (on heap) of void pointers to any
 * data type.
 * 
 * @see vector.h
 */
struct abel_vector;

/**
 * @brief Linked list
 * 
 * Singly linked list that can reference any data type.
 * 
 * @see linked_list.h
 */
struct abel_linked_list;

/**
 * @brief Hash map
 * 
 * Abel map is a hash map that associates a key with a
 * void pointer to any data type, i.e. "key-value pair"
 * or simply "pair". Map holds two `vector_ptr` as its
 * internal vectors.
 * 
 * @see map.h
 */
struct abel_map;

/* Utility functions */

/**
 * @brief Next highest power of 2
 *
 * For a given 32-bit integer, it computes the next highest
 * power of 2. For example, given 9, the next highest power
 * of 2 is 16. It mostly used in calculating the capacity of
 * containers.
 * 
 * @param src_int : An unsigned integer for which the next
 *                 highest power of 2 is computed
 * @return An unsigned integer.
 * @note If the input is 0, it manually returns 1 such that
 *       2^0 is the next highest power of 2.
 * @see vector.h, astring.h
 */
unsigned int next_power_of_two(unsigned int src_int);

#endif