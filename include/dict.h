/*
 * Header dict.h
 * Created 21-10-2022
 * Updated 21-11-2022
 *
 * Abel dictionary wraps a map object and delegate most
 * functions to it.
 */
#ifndef ABEL_ON_C_DICT_H
#define ABEL_ON_C_DICT_H

#include "object.h"

/**
 * @brief Make a dict on heap
 *
 * Returns a pointer to a dictionary created on heap.
 */
struct abel_dict* abel_make_dict_ptr();

/**
 * @brief Check if dict has the given key
 *
 * Returns true / false if a key exists in map. It delegates
 * the key searching to its internal map.
 * 
 * @return If key is found, returns `true`; otherwise `false`.
 */
Bool abel_dict_has_key(struct abel_dict* ptr_dict, char* key_str);

/**
 * @brief Dictionary size
 * 
 * Size is the number of pairs stored in it.
 */
size_t abel_dict_size(struct abel_dict* ptr_map);

/**
 * @brief Insert a key-value pair into dict
 *
 * Insertion into a dictionary is delegated to map's insert.
 * If insertion is successful, struct abel_object's ref count increased
 * by 1.
 * 
 * @param ptr_dict : Pointer to the target dictionary.
 * @param key_str : Key to the key-value pair to be inserted
 *                  into dict.
 * @param ptr_obj : Pointer to the object to be inserted into
 *                  the dict.
 */
struct abel_return_option abel_dict_insert(
        struct abel_dict* ptr_dict, char* key_str, struct abel_object* ptr_obj);
/* Bool */
struct abel_return_option abel_dict_insert_bool_ptr(
        struct abel_dict* ptr_dict, char* key_str, Bool* ptr_src_data);
struct abel_return_option abel_dict_insert_bool(
        struct abel_dict* ptr_dict, char* key_str,  Bool src_data);
/* Null */
struct abel_return_option abel_dict_insert_null_ptr(
        struct abel_dict* ptr_dict, char* key_str, Null* ptr_src_data);
struct abel_return_option abel_dict_insert_null(
        struct abel_dict* ptr_dict, char* key_str, Null src_data);
/* String */
struct abel_return_option abel_dict_insert_string(
        struct abel_dict* ptr_dict, char* key_str, char* src_str);
/* Double */
struct abel_return_option abel_dict_insert_double_ptr(
        struct abel_dict* ptr_dict, char* key_str, double* ptr_src_data);
struct abel_return_option abel_dict_insert_double(
        struct abel_dict* ptr_dict, char* key_str, double src_data);
/* struct abel_dict */
struct abel_return_option abel_dict_insert_dict_ptr(
        struct abel_dict* ptr_dict, char* key_str, struct abel_dict* ptr_src_data);

/**
 * @brief Delete a key-value from dict
 *
 * Calls `abel_map_erase` function to erase the key-value
 * pair. Not that function `abel_map_erase` disconnects
 * the pair from the map and returns the pointer to it
 * in the return option. Dictionary then decides what to
 * do with it.
 * 
 * If the key is found, return an error. Otherwise,
 * the object associated with the key is either deleted
 * (unique ownership) or disassociated (shared ownership).
 * 
 * @return struct abel_return_option instance.
 *         - If successful, flag is_okay is true and pointer
 *           is NULL.
 *         - If failure, flag is_error is true and error is
 *           stored in error.
 */
struct abel_return_option abel_dict_delete(
        struct abel_dict* ptr_dict, char* key_str);

/**
 * @brief Get object from dict
 *
 * This function is mostly facilitating the terminal data
 * getters.
 * 
 * @return If the key exists, pointer to the desired object
 *         is returned; otherwise NULL is returned.
 * 
 * @warning Should I return option or just pointer?
 */
struct abel_object* abel_dict_get_object_ptr(
        struct abel_dict* ptr_dict, char* key_str);

/**
 * @brief Get the type of object at given key
 */
enum data_type abel_dict_get_data_type(struct abel_dict* ptr_dict, char* key_str);

/* Terminal-data getter */
Bool* abel_dict_get_bool_ptr(struct abel_dict* ptr_dict, char* key_str);
Bool abel_dict_get_bool(struct abel_dict* ptr_dict, char* key_str);
int* abel_dict_get_int_ptr(struct abel_dict* ptr_dict, char* key_str);
int abel_dict_get_int(struct abel_dict* ptr_dict, char* key_str);
double* abel_dict_get_double_ptr(struct abel_dict* ptr_dict, char* key_str);
double abel_dict_get_double(struct abel_dict* ptr_dict, char* key_str);
char* abel_dict_get_string(struct abel_dict* ptr_dict, char* key_str);

/* Container pointer getter */
struct abel_list* abel_dict_get_list_ptr(struct abel_dict* ptr_dict, char* key_str);
struct abel_dict* abel_dict_get_dict_ptr(struct abel_dict* ptr_dict, char* key_str);

#endif