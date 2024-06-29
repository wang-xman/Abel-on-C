/*
 * Unittest object
 *
 * Caution
 * 
 * When calling `abel_free_object_ptr` from outside of
 * any container, make sure to manually change the ref
 * count of the object to 1.
 */
#include <assert.h>
#include "object.h"

/* pointer from primitive and intrinsic types */

void test_make_object_ptr_from_bool()
{
    Bool test_data = false;
    struct abel_object* ptr_test_object = abel_make_object_ptr_from_bool(test_data);
    /* manually ref count set for memory freer */
    ptr_test_object->ref_count = 1;
    assert(*(Bool*)ptr_test_object->ptr_data == false);
    assert( ptr_test_object->data_size == sizeof(test_data) );
    assert(ptr_test_object->data_type == BOOL_TYPE);
    assert(ptr_test_object->ref_count == 1);

    abel_free_object_ptr(ptr_test_object);
}

void test_make_object_ptr_from_string()
{
    char* test_data = "Halo, Wereld!";
    struct abel_object* ptr_test_object = abel_make_object_ptr_from_string(test_data);
    /* manually ref count set for memory freer */
    ptr_test_object->ref_count = 1;
    assert(strcmp((char*)ptr_test_object->ptr_data, test_data) == 0);
    assert(ptr_test_object->data_type == STRING_TYPE);
    assert(ptr_test_object->ref_count == 1);

    abel_free_object_ptr(ptr_test_object);
}

void test_make_object_ptr_from_int()
{
    int test_data = 1989;
    struct abel_object* ptr_test_object = abel_make_object_ptr_from_int(test_data);
    /* manually ref count set for memory freer */
    ptr_test_object->ref_count = 1;
    assert(*(int*)ptr_test_object->ptr_data == 1989);
    assert( ptr_test_object->data_size == sizeof(test_data) );
    assert(ptr_test_object->data_type == INTEGER_TYPE);
    assert(ptr_test_object->ref_count == 1);

    abel_free_object_ptr(ptr_test_object);
}

void test_make_object_ptr_from_double()
{
    int test_data = 1989;
    struct abel_object* ptr_test_object = abel_make_object_ptr_from_int(test_data);
    /* manually ref count set for memory freer */
    ptr_test_object->ref_count = 1;
    assert(*(int*)ptr_test_object->ptr_data == 1989);    // type casting
    assert( ptr_test_object->data_size == sizeof(int) );
    assert(ptr_test_object->data_type == INTEGER_TYPE);
    assert(ptr_test_object->ref_count == 1);

    abel_free_object_ptr(ptr_test_object);
}

void test_make_object_ptr_generic_from()
{
    /* int */
    int test_data_int = 1989;
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data_int);
    /* manually ref count set for memory freer */
    ptr_test_object->ref_count = 1;
    assert(*(int*)ptr_test_object->ptr_data == 1989);    // type casting
    assert( ptr_test_object->data_size == sizeof(int) );
    assert(ptr_test_object->data_type == INTEGER_TYPE);
    assert(ptr_test_object->ref_count == 1);

    abel_free_object_ptr(ptr_test_object);
    
    /* double */
    double test_data_double = 4.07e-8;
    struct abel_object* ptr_test_object_2 = abel_make_object_ptr(test_data_double);
    ptr_test_object_2->ref_count = 1;    // manual ref count update
    assert(*(double*)ptr_test_object_2->ptr_data == 4.07e-8);    // type casting
    assert( ptr_test_object_2->data_size == sizeof(double) );
    assert(ptr_test_object_2->data_type == DOUBLE_TYPE);
    abel_free_object_ptr(ptr_test_object_2);
}

/* data getters */

void test_get_bool()
{
    Bool test_data = true;
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data);
    ptr_test_object->ref_count = 1;    // manual ref count update
    assert( ptr_test_object->data_size == sizeof(Bool) );
    assert(ptr_test_object->data_type == BOOL_TYPE);
    assert(abel_object_get_bool(ptr_test_object) == test_data);
    abel_free_object_ptr(ptr_test_object);
}

void test_get_string()
{
    char* test_data = "1989";
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data);
    ptr_test_object->ref_count = 1;    // manual ref count update
    assert(ptr_test_object->data_type == STRING_TYPE);
    assert(strcmp(abel_object_get_string(ptr_test_object), test_data) == 0);
    abel_free_object_ptr(ptr_test_object);
}

void test_get_int()
{
    int test_data = 1989;
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data);
    ptr_test_object->ref_count = 1;    // manual ref count update
    assert( ptr_test_object->data_size == sizeof(int) );
    assert(ptr_test_object->data_type == INTEGER_TYPE);
    assert(abel_object_get_int(ptr_test_object) == test_data);
    abel_free_object_ptr(ptr_test_object);
}

void test_get_double()
{
    double test_data = 1.7e-7;
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data);
    ptr_test_object->ref_count = 1;    // manual ref count update
    assert( ptr_test_object->data_size == sizeof(double) );
    assert(ptr_test_object->data_type == DOUBLE_TYPE);
    assert(abel_object_get_double(ptr_test_object) == test_data);
    abel_free_object_ptr(ptr_test_object);
}

void test_object_get_data()
/* Generic data getter */
{
    int test_data = 1989;
    struct abel_object* ptr_test_object = abel_make_object_ptr_from_int(test_data);
    ptr_test_object->ref_count = 1;    // manual ref count update
    assert(abel_object_get_data(ptr_test_object, test_data) == 1989);

    double test_data_double = 50.021;
    struct abel_object* ptr_test_object_2 = abel_make_object_ptr_from_double(test_data_double);
    ptr_test_object_2->ref_count = 1;    // manual ref count update
    assert(abel_object_get_data(ptr_test_object_2, test_data_double) == test_data_double);

    abel_free_object_ptr(ptr_test_object);
    abel_free_object_ptr(ptr_test_object_2);
}

/* type and type string getter */
void test_get_type()
{
    //Object test_object = {.ptr_data = NULL, .data_size = 0, .data_type= BOOL_TYPE};
    enum data_type test_value;
    //test_value = abel_object_get_type(test_object);    /* generic */
    //assert(test_value == BOOL_TYPE);

    double test_data = 19.89;
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data);
    ptr_test_object->ref_count = 1;
    test_value = abel_object_get_type(ptr_test_object);    // generic
    assert(test_value == DOUBLE_TYPE);
    abel_free_object_ptr(ptr_test_object);
}

void test_get_type_string()
{
    struct abel_object test_object = {.ptr_data = NULL, .data_size = 0, .data_type= BOOL_TYPE};
    char* test_value = abel_object_get_type_string(&test_object); /* from obj */
    /* TODO need a assert function for strings */
    assert(strcmp(test_value, "Bool") == 0);

    test_object.data_type = DOUBLE_TYPE;
    test_value = abel_object_get_type_string(&test_object);    // from obj
    assert(strcmp(test_value, "Double") == 0);

    double test_data = 19.89;
    struct abel_object* ptr_test_object = abel_make_object_ptr(test_data);
    ptr_test_object->ref_count = 1;
    test_value = abel_object_get_type_string(ptr_test_object);    // from ptr
    assert(strcmp(test_value, "Double") == 0);
    abel_free_object_ptr(ptr_test_object);
}

int main()
{
/* new pointer from primitive and intrinsic types */
    test_make_object_ptr_from_bool();
    test_make_object_ptr_from_string();
    test_make_object_ptr_from_int();
    test_make_object_ptr_from_double();
    test_make_object_ptr_generic_from();

/* data getters */
    test_get_bool();
    test_get_string();
    test_get_int();
    test_object_get_data();    // generic getter

/* type and type string getters */
    test_get_type();    // generic
    test_get_type_string();    // generic
}