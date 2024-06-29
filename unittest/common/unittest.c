/**
 * @file Unitetst common 
 * 
 * Common header consists of all modules that can be
 * used as an independent library. It consits of Abel
 * string and three types of containers.
 * 
 * This unittest focuses on Abel strings in containers
 * such as vector and map. 
 **/
#include <assert.h>
#include "common.h"

void test_strings_in_vector()
{
    struct abel_string* ptr_str_0 = abel_make_string_ptr("Hello");
    struct abel_vector test_vector = abel_make_vector(0);
    assert(abel_vector_size(&test_vector) == 0);
    /* Append a string pointer to vector */
    abel_vector_append(&test_vector, ptr_str_0);
    assert(abel_vector_size(&test_vector) == 1);
    /* Append another string to the vector */
    struct abel_string* ptr_str_1 = abel_make_string_ptr("World");
    abel_vector_append(&test_vector, ptr_str_1);
    assert(abel_vector_size(&test_vector) == 2);
    
    /* Read the stored strings */
    struct abel_string* ptr_str_acquired_0 = abel_vector_at(&test_vector, 0).pointer;
    assert(abel_string_eq_cstring(ptr_str_acquired_0, "Hello") == true);

    struct abel_string* ptr_str_acquired_1 = abel_vector_at(&test_vector, 1).pointer;
    assert(abel_string_eq_cstring(ptr_str_acquired_1, "World") == true);

    /* Replace the second string */
    struct abel_string* ptr_str_2 = abel_make_string_ptr("You");
    struct abel_string* ptr_old = abel_vector_emplace(&test_vector, 1, ptr_str_2).pointer;
    /* Must free the previous content */
    abel_free_string_ptr(ptr_old);
    /* check the new content */
    ptr_str_acquired_1 = abel_vector_at(&test_vector, 1).pointer;
    assert(abel_string_eq_cstring(ptr_str_acquired_1, "You") == true);

    /* Clean up */
    abel_free_string_ptr(ptr_str_acquired_0);
    abel_free_string_ptr(ptr_str_acquired_1);
    abel_free_vector(&test_vector);
}

struct sample {
    struct abel_vector vector_field;
};

void make_sample(struct sample* ptr_sample)
{
    ptr_sample->vector_field = abel_make_vector(0);
}

void test_vector_in_struct()
{
    struct sample test_struct;
    make_sample(&test_struct);

    char root_key[] = "ROOT_KEY_";
    struct abel_string* ptr_pk = abel_make_string_ptr(root_key);
    abel_vector_append(&test_struct.vector_field, ptr_pk);
    assert(abel_vector_size(&test_struct.vector_field) == 1);
    abel_free_string_ptr(ptr_pk);    // use free_string_ptr
    abel_free_vector(&test_struct.vector_field);
}

int main()
{
    test_strings_in_vector();
    test_vector_in_struct();
}