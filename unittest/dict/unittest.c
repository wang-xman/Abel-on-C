/* Unittest dict */
#include <assert.h>
#include "dict.h"

void test_dict_insert()
{
    char* key = "Abcd";
    int value = 1000;
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value);

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert(ptr_test_dict, key, ptr_object);    // ("Abcd":1000)
    abel_dict_insert(ptr_test_dict, "ABcd", ptr_object_2);    // ("ABcd":1000)

    struct abel_object* acquired_value
            = abel_dict_get_object_ptr(ptr_test_dict, key);
    assert(*(int*)(acquired_value->ptr_data) == 1000);
    assert(*(int*)(abel_dict_get_object_ptr(ptr_test_dict, "ABcd")->ptr_data) == value);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_insert_bool()
{
    char* key = "Abcd";
    Bool value = true;

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert_bool_ptr(ptr_test_dict, key, &value);    // ("Abcd":true)
    abel_dict_insert_bool(ptr_test_dict, "ABcd", value);    // ("ABcd":true)

    struct abel_object* acquired_value
            = abel_dict_get_object_ptr(ptr_test_dict, key);
    assert(*(Bool*)(acquired_value->ptr_data) == true);
    assert(*(Bool*)(abel_dict_get_object_ptr(ptr_test_dict, "ABcd")->ptr_data) == value);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_insert_null()
{
    char* key = "Abcd";
    Null value = null;

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert_null_ptr(ptr_test_dict, key, &value);    // ("Abcd":null)
    abel_dict_insert_null(ptr_test_dict, "ABcd", value);    // ("ABcd":null)

    struct abel_object* acquired_value
            = abel_dict_get_object_ptr(ptr_test_dict, key);
    assert(*(Null*)(acquired_value->ptr_data) == null);
    assert(*(Null*)(abel_dict_get_object_ptr(ptr_test_dict, "ABcd")->ptr_data) == value);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_insert_string()
{
    char* key = "Abcd";
    char* value = "World";

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert_string(ptr_test_dict, key, value);    // ("Abcd":"World")

    struct abel_object* acquired_value = abel_dict_get_object_ptr(ptr_test_dict, key);
    assert(strcmp((char*)(char*)(acquired_value->ptr_data), value) == 0);
    assert(strcmp((char*)(char*)(abel_dict_get_object_ptr(ptr_test_dict, "Abcd")->ptr_data), value) == 0);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_insert_double()
{
    char* key = "Abcd";
    double value = 0.19;

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert_double_ptr(ptr_test_dict, key, &value);    // ("Abcd":0.19)
    abel_dict_insert_double(ptr_test_dict, "ABcd", value);    // ("ABcd":0.19)

    struct abel_object* acquired_value = abel_dict_get_object_ptr(ptr_test_dict, key);
    assert(*(double*)(acquired_value->ptr_data) == value);
    assert(*(double*)(abel_dict_get_object_ptr(ptr_test_dict, "ABcd")->ptr_data) == 0.19);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_insert_dict_ptr()
{
    char* key = "Abcd";
    struct abel_dict* ptr_dict = abel_make_dict_ptr();

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    // insert dict into the test dict
    abel_dict_insert_dict_ptr(ptr_test_dict, key, ptr_dict);    // ("Abcd": dict)

    struct abel_object* acquired_value
        = abel_dict_get_object_ptr(ptr_test_dict, key);    // get it
    assert(acquired_value->data_type == DICT_TYPE);    // check its type
    abel_free_dict_ptr(ptr_test_dict);
}

/* getters */

void test_dict_get_bool()
{
    char* key = "Abcd";
    Bool value = false;
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value);

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert(ptr_test_dict, key, ptr_object);
    abel_dict_insert(ptr_test_dict, "ABcd", ptr_object_2);
    assert(abel_dict_get_bool(ptr_test_dict, key) == false);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_get()
{
    char* key = "Abcd";
    int value = 1000;
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value);

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert(ptr_test_dict, key, ptr_object);
    abel_dict_insert(ptr_test_dict, "ABcd", ptr_object_2);

    struct abel_object* acquired_value
            = abel_dict_get_object_ptr(ptr_test_dict, key);
    assert(*(int*)(acquired_value->ptr_data) == value);
    assert(*(int*)(abel_dict_get_object_ptr(ptr_test_dict, "ABcd")->ptr_data) == 1000);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_get_int()
{
    char* key = "Abcd";
    int value = 1000;
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value);

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert(ptr_test_dict, key, ptr_object);
    abel_dict_insert(ptr_test_dict, "ABcd", ptr_object_2);

    assert(abel_dict_get_int(ptr_test_dict, key) == value);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_get_double()
{
    char* key = "Abcd";
    double value = 0.07;
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value);

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert(ptr_test_dict, key, ptr_object);
    abel_dict_insert(ptr_test_dict, "ABcd", ptr_object_2);

    assert(abel_dict_get_double(ptr_test_dict, key) == value);

    abel_free_dict_ptr(ptr_test_dict);
}

void test_dict_get_string()
{
    char* key = "Abcd";
    char* value = "X-Wing";
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value);

    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    abel_dict_insert(ptr_test_dict, key, ptr_object);
    abel_dict_insert(ptr_test_dict, "ABcd", ptr_object_2);
    assert(strcmp(abel_dict_get_string(ptr_test_dict, key), value) == 0);

    abel_free_dict_ptr(ptr_test_dict);
}

/**
 * Note. This test show that even when one object is
 * referenced by more than 1 container, the recommended
 * freers can relese resources properly.
*/
void test_dict_multi_referenced_object()
{
    char* key = "Abcd";
    int value = 1000;
    /* Create an object pointer */
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    /* And, create two dictionaries. */
    struct abel_dict* ptr_dict_1 = abel_make_dict_ptr();
    struct abel_dict* ptr_dict_2 = abel_make_dict_ptr();
    /* Here comes the multiple ownership. */
    abel_dict_insert(ptr_dict_1, key, ptr_object);
    abel_dict_insert(ptr_dict_2, "Hello", ptr_object);
    /* The above insertions have leveled up the ref count */
    assert(ptr_object->ref_count == 2);
    /* First free the first dictionary */
    abel_free_dict_ptr(ptr_dict_1);
    /* And the ref counter becomes 1 */
    assert(ptr_object->ref_count == 1);
    abel_free_dict_ptr(ptr_dict_2);
}

void test_abel_dict_delete_unique_ownership()
{
    /* Let's start with making a dictionary pointer */
    struct abel_dict* ptr_dict = abel_make_dict_ptr();
    /* ... and insert some stuff into the dict */
    char* key_1 = "Abcd";
    int value_1 = 1000;
    struct abel_object* ptr_object_1 = abel_make_object_ptr(value_1);
    abel_dict_insert(ptr_dict, key_1, ptr_object_1);

    char* key_2 = "Hello";
    double value_2 = 4.1e+8;
    struct abel_object* ptr_object_2 = abel_make_object_ptr(value_2);
    abel_dict_insert(ptr_dict, key_2, ptr_object_2);

    /* Next, create a key that doesn't exit */
    char* key_3 = "Darth";
    /* ... and a return option */
    struct abel_return_option ret;

    /* Sanity check */
    assert(abel_dict_has_key(ptr_dict, key_1) == true);
    assert(abel_dict_has_key(ptr_dict, key_2) == true);
    assert(ptr_object_1->ref_count == 1);
    assert(ptr_object_2->ref_count == 1);

    /* Now, let me delete key 1 */
    ret = abel_dict_delete(ptr_dict, key_1);
    /* Deletion is rather smooth ... */
    assert(ret.is_okay == true);
    /* ... and key 1 is indeed deleted from dict */
    assert(abel_dict_has_key(ptr_dict, key_1) == false);
    /* ... yet key 2 still exists */
    assert(abel_dict_has_key(ptr_dict, key_2) == true);

    /* Attempting a non-existent key results in NULL */
    struct abel_object* ptr = abel_dict_get_object_ptr(ptr_dict, key_1);
    assert(ptr == NULL);
    
    /* Let me delete a key that doesn't exist */
    ret = abel_dict_delete(ptr_dict, key_3);
    /* ... then the return is not okay. */
    assert(ret.is_okay == false);
    /* In fact, the return option complains about missing key */
    assert(ret.error.error_type == KEY_NOT_FOUND);

    /* To this end, key 2 is still there */
    assert(abel_dict_has_key(ptr_dict, key_2) == true);
    /* ... and it holds the cherished data */
    assert(
        *(double*)abel_dict_get_object_ptr(ptr_dict, key_2)->ptr_data == value_2);

    /* Finally, clean up the mess */
    abel_free_dict_ptr(ptr_dict);
}

void test_abel_dict_delete_shared_ownership()
{
    char* key = "Abcd";
    int value = 1000;
    /* Create an object pointer */
    struct abel_object* ptr_object = abel_make_object_ptr(value);
    /* And, create two dictionaries. */
    struct abel_dict* ptr_dict_1 = abel_make_dict_ptr();
    struct abel_dict* ptr_dict_2 = abel_make_dict_ptr();
    /* Here comes the multiple ownership. */
    abel_dict_insert(ptr_dict_1, key, ptr_object);
    abel_dict_insert(ptr_dict_2, "Hello", ptr_object);
    /* The above insertions have leveled up the ref count */
    assert(ptr_object->ref_count == 2);

    /* First, delete the only key from the first dictionary */
    abel_dict_delete(ptr_dict_1, key);
    /* ... and the first dict is empty */
    assert(abel_dict_has_key(ptr_dict_1, key) == false);
    /* As the object is co-owned, the ref count decreases */
    assert(ptr_object->ref_count == 1);
    /* ... but it still referenced in second dictionary */
    assert(*(int*)abel_dict_get_object_ptr(ptr_dict_2, "Hello")->ptr_data == value);

    abel_free_dict_ptr(ptr_dict_1);
    abel_free_dict_ptr(ptr_dict_2);
}

int main()
{
    /* regular offsite unittest */
    test_dict_insert();
    test_dict_insert_bool();
    test_dict_insert_null();
    test_dict_insert_string();
    test_dict_insert_double();

    /* Terminal-data getters */
    test_dict_get_bool();
    test_dict_get();
    test_dict_get_int();
    test_dict_get_double();
    test_dict_get_string();
    test_dict_insert_dict_ptr();

    /* test different ownerships */
    test_dict_multi_referenced_object();
    test_abel_dict_delete_unique_ownership();
    test_abel_dict_delete_shared_ownership();
}