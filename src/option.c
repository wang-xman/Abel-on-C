/* Source option.h */
#include "option.h"

struct abel_return_option abel_option_okay(void* ptr)
{
    struct abel_return_option ret = {.is_okay=true, .is_error=false, .pointer=ptr};
    return ret;
}

struct abel_return_option abel_option_error(struct abel_error err)
{
    struct abel_return_option ret = {.is_okay=false, .is_error=true, .error=err};
    return ret;
}