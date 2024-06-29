/* Examples */
#include <assert.h>

/* Only need to include abelc.h */
#include "abelc.h"

/**
 * Example 1. Create, insert and access a dictionary.
 */
void operating_on_dict(void)
{
    /* Create a dict (pointer) */
    struct abel_dict* ptr_test_dict = abel_make_dict_ptr();
    
    /* Insert ("Halo":"World") pair */
    abel_dict_insert_string(ptr_test_dict, "Halo", "World");
    /* Insert ("pi":3.1415926) pair */
    abel_dict_insert_double(ptr_test_dict, "pi", 3.1415926);

    /* Access via Abel object */
    struct abel_object* acquired_value
            = abel_dict_get_object_ptr(ptr_test_dict, "Halo");
    assert(strcmp((char*)(char*)(acquired_value->ptr_data), "World") == 0);
    assert(strcmp((char*)(char*)(abel_dict_get_object_ptr(ptr_test_dict, "Halo")->ptr_data), "World") == 0);

    /* Access via type-specific APIs */
    assert(strcmp(abel_dict_get_string(ptr_test_dict, "Halo"), "World") == 0);
    assert(abel_dict_get_double(ptr_test_dict, "pi") == 3.1415926);

    /* Check if key exists */
    assert(abel_dict_has_key(ptr_test_dict, "Halo") == true);
    assert(abel_dict_has_key(ptr_test_dict, "Nr") == false);
    assert(abel_dict_has_key(ptr_test_dict, "PI") == false);
    assert(abel_dict_has_key(ptr_test_dict, "pi") == true);

    /*
    So far, the test dict is
    {"Halo": "World", "pi": 3.1415926}
    Let's insert a dictionary into this.
    */
    
    /* Create a sub dict {"Filter": "ReLU"} */
    struct abel_dict* ptr_subdict = abel_make_dict_ptr();
    abel_dict_insert_string(ptr_subdict, "Filter", "ReLU");
    /* Insert this dict inside the test dict */
    abel_dict_insert_dict_ptr(ptr_test_dict, "layer", ptr_subdict);

    /*
    Now, the test dict becomes
    {"Halo":"World", "pi":3.1415926, "layer":{"Filter": "ReLU"}}
    */
    assert(abel_dict_has_key(ptr_test_dict, "layer") == true);
    struct abel_dict* ptr_acquired_subdict
            = abel_dict_get_dict_ptr(ptr_test_dict, "layer");
    /* Access inside the sub dict */
    assert(strcmp(abel_dict_get_string(ptr_acquired_subdict, "Filter"), "ReLU") == 0);

    /*
    If you don't know what type of data is associated
    with a key, use the following
    */
    assert(abel_dict_get_data_type(ptr_test_dict, "layer") == DICT_TYPE);
    assert(abel_dict_get_data_type(ptr_test_dict, "pi") != DICT_TYPE);
    assert(abel_dict_get_data_type(ptr_test_dict, "pi") == DOUBLE_TYPE);
    assert(abel_dict_get_data_type(ptr_test_dict, "Halo") == STRING_TYPE);

    /*
    IMPORTANT. Free the resources.
    However, you only need to free the outmost dictionary.
    */
    abel_free_dict_ptr(ptr_test_dict);
}

/**
 * Example 2. Create, insert and access a list.
 */
void operating_on_list(void)
{
    /* Create an empty list */
    struct abel_list* ptr_test_list = abel_make_list_ptr(0);
    abel_list_append(ptr_test_list, 0.87);    // double
    abel_list_append(ptr_test_list, (Bool)false);    // Bool
    abel_list_append(ptr_test_list, "|01001>");    // String

    /* Now the list is [ 0.87, false, "|01001>" ]*/
    assert(abel_list_get_data_type(ptr_test_list, 2) == STRING_TYPE);
    assert(abel_list_get_data_type(ptr_test_list, 1) == BOOL_TYPE);
    assert(abel_list_get_data_type(ptr_test_list, 0) == DOUBLE_TYPE);

    /* Let's insert a dictionary */
    struct abel_dict* ptr_subdict = abel_make_dict_ptr();
    abel_dict_insert_string(ptr_subdict, "Filter", "ReLU");
    /* The subdict is {"Filter":"ReLU"}*/

    /* Let's append it to the list */
    abel_list_append(ptr_test_list, ptr_subdict);

    /* Size of the list becomes 4 */
    assert(abel_list_size(ptr_test_list) == 4);
    /* Last element has dict data type */
    assert(abel_list_get_data_type(ptr_test_list, 3) == DICT_TYPE);

    /* Free the list */
    abel_free_list_ptr(ptr_test_list);
}

/**
 * Example 3. Load a simple dictionary type of file.
 * File: ./files/simple_dict.abel
 * 
 * This simple file is a dictionary contains 2 keys.
 * Comments are included. See the following
 * 
 * # Simple dictionary
 * {
 *     "num": 10, # double type
 * # next key is a string
 *     "str": "Halo"
 * }
 * 
 */
void loader_simple_dict()
{
    /* Create a parser */
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    /* Parse the file */
    abel_parse_file(&test_parser, "./files/simple_dict.abel");
    
    /* Create a loader */
    struct json_loader test_loader = able_make_json_loader();

    /* Create a global dictionary to store the parsed */
    struct abel_dict* ptr_global_dict = abel_make_dict_ptr();

    /*
    Call loader API to populate the global dictionary using
    the parser. The global dictionary has only 1 key, i.e.
    the root key
    
    Global dictionary = {
       "ROOT_KEY_": [ {} ]
    }
    */
    load_from_parser(&test_loader, &test_parser, ptr_global_dict);

    /*
    We can check what type of container the root key
    is associated with. In this case it is a list.
    */
    struct abel_object* ptr_object
            = abel_dict_get_object_ptr(ptr_global_dict, "ROOT_KEY_");
    assert(ptr_object->data_type == LIST_TYPE);

    /* Acquire this list */
    struct abel_list* ptr_root_list = ptr_object->ptr_data;
    
    /*
    The desired ditionary is the first element in the list
    is a dictionary. We may just work with the pointer to
    this dictionary.
    */
    struct abel_dict* ptr_file_dict
            = abel_list_get_object_pointer(ptr_root_list, 0)->ptr_data;
    /*
    Acquire entries in this dictionary
    */
    double db = abel_dict_get_double(ptr_file_dict, "num");
    assert(db == 10.0);
    char* str = abel_dict_get_string(ptr_file_dict, "str");
    assert(strcmp(str, "Halo")==0);

    /* Free the resource */
    abel_free_json_parser(&test_parser);
    abel_free_dict_ptr(ptr_global_dict);
}

/**
 * Example 4. The standard JSON file has the following
 * {
 *     "dble": 1e-6,
 *     "list": ["A", 3.14, "Relu"],
 *     "dict": {
 *         "layer1": [640, 640, "RGB"]
 *     }
 * }
 * 
 * This dictionary is put into a list and associated
 * with the key "ROOT_KEY_" in the global dictionary.
 * 
 */
void load_a_standard_json()
{
    /* Create a parser */
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    abel_parse_file(&test_parser, "./files/standard.json");
    /* create loader and global dict */
    struct json_loader test_loader = able_make_json_loader();
    struct abel_dict* ptr_global_dict = abel_make_dict_ptr();

    /* Load the global dict { "ROOT_KEY_": [ {...} ]} */ 
    load_from_parser(&test_loader, &test_parser, ptr_global_dict);
    /* Probe the type of the root container */
    struct abel_object* ptr_object
            = abel_dict_get_object_ptr(ptr_global_dict, "ROOT_KEY_");
    /* should be a list */
    assert(ptr_object->data_type == LIST_TYPE);

    /* Get the desired dictionary from the root list */
    struct abel_list* ptr_root_list = ptr_object->ptr_data;
    /* First element is the desired a dictionary */ 
    struct abel_dict* ptr_file_dict
            = abel_list_get_object_pointer(ptr_root_list, 0)->ptr_data;

    /* Explore the dictionary */
    double db = abel_dict_get_double(ptr_file_dict, "dble");
    assert(db == 1e-6);

    /* One entry in the dict is a list */
    struct abel_list* ptr_sublist = abel_dict_get_list_ptr(ptr_file_dict, "list");
    /* check the types of elements */
    assert(abel_list_get_data_type(ptr_sublist, 0) == STRING_TYPE);
    assert(abel_list_get_data_type(ptr_sublist, 1) != STRING_TYPE);
    assert(abel_list_get_data_type(ptr_sublist, 1) == DOUBLE_TYPE);
    /* Get the 3rd element */
    char* str = abel_list_get_object_pointer(ptr_sublist, 2)->ptr_data;
    assert(strcmp(str, "Relu") == 0);

    /* Another entry is a sub dictionary */
    struct abel_dict* ptr_subdict = abel_dict_get_dict_ptr(ptr_file_dict, "dict");
    /* Inside the sub dict there is a list */
    struct abel_list* ptr_list = abel_dict_get_list_ptr(ptr_subdict, "layer1");
    /* Walk through the elements in the list */
    assert(abel_list_get_double(ptr_list, 0) == 640);
    assert(abel_list_get_double(ptr_list, 1) == 640);
    str = abel_list_get_object_pointer(ptr_list, 2)->ptr_data;
    assert(strcmp(str, "RGB") == 0);

    /* Free parser and loader */
    abel_free_json_parser(&test_parser);
    abel_free_dict_ptr(ptr_global_dict);
}

int main(void)
{
    operating_on_dict();
    operating_on_list();
    loader_simple_dict();
    load_a_standard_json();
    return 0;
}