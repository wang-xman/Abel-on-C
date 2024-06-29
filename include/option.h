/**
 * Header option.h
 *
 * Return option
 *
 * Throwing error in C can cause memory leak if not handled
 * properly. Abel-on-C propagates error via function return.
 * 
 * Should a function trigger an error, an option is returned.
 * (I must acknowledge the inspiration from Rust language.)
 * The return option either signals that there is no error,
 * via its field `okay`, or contains an error in field `error`.
 *
 * Constructor, function name starts with "abel_make_", must
 * return the intended instance directly. However, Abel-on-C
 * makes extensive use of pointers. Should the returned be a
 * pointer, then NULL - not option - is returned in the event
 * of failure to make it.
 */
#ifndef ABEL_ON_C_OPTION_H
#define ABEL_ON_C_OPTION_H

#include "generic.h"

/**
 * Option for return
 *   
 * Return option is a union for all returns that may contain
 * error.
 *
 * Since it is a union, either a void pointer or an error can
 * be returned from the function, making error propagation
 * in Abel an easy practice.
 **/
struct abel_return_option {
    enum abel_bool_type is_okay;
    enum abel_bool_type is_error;
    union {
        void* pointer;
        struct abel_error error;
    }; 
};

struct abel_return_option abel_option_okay(void* ptr);

struct abel_return_option abel_option_error(struct abel_error err);

#endif