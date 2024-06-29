/* Unittest vector */
#include <assert.h>
#include "vector.h"

void test_maker_with_zero_size()
{
    struct abel_vector test_vector = abel_make_vector(0);
    assert(test_vector.capacity == 2);
    assert(abel_vector_size(&test_vector) == 0);
    free(test_vector.ptr_array);
}

void test_maker_with_size_1()
{
    struct abel_vector test_vector = abel_make_vector(1);
    assert(abel_vector_size(&test_vector) == 1);
    assert(test_vector.capacity == 2);
    assert(test_vector.ptr_array[0] == NULL);
    free(test_vector.ptr_array);
}

void test_maker_with_size_many()
{
    struct abel_vector test_vector = abel_make_vector(5);
    assert(test_vector.size == 5);
    assert(test_vector.capacity == 8);
    assert(test_vector.ptr_array[0] == NULL);
    assert(test_vector.ptr_array[2] == NULL);
    assert(test_vector.ptr_array[3] == NULL);
    assert(test_vector.ptr_array[4] == NULL);
    free(test_vector.ptr_array);
}

void test_vector_pointer_maker()
{
    struct abel_vector* ptr_test_vector = abel_make_vector_ptr(5);
    assert(ptr_test_vector->size == 5);
    assert(ptr_test_vector->capacity == 8);
    assert(ptr_test_vector->ptr_array[0] == NULL);
    assert(ptr_test_vector->ptr_array[2] == NULL);
    assert(ptr_test_vector->ptr_array[3] == NULL);
    assert(ptr_test_vector->ptr_array[4] == NULL);
    free(ptr_test_vector->ptr_array);
    free(ptr_test_vector);
}

void test_vector_append()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    assert(abel_vector_size(&test_vector) == 4);
    assert(test_vector.capacity == 4);

    int integer = 100;
    ret = abel_vector_append(&test_vector, &integer);
    assert(ret.is_okay == true);
    assert(abel_vector_size(&test_vector) == 5);
    assert(test_vector.capacity == 8);

    abel_vector_append(&test_vector, &integer);
    assert(abel_vector_size(&test_vector) == 6);
    assert(test_vector.capacity == 8);

    abel_vector_append(&test_vector, &integer);
    assert(abel_vector_size(&test_vector) == 7);
    assert(test_vector.capacity == 8);

    abel_vector_append(&test_vector, &integer);
    assert(abel_vector_size(&test_vector) == 8);
    assert(test_vector.capacity == 8);

    abel_vector_append(&test_vector, &integer);
    assert(abel_vector_size(&test_vector) == 9);
    assert(test_vector.capacity == 16);

    free(test_vector.ptr_array);
}

/* Place and replace */

void test_vector_set()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    assert(abel_vector_size(&test_vector) == 4);
    assert(test_vector.capacity == 4);

    int integer = 100;
    ret = abel_vector_set(&test_vector, 0, &integer);
    assert(ret.is_okay == true);
    /*
        Previous value is NULL
    */
    assert(ret.pointer == NULL);
    assert(abel_vector_size(&test_vector) == 4);
    assert(test_vector.capacity == 4);
    ret = abel_vector_get(&test_vector, 0);
    assert( *(int*)ret.pointer == 100);

    /*
        Now, set to replace at index 0.
    */
    double dbl = 0.26;
    ret = abel_vector_set(&test_vector, 0, &dbl);
    /*
        The previous occupant is retuned.
    */
    assert(*(int*)ret.pointer == 100);
    ret = abel_vector_get(&test_vector, 0);
    assert( *(double*)ret.pointer == 0.26);

    /*
        Setting at an index that is equal or greater than
        the size results in error
    */
    ret = abel_vector_set(&test_vector, 4, &dbl);
    assert(ret.is_error == true);
    assert(ret.error.error_type == OUT_OF_RANGE);
    ret = abel_vector_set(&test_vector, 8, &dbl);
    assert(ret.is_error == true);
    assert(ret.error.error_type == OUT_OF_RANGE);

    /*
        Of course, setting on empty vector results in error, too.
    */
    struct abel_vector empty_vector = abel_make_vector(0);
    ret = abel_vector_set(&empty_vector, 0, &integer);
    assert(ret.is_error == true);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(empty_vector.ptr_array);
    free(test_vector.ptr_array);
}

/* vector getters: at, get, back, begin */

void test_vector_at()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    
    /*
        At index 1, lies a double
    */
    ret = abel_vector_at(&test_vector, 1);
    assert(*(double*)ret.pointer == 7.12);
    /*
        At index 0, lies an integer
    */
    ret = abel_vector_at(&test_vector, 0);
    assert(*(int*)ret.pointer == 456);
    /*
        At index 2, lies a string
    */
    ret = abel_vector_at(&test_vector, 2);
    assert(strcmp((char*)ret.pointer, "Ola") == 0);
    /*
        Attempting index 5 results in error
    */
    ret = abel_vector_at(&test_vector, 5);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(test_vector.ptr_array);
}

void test_vector_back()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    
    /*
        Last element is a double type
    */
    ret = abel_vector_back(&test_vector);
    assert(*(double*)ret.pointer == 0.04);

    /*
        Empty vector
    */
    struct abel_vector empty_vector = abel_make_vector(0);
    ret = abel_vector_back(&empty_vector);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(empty_vector.ptr_array);
    free(test_vector.ptr_array);
}

void test_vector_front()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    
    /*
        First element is a double type
    */
    ret = abel_vector_front(&test_vector);
    assert(*(int*)ret.pointer == 456);

    /*
        Empty vector
    */
    struct abel_vector empty_vector = abel_make_vector(0);
    ret = abel_vector_front(&empty_vector);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(empty_vector.ptr_array);
    free(test_vector.ptr_array);
}

/* vector insert */

void test_vector_insert_minimal_capacity()
{
    //struct abel_return_option ret;
    /*
        Make an empty vector
    */
    struct abel_vector test_vector = abel_make_vector(0);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);

    int el0 = 456;
    double el1 = 7.12;
    double el2 = 3.14;
    /*
        Insert at 0 into an empty vector is okay.
    */
    abel_vector_insert(&test_vector, 0, &el0);
    assert(test_vector.size == 1);
    assert(test_vector.capacity == 2);
    /*
        Insert at 1 shall be identical to append.
    */
    abel_vector_insert(&test_vector, 1, &el1);
    assert(test_vector.size == 2);
    assert(test_vector.capacity == 2);
    /*
        Insert again at 1 shall increase the size and capacity
        of the vector.
    */
    abel_vector_insert(&test_vector, 1, &el2);
    assert(test_vector.size == 3);
    assert(test_vector.capacity == 4);

    /*
        Now the vector is [456, 3.14, 7.12]
    */
    assert(*(int*)abel_vector_at(&test_vector, 0).pointer == 456);
    assert(*(double*)abel_vector_at(&test_vector, 1).pointer == 3.14);
    assert(*(double*)abel_vector_at(&test_vector, 2).pointer == 7.12);

    free(test_vector.ptr_array);
}

void test_vector_insert_mid()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    /*
        sanity check. now the vector is [456, 7.12, "Ola", 0.04]
    */
    ret = abel_vector_get(&test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);
    
    /*
        Now, I want to insert the following at index 1 to have
        [456, 101, 7.12, "Ola", 0.04]
    */
    int el_new = 101;
    ret = abel_vector_insert(&test_vector, 1, &el_new);
    assert(test_vector.size == 5);
    assert(test_vector.capacity == 8);
    /*
        Let's see what we have here.
    */
    ret = abel_vector_get(&test_vector, 0);
    assert( *(int*)ret.pointer == 456);
    ret = abel_vector_get(&test_vector, 1);    // newly added
    assert( *(int*)ret.pointer == 101);
    ret = abel_vector_get(&test_vector, 2);
    assert( *(double*)ret.pointer == 7.12);
    ret = abel_vector_get(&test_vector, 3);
    assert( strcmp((char*)ret.pointer, "Ola") == 0);
    ret = abel_vector_get(&test_vector, 4);
    assert( *(double*)ret.pointer == 0.04);

    free(test_vector.ptr_array);
}

void test_vector_insert_head()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    /*
        sanity check. now the vector is [456, 7.12, "Ola", 0.04]
    */
    ret = abel_vector_get(&test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);
    
    /*
        Now, I want to insert the following at index 0 to have
        [101, 456, 7.12, "Ola", 0.04]
    */
    int el_new = 101;
    ret = abel_vector_insert(&test_vector, 0, &el_new);
    assert(test_vector.size == 5);
    assert(test_vector.capacity == 8);
    /*
        Let's see what we have here.
    */
   ret = abel_vector_get(&test_vector, 0);    // newly added
    assert( *(int*)ret.pointer == 101);
    ret = abel_vector_get(&test_vector, 1);
    assert( *(int*)ret.pointer == 456);
    ret = abel_vector_get(&test_vector, 2);
    assert( *(double*)ret.pointer == 7.12);
    ret = abel_vector_get(&test_vector, 3);
    assert( strcmp((char*)ret.pointer, "Ola") == 0);
    ret = abel_vector_get(&test_vector, 4);
    assert( *(double*)ret.pointer == 0.04);

    free(test_vector.ptr_array);
}

void test_vector_insert_tail()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    /*
        sanity check. now the vector is [456, 7.12, "Ola", 0.04]
    */
    ret = abel_vector_get(&test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);
    
    /*
        Now, I want to insert the following at index 4 to have
        [456, 7.12, "Ola", 0.04, 101]
    */
    int el_new = 101;
    ret = abel_vector_insert(&test_vector, 4, &el_new);
    assert(test_vector.size == 5);
    assert(test_vector.capacity == 8);
    /*
        Let's see what we have here.
    */
    ret = abel_vector_get(&test_vector, 0);
    assert( *(int*)ret.pointer == 456);
    ret = abel_vector_get(&test_vector, 1);
    assert( *(double*)ret.pointer == 7.12);
    ret = abel_vector_get(&test_vector, 2);
    assert( strcmp((char*)ret.pointer, "Ola") == 0);
    ret = abel_vector_get(&test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);
    ret = abel_vector_get(&test_vector, 4);    // newly added
    assert( *(int*)ret.pointer == 101);

    free(test_vector.ptr_array);
}

void test_vector_insert_failure()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    /*
        sanity check. now the vector is [456, 7.12, "Ola", 0.04]
    */
    ret = abel_vector_get(&test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);
    
    /*
        Now, I want to insert the following at index 5.
        This triggers error as 5 is out of range.
    */
    int el_new = 101;
    ret = abel_vector_insert(&test_vector, 5, &el_new);
    assert(ret.error.error_type == OUT_OF_RANGE);
    /*
        Let's see what we have here.
        The vector remains unchanged.
    */
    ret = abel_vector_get(&test_vector, 0);
    assert( *(int*)ret.pointer == 456);
    ret = abel_vector_get(&test_vector, 1);
    assert( *(double*)ret.pointer == 7.12);
    ret = abel_vector_get(&test_vector, 2);
    assert( strcmp((char*)ret.pointer, "Ola") == 0);
    ret = abel_vector_get(&test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);

    free(test_vector.ptr_array);
}

void test_vector_insert_into_empty()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(0);
    int el0 = 456;
    /*
        Now, I want to insert the following at index 0 to have
        [456]
    */
    ret = abel_vector_insert(&test_vector, 0, &el0);
    assert(test_vector.size == 1);
    assert(test_vector.capacity == 2);
    /*
        Let's see what we have here.
        The vector remains unchanged.
    */
    ret = abel_vector_get(&test_vector, 0);
    assert( *(int*)ret.pointer == 456);

    free(test_vector.ptr_array);
}

/* vector pointer insert */

void test_vector_ptr_insert_mid()
{
    struct abel_return_option ret;
    struct abel_vector* ptr_test_vector = abel_make_vector_ptr(4);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    abel_vector_set(ptr_test_vector, 0, &el0);
    abel_vector_set(ptr_test_vector, 1, &el1);
    abel_vector_set(ptr_test_vector, 2, el2);    // Caution!
    abel_vector_set(ptr_test_vector, 3, &el3);
    /*
        sanity check. now the vector is [456, 7.12, "Ola", 0.04]
    */
    ret = abel_vector_get(ptr_test_vector, 3);
    assert( *(double*)ret.pointer == 0.04);
    
    /*
        Now, I want to insert the following at index 1 to have
        [456, 101, 7.12, "Ola", 0.04]
    */
    int el_new = 101;
    ret = abel_vector_insert(ptr_test_vector, 1, &el_new);
    assert(ptr_test_vector->size == 5);
    assert(ptr_test_vector->capacity == 8);
    /*
        Let's see what we have here.
    */
    ret = abel_vector_get(ptr_test_vector, 0);
    assert( *(int*)ret.pointer == 456);
    ret = abel_vector_get(ptr_test_vector, 1);    // newly added
    assert( *(int*)ret.pointer == 101);
    ret = abel_vector_get(ptr_test_vector, 2);
    assert( *(double*)ret.pointer == 7.12);
    ret = abel_vector_get(ptr_test_vector, 3);
    assert( strcmp((char*)ret.pointer, "Ola") == 0);
    ret = abel_vector_get(ptr_test_vector, 4);
    assert( *(double*)ret.pointer == 0.04);

    /*
        Now, must free the resource
    */
    abel_free_vector_ptr(ptr_test_vector);
}

/* delete, erase, pop */

void test_vector_delete_mid()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(5);
    int el0 = 456;
    double el1 = 7.12;
    char* el2 = "Ola";
    double el3 = 0.04;
    int el4 = 101;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    abel_vector_set(&test_vector, 2, el2);    // Caution!
    abel_vector_set(&test_vector, 3, &el3);
    abel_vector_set(&test_vector, 4, &el4);
    /*
        Sanity check.
    */
    assert(test_vector.size == 5);
    assert(test_vector.capacity == 8);

    /*
        Delete element at index 1, resulting in index upshifting.
        The previous occupant is 7.12
    */
    ret = abel_vector_delete(&test_vector, 1);
    /*
        The previous occupant 7.12 is returned
    */
    assert(*(double*)ret.pointer == 7.12);
    assert(test_vector.size == 4);
    assert(test_vector.capacity == 4);
    ret = abel_vector_get(&test_vector, 0);
    assert( *(int*)ret.pointer == 456);
    /*
        Indices have shifted
    */
    ret = abel_vector_get(&test_vector, 1);
    assert( strcmp((char*)ret.pointer, "Ola") == 0);
    ret = abel_vector_get(&test_vector, 2);
    assert( *(double*)ret.pointer == 0.04);
    ret = abel_vector_get(&test_vector, 3);
    assert( *(int*)ret.pointer == 101);
    /*
        Index 4 is no longer valid
    */
    ret = abel_vector_get(&test_vector, 4);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(test_vector.ptr_array);
}

void test_vector_delete_to_empty()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(1);
    int el0 = 456;
    abel_vector_set(&test_vector, 0, &el0);
    /*
        Sanity check.
    */
    assert(test_vector.size == 1);
    assert(test_vector.capacity == 2);

    /*
        Return and delete the only element
    */
    ret = abel_vector_delete(&test_vector, 0);
    assert(*(int*)ret.pointer == 456);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);
    assert(test_vector.ptr_array[0] == NULL);

    /*
        Delete from an empty vector results in error
    */
    ret = abel_vector_delete(&test_vector, 0);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(test_vector.ptr_array);
}

void test_vector_delete_front()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(2);
    int el0 = 456;
    double el1 = 3.14;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_set(&test_vector, 1, &el1);
    /*
        Sanity check.
    */
    assert(test_vector.size == 2);
    assert(test_vector.capacity == 2);

    /*
        Return and delete element at index 0, the next is moved up.
        Then at index 1, a NULL is placed.
    */
    ret = abel_vector_delete(&test_vector, 0);
    assert(*(int*)ret.pointer == 456);
    assert(test_vector.size == 1);
    assert(test_vector.capacity == 2);
    assert(test_vector.ptr_array[1] == NULL);
    /*
        The previous one at index 1 is moved up to index 0.
    */
    assert(*(double*)(test_vector.ptr_array[0]) == 3.14);

    /*
        Delete the last element to get an empty vector.
    */
    ret = abel_vector_delete(&test_vector, 0);
    assert(*(double*)ret.pointer == 3.14);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);
    assert(test_vector.ptr_array[0] == NULL);
    assert(test_vector.ptr_array[1] == NULL);

    /*
        Delete from an empty vector results in error
    */
    ret = abel_vector_delete(&test_vector, 0);
    assert(ret.error.error_type == OUT_OF_RANGE);

    free(test_vector.ptr_array);
}

void test_vector_pop_back()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(1);
    int el0 = 456;
    double el1 = 3.14;
    abel_vector_set(&test_vector, 0, &el0);
    abel_vector_push_back(&test_vector, &el1);
    /*
        Sanity check.
    */
    assert(test_vector.size == 2);
    assert(test_vector.capacity == 2);
    /*
        Pop back and returns the last element,
        then the vector has 1 element left.
    */
    ret = abel_vector_pop_back(&test_vector);
    assert(*(double*)ret.pointer == 3.14);
    assert(test_vector.size == 1);
    assert(test_vector.capacity == 2);
    assert(*(int*)(abel_vector_at(&test_vector, 0).pointer) == 456);
    /*
        Pop back again to return el0 and to have an empty vector.
    */
    ret = abel_vector_pop_back(&test_vector);
    assert(*(int*)ret.pointer == 456);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);
    /*
        Pop back from an empty vector results in error.
    */
    ret = abel_vector_pop_back(&test_vector);
    assert(ret.error.error_type == OUT_OF_RANGE);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);

    free(test_vector.ptr_array);
}

void test_vector_pop_front()
{
    struct abel_return_option ret;
    struct abel_vector test_vector = abel_make_vector(0);
    int el0 = 456;
    double el1 = 3.14;
    int el2 = 777;
    abel_vector_push_back(&test_vector, &el0);
    abel_vector_push_back(&test_vector, &el1);
    abel_vector_push_back(&test_vector, &el2);
    /*
        Sanity check.Now vector is [456, 3.14, 777]
    */
    assert(test_vector.size == 3);
    assert(test_vector.capacity == 4);
    /*
        Pop front, returns pointer to 456,
        and then the vector becomes [3.14, 777]
    */
    ret = abel_vector_pop_front(&test_vector);
    assert(*(int*)ret.pointer == 456);
    assert(test_vector.size == 2);
    assert(test_vector.capacity == 2);
    assert(*(double*)(abel_vector_at(&test_vector, 0).pointer) == 3.14);
    assert(*(int*)(abel_vector_at(&test_vector, 1).pointer) == 777);
    /*
        Pop front again to return 3.14, vector becomes [777]
    */
    ret = abel_vector_pop_front(&test_vector);
    assert(*(double*)ret.pointer == 3.14);
    assert(test_vector.size == 1);
    assert(test_vector.capacity == 2);
    /*
        Pop front to return 777 and to have an empty vector.
    */
    ret = abel_vector_pop_back(&test_vector);
    assert(*(int*)ret.pointer == 777);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);
    /*
        Pop front from an empty vector results in error
    */
    ret = abel_vector_pop_back(&test_vector);
    assert(ret.error.error_type == OUT_OF_RANGE);
    assert(test_vector.size == 0);
    assert(test_vector.capacity == 2);

    free(test_vector.ptr_array);
}

int main()
{
/* constructor */
    test_maker_with_zero_size();
    test_maker_with_size_1();
    test_maker_with_size_many();

/* pointer constructor */
    test_vector_pointer_maker();

/* vector append */
    test_vector_append();

/* vector set */
    test_vector_set();

/* vector getters : at, get, back, front */
    test_vector_at();
    test_vector_back();
    test_vector_front();

/* vector insert */
    test_vector_insert_minimal_capacity();    // edgy case
    test_vector_insert_mid();
    test_vector_insert_head();
    test_vector_insert_tail();
    test_vector_insert_failure();
    test_vector_insert_into_empty();

/* vector insert */
    test_vector_ptr_insert_mid();

/* vector delete */
    test_vector_delete_mid();
    test_vector_delete_to_empty();
    test_vector_delete_front();
    test_vector_pop_back();
    test_vector_pop_front();
}