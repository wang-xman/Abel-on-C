/**
 * Header util.h
 * Created 08-11-2022
 * Updated 08-11-2022
 * 
 * Utility functions
 */
#ifndef ABEL_ON_C_UTIL_H
#define ABEL_ON_C_UTIL_H

#include "symbol.h"

/**
 * @brief Remove space from an array of chars
 * 
 * Ther source array will be modified to store the space
 * removed array. There is no return value.
 * 
 * Caution
 * 
 * The source must be a char array. Not applicable to a
 * pointer to a literal as modification is required.
 */
void remove_space(char src_array[]);

/**
 * @brief Scans entire string to find letter
 *
 * If a letter is found, returns true; if not letter found
 * returns false.
 */
Bool has_letter(char* src_str);

/**
 * @brief Verifies if a string begins with letter
 *
 * String is checked as it is, meaning that if the first
 * char is space, the return is false.
 */
Bool begins_with_letter(char* src_str);

/**
 * @brief Verifies if a string begins with sign
 *
 * Sign is either character plus ('+') or minus ('-').
 *  
 * String is checked as it is, meaning that if the first
 * char is space, the return is false.
 */
Bool begins_with_sign(char* src_str);

/**
 * @brief Verifies if a string is an algebraic operator
 *
 * The following strings are algebraic operators
 *     +, -, *, /, ^, **
 * all but one consists of a single character.
 */
Bool is_algebraic_operator(char* src_str);

#endif