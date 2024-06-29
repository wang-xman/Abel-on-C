/* Unittest list */
#include <assert.h>
#include "list.h"

/* constructors, create, and new pointer */

void test_make_list()
{
    struct abel_list test_list = abel_make_list(100);
    assert(test_list.ptr_vector->size == 100);
    assert(test_list.ptr_vector->capacity == 128);
    /* free the internal vector */
    abel_free_vector_ptr(test_list.ptr_vector);
}

void test_make_list_ptr()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(100);
    assert(ptr_test_list->ptr_vector->size == 100);
    assert(ptr_test_list->ptr_vector->capacity == 128);
    /* free both internal vector and list pointer */
    //abel_free_list(ptr_test_list);
    abel_free_vector_ptr(ptr_test_list->ptr_vector);
    free(ptr_test_list);
}

void test_make_list_ptr_zero_size()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    assert(ptr_test_list->ptr_vector->size == 0);
    assert(ptr_test_list->ptr_vector->capacity == 2);
    // If zero size, the capacity is still 2,
    // the pointer still must be freed.
    abel_free_vector_ptr(ptr_test_list->ptr_vector);
    free(ptr_test_list);
}

/* size, capacity getter, and validators */

void test_abel_list_size_and_capacity()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(100);
    assert(abel_list_size(ptr_test_list) == 100);
    assert(abel_list_capacity(ptr_test_list) == 128);
    assert(abel_list_is_empty(ptr_test_list) == false);
    assert(abel_list_is_valid_index(ptr_test_list, 3) == true);
    abel_free_list_ptr(ptr_test_list);
}

/* append */

void test_list_append_bool()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append_bool(ptr_test_list, false);
    assert(abel_list_size(ptr_test_list) == 1);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]))->ref_count == 1 );
    /* get via vector */
    assert( ((struct abel_object*)(abel_vector_get(ptr_test_list->ptr_vector, 0).pointer))->ref_count == 1 );
    abel_free_list_ptr(ptr_test_list);
}

void test_list_append_string()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    char* test_data = "Halo, Wereld!!";
    abel_list_append_string(ptr_test_list, test_data);
    assert(abel_list_size(ptr_test_list) == 1);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);

    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]))->ref_count == 1 );

    abel_free_list_ptr(ptr_test_list);
}

void test_list_append_int()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append_bool(ptr_test_list, 100);
    assert(abel_list_size(ptr_test_list) == 1);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]))->ref_count == 1 );

    abel_free_list_ptr(ptr_test_list);
}

void test_list_append_double()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append_bool(ptr_test_list, 3.96);
    assert(abel_list_size(ptr_test_list) == 1);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]))->ref_count == 1 );

    abel_free_list_ptr(ptr_test_list);
}

void test_list_append()    /* generic append */
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, 456);    /* int */
    abel_list_append(ptr_test_list, 0.87);    /* double */
    assert(abel_list_size(ptr_test_list) == 2);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);
    /* check ref count of elements */
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]))->ref_count == 1);
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[1]))->ref_count == 1);

    abel_list_append(ptr_test_list, 92);    /* int */
    assert(abel_list_size(ptr_test_list) == 3);
    assert(abel_list_capacity(ptr_test_list) == 4);    /* capacity doubled */
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[2]))->ref_count == 1);

    abel_list_append(ptr_test_list, false);    /* Bool */
    assert(abel_list_size(ptr_test_list) == 4);
    assert(abel_list_capacity(ptr_test_list) == 4);
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[3]))->ref_count == 1);

    abel_list_append(ptr_test_list, true);    /* Bool */
    assert(abel_list_size(ptr_test_list) == 5);
    assert(abel_list_capacity(ptr_test_list) == 8);    /* capacity doubled */
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[4]))->ref_count == 1);

    struct abel_list* ptr_sub_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, ptr_sub_list);    /* list ptr */
    assert(abel_list_size(ptr_test_list) == 6);
    assert(abel_list_capacity(ptr_test_list) == 8);
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[5]))->ref_count == 1);
    /* sub list should be empty */
    struct abel_object* ptr_object = (struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[5]);
    /* get the sublist */
    struct abel_list* ptr_list_acquired = (struct abel_list*)ptr_object->ptr_data;
    assert(abel_list_size(ptr_list_acquired) == 0);
    
    /* Recursively free */
    abel_free_list_ptr(ptr_test_list);
}


/* Emplace and set */

void test_list_set()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(2);

    /*
        First, set a bool at index 0.
        Mind the the issue with direct setting a rvalue
        of Bool. So, use forced type conversion, or note
        the type.
    */
    abel_list_set(ptr_test_list, 0, (Bool)false);    /* bool */
    assert(abel_list_get_data_type(ptr_test_list, 0) == BOOL_TYPE);
    /*
        Now I replace that Bool by an integer
    */
    abel_list_set(ptr_test_list, 0, 777);    /* int */
    assert(abel_list_get_data_type(ptr_test_list, 0) == INTEGER_TYPE);
    assert(abel_list_size(ptr_test_list) == 2);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);
    /*
        Let me check if the integer has settled down.
    */
    assert(abel_list_get_int(ptr_test_list, 0) == 777);

    /*
        Since there are two seats, I can use the 1st seat.
    */
    abel_list_set(ptr_test_list, 1, 3.14);    /* double */
    assert(abel_list_get_data_type(ptr_test_list, 1) == DOUBLE_TYPE);
    assert(abel_list_get_int(ptr_test_list, 0) == 777);
    assert(abel_list_get_double(ptr_test_list, 1) == 3.14);

    /*
        Again, replace the 1st by something else
    */
    abel_list_set(ptr_test_list, 1, false);    /* bool */
    assert(abel_list_get_bool(ptr_test_list, 1) == false);

    abel_free_list_ptr(ptr_test_list);
}

/* key method : data getter */

void test_abel_list_get_object_pointer()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, 456);    /* int */
    abel_list_append(ptr_test_list, 0.87);    /* double */
    abel_list_append(ptr_test_list, 92);    /* int */

    struct abel_object* objptr = abel_list_get_object_pointer(ptr_test_list, 0);
    int data_0 = abel_object_get_data(objptr, 1);
    assert(data_0 == 456);

    objptr = abel_list_get_object_pointer(ptr_test_list, 1);
    double data_1 = abel_object_get_data(objptr, 0.1);
    assert(data_1 == 0.87);

    objptr = abel_list_get_object_pointer(ptr_test_list, 2);
    int data_2 = abel_object_get_data(objptr, 92);
    assert(data_2 == 92);

    abel_free_list_ptr(ptr_test_list);
}

void test_abel_list_get_data_int()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, 456);    /* int */
    abel_list_append(ptr_test_list, 0.87);    /* double */
    abel_list_append(ptr_test_list, 92);    /* int */

    assert(abel_list_get_int(ptr_test_list, 0) == 456);

    abel_free_list_ptr(ptr_test_list);
}

void test_abel_list_get_data_double()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, 456);    /* int */
    abel_list_append(ptr_test_list, 0.87);    /* double */
    abel_list_append(ptr_test_list, 92);    /* int */

    assert(abel_list_get_double(ptr_test_list, 1) == 0.87);

    abel_free_list_ptr(ptr_test_list);
}

void test_abel_list_get()
{
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, 456);    /* int */
    abel_list_append(ptr_test_list, 0.87);    /* double */
    abel_list_append(ptr_test_list, 92);    /* int */

    assert(abel_list_get(ptr_test_list, 0, (int)1) == 456);
    assert(abel_list_get(ptr_test_list, 1, (double)0.1) == 0.87);
    assert(abel_list_get(ptr_test_list, 2, (int)0) == 92);

    /*
        Another way of using getter is to store it in
        a variable of desired type and use that variable
        as the flag
    */
    double value = abel_list_get(ptr_test_list, 1, value);
    assert(value == 0.87);

    abel_free_list_ptr(ptr_test_list);
}

/* Delete an item from list */

void test_list_delete()
{
    int test_data = 77;
    double test_data_1 = 3.14;
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, test_data); /* append object */
    abel_list_append(ptr_test_list, test_data_1);

    assert(abel_list_size(ptr_test_list) == 2);
    assert(abel_list_capacity(ptr_test_list) == 2);
    assert(abel_list_is_empty(ptr_test_list) == false);

    /* check object ref count */
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]))->ref_count == 1 );
    assert( ((struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[1]))->ref_count == 1 );

    /* delete the 0th item in the list */
    struct abel_return_option ret = abel_list_delete(ptr_test_list, 0);
    //assert(ret.okay == OKAY);
    assert(abel_list_size(ptr_test_list) == 1);
    assert(abel_list_capacity(ptr_test_list) == 2);
    struct abel_object* ptr_object_acquired = (struct abel_object*)(ptr_test_list->ptr_vector->ptr_array[0]);
    /*
        The previous 1st element now becomes the 0th. 
     */
    assert(*(double*)ptr_object_acquired->ptr_data == test_data_1);

    /* Further delete the only item in the list */
    ret = abel_list_delete(ptr_test_list, 0);
    //assert(ret.okay == OKAY);
    assert(abel_list_size(ptr_test_list) == 0);
    assert(abel_list_capacity(ptr_test_list) == 2);

    /* Free them */
    abel_free_list_ptr(ptr_test_list);
}


int main()
{
/* constructors, create, and new pointer */
    test_make_list();
    test_make_list_ptr();
    test_make_list_ptr_zero_size();

/* size and capacity */
    test_abel_list_size_and_capacity();

/* key method : append */
    test_list_append_bool();
    test_list_append_string();
    test_list_append_int();
    test_list_append_double();
    test_list_append();

/* key method : setter */
    test_list_set();

/* key method : data getter */
    test_abel_list_get_object_pointer();
    test_abel_list_get_data_int();
    test_abel_list_get_data_double();
    test_abel_list_get();

/* key method : list delete */
    test_list_delete();
}