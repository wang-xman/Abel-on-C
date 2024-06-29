/**
 * Source dict.c
 * Created 21-10-2022
 * Updated 23-11-2022
 **/
#include "dict.h"

/* Public */

struct abel_dict* abel_make_dict_ptr()
{
    struct abel_dict* ptr_dict = NULL;
    ptr_dict = malloc( sizeof(*ptr_dict) );
    ptr_dict->ptr_map = abel_make_map_ptr();
    ptr_dict->data_type = OBJECT_TYPE;
    return ptr_dict;
}

Bool abel_dict_has_key(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_return_option ret_from_map
            = abel_map_find(ptr_dict->ptr_map, key_str);
    if (ret_from_map.is_okay == true) {
        return true;
    } else {
        return false;
    }
}

size_t abel_dict_size(struct abel_dict* ptr_dict)
{
    return abel_map_size(ptr_dict->ptr_map);
}

struct abel_return_option abel_dict_insert(
        struct abel_dict* ptr_dict, char* key_str, struct abel_object* ptr_obj)
{
    struct abel_return_option ret_map_insert;
    ret_map_insert = abel_map_insert(ptr_dict->ptr_map, key_str, ptr_obj);
    if (ret_map_insert.is_okay) {    // if succeeds, update ref count
        ptr_obj->ref_count += 1;
    }
    return ret_map_insert;
}

struct abel_return_option abel_dict_insert_bool_ptr(
        struct abel_dict* ptr_dict, char* key_str, Bool* ptr_src_data)
{
    return abel_dict_insert( ptr_dict, key_str,
                             abel_make_object_ptr_from_bool_ptr(ptr_src_data) );
}

struct abel_return_option abel_dict_insert_bool(
        struct abel_dict* ptr_dict, char* key_str, Bool src_data)
{
    return abel_dict_insert_bool_ptr(ptr_dict, key_str, &src_data);
}

struct abel_return_option abel_dict_insert_null_ptr(
        struct abel_dict* ptr_dict, char* key_str, Null* ptr_src_data)
{
    return abel_dict_insert( ptr_dict, key_str,
                             abel_make_object_ptr_from_null_ptr(ptr_src_data) );
}

struct abel_return_option abel_dict_insert_null(
        struct abel_dict* ptr_dict, char* key_str, Null src_data)
{
    return abel_dict_insert_null_ptr(ptr_dict, key_str, &src_data);
}

struct abel_return_option abel_dict_insert_string(
        struct abel_dict* ptr_dict, char* key_str, char* src_str)
{
    return abel_dict_insert( ptr_dict, key_str,
                             abel_make_object_ptr_from_string(src_str) );
}

struct abel_return_option abel_dict_insert_double_ptr(
        struct abel_dict* ptr_dict, char* key_str, double* ptr_src_data)
{
    return abel_dict_insert( ptr_dict, key_str,
                             abel_make_object_ptr_from_double_ptr(ptr_src_data) );
}

struct abel_return_option abel_dict_insert_double(
        struct abel_dict* ptr_dict, char* key_str, double src_data)
{
    return abel_dict_insert_double_ptr(ptr_dict, key_str, &src_data);
}

/* struct abel_dict */
struct abel_return_option abel_dict_insert_dict_ptr(
        struct abel_dict* ptr_dict, char* key_str, struct abel_dict* ptr_src_data)
{
    return abel_dict_insert( ptr_dict, key_str,
                             abel_make_object_ptr_from_dict_ptr(ptr_src_data) );
}

struct abel_return_option abel_dict_delete(
        struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_return_option ret;
    struct abel_key_value_pair* ptr_pair_to_erase = NULL;
    ret = abel_map_erase(ptr_dict->ptr_map, key_str);
    if (ret.is_okay == true) {
        ptr_pair_to_erase = ret.pointer;
        /* free object */
        abel_free_object_ptr(ptr_pair_to_erase->ptr_data);
        abel_free_pair(ptr_pair_to_erase);
        ret = abel_option_okay(NULL);
    }
    return ret;
}

/* Getters */

struct abel_object* abel_dict_get_object_ptr(
        struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_return_option ret_map_at
            = abel_map_at(ptr_dict->ptr_map, key_str);
    if (ret_map_at.is_okay) {
        return (struct abel_object*) ret_map_at.pointer;
    } else {
        return NULL;
    }
}

enum data_type abel_dict_get_data_type(struct abel_dict* ptr_dict, char* key_str)
{
    return abel_dict_get_object_ptr(ptr_dict, key_str)->data_type;
}

/* Terminal data getter */

Bool* abel_dict_get_bool_ptr(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_object* ptr_object_temp
            = abel_dict_get_object_ptr(ptr_dict, key_str);
    Bool* ptr_ret = NULL;
    if (ptr_object_temp!= NULL && ptr_object_temp->ptr_data != NULL
            && ptr_object_temp->data_type == BOOL_TYPE) {
        ptr_ret = (Bool*)ptr_object_temp->ptr_data;
    }
    return ptr_ret;
}

Bool abel_dict_get_bool(struct abel_dict* ptr_dict, char* key_str)
{
    Bool ret = *abel_dict_get_bool_ptr(ptr_dict, key_str);
    return ret;
}

int* abel_dict_get_int_ptr(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_object* ptr_object_temp
            = abel_dict_get_object_ptr(ptr_dict, key_str);
    int* ptr_ret = NULL;
    if (ptr_object_temp!= NULL && ptr_object_temp->ptr_data != NULL
            && ptr_object_temp->data_type == INTEGER_TYPE) {
        ptr_ret = (int*)ptr_object_temp->ptr_data;
    }
    return ptr_ret;
}

int abel_dict_get_int(struct abel_dict* ptr_dict, char* key_str)
{
    int ret = *abel_dict_get_int_ptr(ptr_dict, key_str);
    return ret;
}

double* abel_dict_get_double_ptr(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_object* ptr_object_temp
            = abel_dict_get_object_ptr(ptr_dict, key_str);
    double* ptr_ret = NULL;
    if (ptr_object_temp!= NULL && ptr_object_temp->ptr_data != NULL
            && ptr_object_temp->data_type == DOUBLE_TYPE) {
        ptr_ret = (double*)ptr_object_temp->ptr_data;
    }
    return ptr_ret;
}

double abel_dict_get_double(struct abel_dict* ptr_dict, char* key_str)
{
    double ret = *abel_dict_get_double_ptr(ptr_dict, key_str);
    return ret;
}

char* abel_dict_get_string(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_object* ptr_object_temp
            = abel_dict_get_object_ptr(ptr_dict, key_str);
    char* ptr_ret = NULL;
    if (ptr_object_temp!= NULL && ptr_object_temp->ptr_data != NULL
            && ptr_object_temp->data_type == STRING_TYPE) {
        ptr_ret = (char*)ptr_object_temp->ptr_data;
    }
    return ptr_ret;
}

/* Container pointer getter */

struct abel_list* abel_dict_get_list_ptr(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_object* ptr_object_temp
            = abel_dict_get_object_ptr(ptr_dict, key_str);
    struct abel_list* ptr_ret = NULL;
    if (ptr_object_temp!= NULL && ptr_object_temp->ptr_data != NULL
            && ptr_object_temp->data_type == LIST_TYPE) {
        ptr_ret = (struct abel_list*)ptr_object_temp->ptr_data;
    }
    return ptr_ret;
}

struct abel_dict* abel_dict_get_dict_ptr(struct abel_dict* ptr_dict, char* key_str)
{
    struct abel_object* ptr_object_temp
            = abel_dict_get_object_ptr(ptr_dict, key_str);
    struct abel_dict* ptr_ret = NULL;
    if (ptr_object_temp!= NULL && ptr_object_temp->ptr_data != NULL
            && ptr_object_temp->data_type == DICT_TYPE) {
        ptr_ret = (struct abel_dict*)ptr_object_temp->ptr_data;
    }
    return ptr_ret;
}