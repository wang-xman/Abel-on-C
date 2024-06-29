/* unittest: error */
#include <assert.h>
#include "error.h"

void test_error_new()
{
    char* err_msg = "Super error just occured!";
    struct abel_error err_obj = error_new(err_msg, VALIDATION_ERROR, -2);
    assert(err_obj.line == -999);
    assert(err_obj.error_type == VALIDATION_ERROR);
}

void test_error_getters()
{
    char* err_msg = "Super error just occured!";
    struct abel_error err = error_new(err_msg, VALIDATION_ERROR, -2);
    assert(error_get_line(err) == -999);
    assert(strcmp(error_get_msg(err), err_msg) == 0);
    assert(error_get_type(err) == VALIDATION_ERROR);

    err_msg = "Okay, oops does happen!";
    err = error_new(err_msg, MALLOC_FAILURE, 12);
    assert(error_get_line(err) == 12);
    assert(strcmp(error_get_msg(err), err_msg) == 0);
    assert(error_get_type(err) == MALLOC_FAILURE);
    assert(strcmp(error_get_type_string(err), "MALLOC_FAILURE") == 0);
}

void test_error_none_const()
{

}

//void test_error_stack()
//{
//    size_t length = sizeof(GLOBAL_ERROR_STACK) / sizeof(GLOBAL_ERROR_STACK[0]);
//    assert(length == 1);
//}

void test_push_pop()
{
    char* err_msg = "Super error just occured!";
    struct abel_error err = error_new(err_msg, VALIDATION_ERROR, -2);
    abel_error_push(err);
    struct abel_error err_caught = abel_error_pop();
    assert(err_caught.error_type == VALIDATION_ERROR);
    /* calling abel_error_pop resets stack */
    err_caught = abel_error_pop();
    assert(err_caught.error_type == NONE);
}

int main()
{
    test_error_new();
    test_error_getters();
    test_push_pop();
}