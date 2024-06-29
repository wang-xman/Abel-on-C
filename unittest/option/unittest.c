#include <assert.h>
#include "option.h"

/*
    Return type is the union Option.
    Returns OPTION_OKAY which is of the union type Option.
*/
struct abel_return_option trial_func_return_okay()
{
    struct abel_return_option ret;
    ret.is_okay = true;
    ret.is_error = false;
    ret.pointer = NULL;
    return ret;
}

/*
    Returns an option that has field .error set to contain
    an error.
*/
struct abel_return_option trial_function_return_error()
{
    struct abel_return_option ret;
    ret.is_okay = false;
    ret.is_error = true;
    ret.error = error_new("Test error", VALIDATION_ERROR, __LINE__);
    return ret;
}

void test_return_option()
{
    struct abel_return_option ret;
    ret = trial_func_return_okay();
    assert(ret.is_okay == true);
    assert(ret.is_error == false);
    assert(ret.pointer == NULL);

    ret = trial_function_return_error();
    assert(ret.is_okay == false);
    assert(ret.is_error == true);
    assert(ret.error.error_type == VALIDATION_ERROR);
}

int main()
{
    test_return_option();
}