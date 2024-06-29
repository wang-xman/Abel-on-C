/**
 * Header error.h
 *
 * Error header is an independent module in Abel library,
 * and hence the absence of "abel_" prefix in all its
 * public functions.
 * 
 * Error defines error types triggered in the library.
 */
#ifndef ABEL_ON_C_ERROR_H
#define ABEL_ON_C_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Error types
 *
 * A collection of all reportable error types in Abel.
 */
enum error_type {
    NONE,
    MALLOC_FAILURE,     /* common errors */
    REALLOC_FAILURE,
    MAP_FULL,
    DICT_FULL,
    KEY_NOT_FOUND,
    KEY_EXISTS,
    VALIDATION_ERROR,
    INCOMPATIBLE_TYPE,
    OUT_OF_RANGE,
    PARSER_ERROR
};


/**
 * @brief Error type string
 *
 * Each variant defined in `enum error_type` is given a string
 * representation. Strings in this array must be ordered the
 * same way as the above.
 */
extern char* ERROR_TYPE_STRING[];

/**
 * @brief Error struct
 *
 * All errors used in Abel are represented as `abel_error` instances.
 * 
 * Fields
 * 
 * msg : A string carrying the error message
 * 
 * error_type : Error type must be one of the variant in
 *     enum `enum error_type`
 * 
 * line : An integer registers the line number of the line
 *     where the error is triggered
 */
struct abel_error {
    char* msg;
    enum error_type error_type;
    int line;
};


/**
 * @brief Experiment - Global error handling
 * If 0, no error; if 1, error occured.
 */

extern int GLOBAL_ERROR_FLAG;

#define ERROR_NONE ( (struct abel_error) {.msg="NONE", .error_type=NONE, .line=-999} )

/**
 * @brief Error push.
 * 
 * Pushes an error into the stack and replaces the
 * existing one as there is only 1 place for error
 * in the global error stack.
 *
 * @param err An `abel_error` instance.
 */
void abel_error_push(struct abel_error err);

/**
 * @brief Error pop.
 * 
 * Error catcher reads the error from error stack and
 * resets it to `ERROR_NONE`.
 * 
 */
struct abel_error abel_error_pop();

/* End Experiment */

/**
 * @brief Makes a new error instance
 *
 * Params
 * 
 * @param msg_str Error message in string. Required
 * 
 * @param err_type Error type selected from enum `error_type`.
 *     Required
 * 
 * @param line_number An integer that records the number of the
 *     line on which the error is triggered. If the line
 *     number is unknown or unvailable, use any negative
 *     and it is set to -999.
 */
struct abel_error error_new(char* msg_str, enum error_type err_type,
                            int line_number);

/**
 * @brief Getters for error
 *
 * The following getters return exactly what the function
 * name indicates. The input is an error instance.
 */

char* error_get_msg(struct abel_error err);

int error_get_line(struct abel_error err);

enum error_type error_get_type(struct abel_error err);

const char* error_get_type_string(struct abel_error err);

/* Common internal errors */

struct abel_error error_malloc_failure();    // Failed to malloc

struct abel_error error_realloc_failure();    // Failed to realloc

struct abel_error error_out_of_range();

struct abel_error error_map_full();

struct abel_error error_dict_full();

struct abel_error error_key_not_found();

struct abel_error error_key_exists();

struct abel_error error_parser_error(char* msg_str, int line_number);

#endif 