/*
 * Unittest container
 *
 * Tests the mixture of containers and the interaction.
 */
#include <assert.h>
#include "container.h"

void test_list_in_dict()
/*
    Note. This test shows that one only needs to free the
    outmost container. Structure of the dictionary
        {
            "list" : [6789]
        }
*/
{
    /* Let me start with making a dictionary */
    struct abel_dict* ptr_dict = abel_make_dict_ptr();

    /* First, make a list ...  */
    struct abel_list* ptr_list = abel_make_list_ptr(5);
    /* ... and append some data into it. */
    abel_list_append(ptr_list, 6789);
    /* The list has now some data, I can check. */
    assert(abel_list_size(ptr_list) == 1);
    assert(abel_list_get_int(ptr_list, 0) == 6789);

    /* Now to insert the list into dict, I need an object wrapper ... */
    struct abel_object* ptr_object = abel_make_object_ptr_from_list_ptr(ptr_list);
    /* ... and a key string ... */
    char* key = "list";
    /* ... to insert into the dictionary. */
    abel_dict_insert(ptr_dict, key, ptr_object);

    /* Sanity check */
    assert(abel_dict_has_key(ptr_dict, key) == true);
    assert(abel_dict_get_object_ptr(ptr_dict, key)->data_type == LIST_TYPE);

    /* Before obliterate the dict, I can play with it */
    //assert(abel_dict_get_list_ptr(ptr_dict, key)->length == 1);
    /* For example, get the integer from the list.*/
    assert(abel_list_get(abel_dict_get_list_ptr(ptr_dict, key), 0, (int)1) == 6789);

    abel_free_dict_ptr(ptr_dict);
}

void test_dict_in_dict()
/*
    Structure of the dictionary
        ptr_super_dict : {
            "dict" : {
                "list" : [6789]
            }
        }
*/
{
    /* Let me start with making a dictionary */
    struct abel_dict* ptr_dict = abel_make_dict_ptr();

    /* First, make a list ...  */
    struct abel_list* ptr_list = abel_make_list_ptr(5);
    /* ... and append some data into it. */
    abel_list_append(ptr_list, 6789);
    /* The list has now some data, I can check. */
    assert(abel_list_size(ptr_list) == 1);
    assert(abel_list_get_int(ptr_list, 0) == 6789);

    /* Now to insert the list into dict, I need an object wrapper ... */
    struct abel_object* ptr_object = abel_make_object_ptr_from_list_ptr(ptr_list);
    /* ... and a key string ... */
    char* key = "list";
    /* ... to insert into the dictionary. */
    abel_dict_insert(ptr_dict, key, ptr_object);

    /* Sanity check */
    assert(abel_dict_has_key(ptr_dict, key) == true);
    assert(abel_dict_get_object_ptr(ptr_dict, key)->data_type == LIST_TYPE);

    /* Now, create the super dictionary ... */
    struct abel_dict* ptr_super_dict = abel_make_dict_ptr();
    /* ... and insert the dict above into it.*/
    ptr_object = abel_make_object_ptr_from_dict_ptr(ptr_dict);
    abel_dict_insert(ptr_super_dict, "dict", ptr_object);

    /* Let's see if everything went as expected. */
    assert(abel_dict_has_key(ptr_super_dict, "dict") == true);
    /* Get the dict inside the dict ... */
    struct abel_dict* ptr_dict_acquired = abel_dict_get_dict_ptr(ptr_super_dict, "dict");
    /* ... and check what keys does it have. */
    assert(abel_dict_has_key(ptr_dict_acquired, "list") == true);
    /* Okay, since it has the key, I can check its type via object ptr */
    assert(abel_dict_get_object_ptr(ptr_dict_acquired, "list")->data_type == LIST_TYPE);
    /* Since it is a list, let me acquire it from the acquired sub-dict. */
    struct abel_list* ptr_list_acquired = abel_dict_get_list_ptr(ptr_dict_acquired, "list");
    /* Hey, how many items are in that list?*/
    assert(abel_list_size(ptr_list_acquired) == 1);
    /* What is the type of that item? */
    assert(abel_list_get_object_pointer(ptr_list_acquired, 0)->data_type == INTEGER_TYPE);
    /* So, it is an integer. Let me get its value. */
    assert(abel_list_get_int(ptr_list_acquired, 0) == 6789);

    /* Get a bit bored? Free the outmost container. */
    abel_free_dict_ptr(ptr_super_dict);
}

int main()
{
    //test_list_in_dict();
    test_dict_in_dict();
}