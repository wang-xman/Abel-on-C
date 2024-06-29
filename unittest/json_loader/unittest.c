/** 
 * Unittest JSON loader
 * Test public functions
 */
#include <assert.h>
#include "json_loader.h"

/**
 * @brief Test parse_file function
 * 
 * @note Default directory is the one where this unittest
 *       file locates.
 */
void test_json_loader_simple_dict()
{
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    abel_parse_file(&test_parser, "./files/simple_dict.txt");
    
    struct json_loader test_loader = able_make_json_loader();
    struct abel_dict* ptr_global_dict = abel_make_dict_ptr();

    // load everything into global dict which has only 1 key
    // { "ROOT_KEY_": [ {} ]}
    load_from_parser(&test_loader, &test_parser, ptr_global_dict);
    struct abel_object* ptr_object
            = abel_dict_get_object_ptr(ptr_global_dict, "ROOT_KEY_");
    // should be LIST
    assert(ptr_object->data_type == LIST_TYPE);
    struct abel_list* ptr_root_list = ptr_object->ptr_data;
    // first element in the list is a dictionary
    struct abel_dict* ptr_file_dict
            = abel_list_get_object_pointer(ptr_root_list, 0)->ptr_data;
    // convert to dict
    double db = abel_dict_get_double(ptr_file_dict, "num");
    assert(db == 10.0);
    char* str = abel_dict_get_string(ptr_file_dict, "str");
    assert(strcmp(str, "Halo")==0);

    // freer
    abel_free_json_parser(&test_parser);
    abel_free_dict_ptr(ptr_global_dict);
}

void test_json_loader_nested()
{
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    abel_parse_file(&test_parser, "./files/nested.json");
    
    struct json_loader test_loader = able_make_json_loader();
    struct abel_dict* ptr_global_dict = abel_make_dict_ptr();

    // load everything into global dict which has only 1 key
    // { "ROOT_KEY_": [ {} ]}
    load_from_parser(&test_loader, &test_parser, ptr_global_dict);
    struct abel_object* ptr_object
            = abel_dict_get_object_ptr(ptr_global_dict, "ROOT_KEY_");
    // should be LIST
    assert(ptr_object->data_type == LIST_TYPE);
    struct abel_list* ptr_root_list = ptr_object->ptr_data;
    // first element in the list is a dictionary
    struct abel_dict* ptr_file_dict
            = abel_list_get_object_pointer(ptr_root_list, 0)->ptr_data;

    // convert to dict
    double db = abel_dict_get_double(ptr_file_dict, "dble");
    assert(db == 1e-6);
    // acquire the list
    struct abel_list* ptr_sublist = abel_dict_get_list_ptr(ptr_file_dict, "list");
    // check the type of elements
    assert(abel_list_get_data_type(ptr_sublist, 0) == STRING_TYPE);
    assert(abel_list_get_data_type(ptr_sublist, 1) != STRING_TYPE);
    assert(abel_list_get_data_type(ptr_sublist, 1) == DOUBLE_TYPE);
    // get the 3rd element
    char* str = abel_list_get_object_pointer(ptr_sublist, 2)->ptr_data;
    assert(strcmp(str, "Relu") == 0);

    // acquire the sub-dict
    struct abel_dict* ptr_subdict = abel_dict_get_dict_ptr(ptr_file_dict, "dict");
    // access the key to acquire a list
    struct abel_list* ptr_list = abel_dict_get_list_ptr(ptr_subdict, "layer1");
    assert(abel_list_get_double(ptr_list, 0) == 640);
    assert(abel_list_get_double(ptr_list, 1) == 640);
    str = abel_list_get_object_pointer(ptr_list, 2)->ptr_data;
    assert(strcmp(str, "RGB") == 0);

    // freer
    abel_free_json_parser(&test_parser);
    abel_free_dict_ptr(ptr_global_dict);
}

int main()
{
    test_json_loader_simple_dict();
    test_json_loader_nested();
}