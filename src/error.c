/* Source error.c */
#include "error.h"

char* ERROR_TYPE_STRING[] = {
    "NONE",
    "MALLOC_FAILURE",
    "REALLOC_FAILURE",
    "MAP_FULL",
    "DICT_FULL",
    "KEY_NOT_FOUND",
    "KEY_EXISTS",
    "VALIDATION_ERROR",
    "INCOMPATIBLE_TYPE",
    "OUT_OF_RANGE",
    "PARSER_ERROR"
};

/** @brief Experiment - Global error stack */
int GLOBAL_ERROR_FLAG = 0;

static struct abel_error GLOBAL_ERROR_STACK[1] = { ERROR_NONE };

void abel_error_push(struct abel_error err)
{
    GLOBAL_ERROR_STACK[0] = err;
}

/**
 * @brief Reset error stack
 * 
 * Resets global error stack back to ERROR_NONE.
 */
static void reset_error_stack()
{
    GLOBAL_ERROR_STACK[0] = ERROR_NONE;
}

struct abel_error abel_error_pop()
{
    struct abel_error error_in_stack = GLOBAL_ERROR_STACK[0];
    reset_error_stack();
    return error_in_stack;
}

/* End Experiment */

struct abel_error error_new(char* msg_str, enum error_type err_type,
                            int line_number)
{
    struct abel_error err_obj;
    int line_number_temp = -999;
    if (line_number >= 0) {
        line_number_temp = line_number;
    }
    err_obj = (struct abel_error){.msg = msg_str, .error_type = err_type,
                      .line = line_number_temp};
    return err_obj;
}

/* Getters */

char* error_get_msg(struct abel_error err)
{
    return err.msg;
}

int error_get_line(struct abel_error err)
{
    return err.line;
}

enum error_type error_get_type(struct abel_error err)
{
    return err.error_type;
}

const char* error_get_type_string(struct abel_error err)
{
    return ERROR_TYPE_STRING[err.error_type];
}

/* Common internal errors */

struct abel_error error_malloc_failure()
{
    struct abel_error error = error_new("Function malloc() failed.", MALLOC_FAILURE, -999);
    return error;
}

struct abel_error error_realloc_failure()
{
    struct abel_error error = error_new("Function realloc() failed.", REALLOC_FAILURE, -999);
    return error;
}

struct abel_error error_out_of_range()
{
    struct abel_error error = error_new("Index is out of range.", OUT_OF_RANGE, -999);
    return error;
}

struct abel_error error_map_full()
{
    struct abel_error error = error_new("Map full.", MAP_FULL, -999);
    return error;
}

struct abel_error error_dict_full()
{
    struct abel_error error = error_new("Dictionary full.", DICT_FULL, -999);
    return error;
}

struct abel_error error_key_not_found()
{
    struct abel_error error = error_new("Key not found in dictionary.", KEY_NOT_FOUND, -999);
    return error;
}

struct abel_error error_key_exists()
{
    struct abel_error error = error_new("Key already exists in map.", KEY_EXISTS, -999);
    return error;
}

struct abel_error error_parser_error(char* msg_str, int line_number)
{
    struct abel_error error = error_new(msg_str, PARSER_ERROR, line_number);
    return error;
}