/**
 * Header astring.h
 * 
 * General-purpose string
 * 
 * String copies the given C string (`char*` type) to heap
 * and holds a pointer to it. Abel string holds resource.
 **/
#ifndef ABEL_ON_C_ASTRING_H
#define ABEL_ON_C_ASTRING_H

#include "option.h"

/**
 * @brief String struct
 *
 * Abel library provides a string type to represent the string
 * available in other higher-level language. It is used mostly
 * as a "container" and the associated methods are thus limited.
 *
 * Fields
 * 
 * ptr_array : An array of characters.
 * 
 * length : Total number of characters in the current array.
 *     This doesn't include the last null terminator ('\0').
 * 
 * capacity : Maximal number of characters allowed, including
 *     the last null-terminator.
 */
struct abel_string {
    char* ptr_array;
    size_t length;
    size_t capacity;
};

/**
 * @brief String instance maker
 * 
 * Source string is copied to heap and its address on heap
 * is the internal array `ptr_array` of the String instance.
 * 
 * If the length of the source char array is shorter than
 * or equal to 3, exactly 4 bytes will be allocated, even
 * if the char array is empty. Otherwise, the next highest
 * power of 2 is the amount of memory requested for the
 * string. In this way, the destination string always has
 * enough space to contain the source including the last
 * null-terminator.
 * 
 * This design guarantees the pointer to array is never
 * zero and is thus avoiding the error caused by realloc
 * zero size.
 * 
 * @param src_cstr Source C string of type `char*`. Both
 *                 char* and char[] are accepted.
 */
struct abel_string abel_make_string(char* src_cstr);

/**
 * @brief Make a string from integer
 * 
 * Converts an integer, signed or unsigned, into a string.
 * If a plus sign is provided, the sign will be removed.
 * For example, integer +777 is converted into "777".
 * Minus sign is of course kept in the string. For example
 * integer -88 it converted into "-88".
 * 
 * @param src_int : Source integer to be converted into
 *                  string.
 */
struct abel_string abel_string_from_int(int src_int);

/**
 * @brief Free string instance
 *
 * Call this freer to free the internal array of a text
 * instance created on stack.
 */
void abel_free_string(struct abel_string* ptr_str);

/**
 * @brief Make a string on heap
 * 
 * The length of internal array is determined the same way
 * as function `abel_make_string` and the entire instance
 * is on heap.
 * 
 * @param src_cstr Source C string of type char*.
 *        Both char* and char[] are accepted.
 * @note The source is copied (duplicated) on heap.
 */
struct abel_string* abel_make_string_ptr(char* src_cstr);

/**
 * @brief Free string on heap
 *
 * Two slices of memory to be freed: One part is for the
 * source string copied onto heap, the other is the string
 * instance itself.
 */
void abel_free_string_ptr(struct abel_string* ptr_str);

/**
 * @brief Return length or capacity of a string
 */
size_t abel_string_len(struct abel_string* ptr_str);
size_t abel_string_capacity(struct abel_string* ptr_str);

/**
 * @brief Assign a string or char
 *
 * Assign a C string to the string. This method replaces the
 * current content of the destination string instance.
 * 
 * @param ptr_str Pointer to destination string.
 * @param src_cstr Source C string to be assigned to the
 *                 destination string.
 */
void abel_string_assign(struct abel_string* ptr_str, char* src_cstr);
void abel_string_assign_char(struct abel_string* ptr_str, char src_char);

/**
 * @brief Append a string or char
 *
 * Append a string at the end of the text. This method does
 * not call strcat function. It loads the source string from
 * the end of the destination string, hence avoiding repeating
 * the destination string.
 * 
 * @param ptr_str Pointer to destination string.
 * @param src_cstr Source C string to be appended to the
 *                 destination string.
 */
void abel_string_append(struct abel_string* ptr_str, char* src_cstr);
void abel_string_append_char(struct abel_string* ptr_str, char src_char);

/**
 * @brief Compare strings
 * 
 * Returns true (false) if the internal arrays are identical.
 * The capacity are not required to be the same.
 */
Bool abel_string_eq(const struct abel_string* ptr_str1, const struct abel_string* ptr_str2);
Bool abel_string_eq_cstring(const struct abel_string* ptr_str, const char* src_cstr);
Bool abel_string_eq_char(struct abel_string* ptr_str, char src_char);
Bool abel_cstring_eq(const char* cstr1, const char* cstr2);
Bool abel_cstring_eq_char(const char* src_str, const char src_char);


/**
 * @brief Get a char
 * 
 * Returns a char at the given index. Should the index
 * be out of range, null-terminator is ('\0') returned.
 */
char abel_string_at(struct abel_string* ptr_str, size_t idx);
char abel_string_front(struct abel_string* ptr_str);
char abel_string_back(struct abel_string* ptr_str);


/**
 * @brief Check if string has a char
 * 
 * Verifies if a char exists in the text (exclusing the
 * null-terminator).
 * 
 * @return Bool `true` (`false`) if the char (not) exists.
*/
Bool abel_string_has(struct abel_string* ptr_str, char src_char);

#endif