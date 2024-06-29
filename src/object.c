/*
 * Source object.h
 * Created 21-10-2022
 * Updated 08-11-2022
 */
#include "object.h"

/* Object-pointer makers */

static struct abel_object* make_object_pointer_on_heap(
    void* ptr_data, size_t data_size, enum data_type data_type)
{
    struct abel_object* ptr_object = NULL;
    ptr_object = malloc( sizeof(*ptr_object) );
    if (ptr_object != NULL) {
        ptr_object->ptr_data = ptr_data;
        ptr_object->data_size = data_size;
        ptr_object->data_type = data_type;
        ptr_object->ref_count = 0;
        return ptr_object;
    } else {
        return NULL;
    }
}

/* Bool */
struct abel_object* abel_make_object_ptr_from_bool_ptr(Bool* ptr_src_data)
{
    Bool* ptr_data = NULL;
    struct abel_object* ptr_object = NULL;
    ptr_data = malloc( sizeof(*ptr_data) );
    *ptr_data = *ptr_src_data;    // copy data via ptr
    ptr_object = make_object_pointer_on_heap(
            ptr_data, sizeof(*ptr_data), BOOL_TYPE);
    return ptr_object;
}

struct abel_object* abel_make_object_ptr_from_bool(Bool src_data)
{
    return abel_make_object_ptr_from_bool_ptr(&src_data);
}

/* Null */
struct abel_object* abel_make_object_ptr_from_null_ptr(Null* ptr_src_data)
{
    Null* ptr_data = NULL;
    struct abel_object* ptr_object = NULL;
    ptr_data = malloc( sizeof(*ptr_data) );
    *ptr_data = *ptr_src_data;    // copy data via ptr
    ptr_object = make_object_pointer_on_heap(
            ptr_data, sizeof(*ptr_data), NULL_TYPE);
    return ptr_object;
}

struct abel_object* abel_make_object_ptr_from_null(Null src_data)
{
    return abel_make_object_ptr_from_null_ptr(&src_data);
}

/* string */
struct abel_object* abel_make_object_ptr_from_string(char* src_str)
{
    size_t src_length = 0;
    char* ptr_data = NULL;
    struct abel_object* ptr_object = NULL;
    src_length = strlen(src_str);
    ptr_data = malloc( sizeof(char)*(src_length + 1) );
    strncpy(ptr_data, src_str, src_length + 1);    // source string copied
    ptr_object = make_object_pointer_on_heap(
            ptr_data, sizeof(*ptr_data), STRING_TYPE);
    return ptr_object;
}

/* int */
struct abel_object* abel_make_object_ptr_from_int_ptr(int* ptr_src_data)
{
    int* ptr_data = NULL;
    struct abel_object* ptr_object = NULL;
    ptr_data = malloc( sizeof(*ptr_data) );
    *ptr_data = *ptr_src_data;    // copy data via ptr
    ptr_object = make_object_pointer_on_heap(
            ptr_data, sizeof(*ptr_data), INTEGER_TYPE);
    return ptr_object;
}

struct abel_object* abel_make_object_ptr_from_int(int src_data)
{
    return abel_make_object_ptr_from_int_ptr(&src_data);
}

/* double */
struct abel_object* abel_make_object_ptr_from_double_ptr(double* ptr_src_data)
{
    double* ptr_data = NULL;
    struct abel_object* ptr_object = NULL;
    ptr_data = malloc( sizeof(*ptr_data) );
    *ptr_data = *ptr_src_data;    // copy data
    ptr_object = make_object_pointer_on_heap(
            ptr_data, sizeof(*ptr_data), DOUBLE_TYPE);
    return ptr_object;
}

struct abel_object* abel_make_object_ptr_from_double(double src_data)
{
    return abel_make_object_ptr_from_double_ptr(&src_data);
}

/* list */
struct abel_object* abel_make_object_ptr_from_list_ptr(struct abel_list* ptr_src_data)
{
    struct abel_object* ptr_object = NULL;
    ptr_object = make_object_pointer_on_heap(
            ptr_src_data, sizeof(*ptr_src_data), LIST_TYPE);
    return ptr_object;
}

/* dict */
struct abel_object* abel_make_object_ptr_from_dict_ptr(struct abel_dict* ptr_src_data)
{
    struct abel_object* ptr_object = NULL;
    ptr_object = make_object_pointer_on_heap(
            ptr_src_data, sizeof(*ptr_src_data), DICT_TYPE);
    return ptr_object;
}

/* Terminal-data getters */

Bool abel_object_get_bool(struct abel_object* ptr_obj)
{
    return *(Bool*)(ptr_obj->ptr_data);
}

Null abel_object_get_null(struct abel_object* ptr_obj)
{
    return *(Null*)(ptr_obj->ptr_data);
}

char* abel_object_get_string(struct abel_object* ptr_obj)
{
    return (char*)(ptr_obj->ptr_data);
}

int abel_object_get_int(struct abel_object* ptr_obj)
{
    return *(int*)(ptr_obj->ptr_data);
}

double abel_object_get_double(struct abel_object* ptr_obj)
{
    return *(double*)(ptr_obj->ptr_data);
}

/* Get container pointer */
struct abel_list* abel_object_get_list_ptr(struct abel_object* ptr_obj)
{
    return (struct abel_list*)(ptr_obj->ptr_data);
}

struct abel_dict* abel_object_get_dict_ptr(struct abel_object* ptr_obj)
{
    return (struct abel_dict*)(ptr_obj->ptr_data);
}

/* Data type getters */
enum data_type abel_object_get_type(struct abel_object* ptr_obj)
{
    return ptr_obj->data_type;
}

/* Data type string getters */
char* abel_object_get_type_string(struct abel_object* ptr_obj)
{
    return DATA_TYPE_STRING[ptr_obj->data_type];
}

/* Freers */

struct abel_return_option abel_free_list_ptr(struct abel_list* ptr_list)
{
    struct abel_return_option ret;
    if (ptr_list != NULL) {    // no need to free empty pointer
        for (int i = 0; i < ptr_list->ptr_vector->size; i++) {
            if (ptr_list->ptr_vector->ptr_array[i] != NULL) {
                ret = abel_free_object_ptr(ptr_list->ptr_vector->ptr_array[i]);
            }
        }
        /* after all objects are freed, free the list */
        free(ptr_list->ptr_vector->ptr_array); 
        free(ptr_list->ptr_vector); 
        free(ptr_list);
    } else {
        /* THINK! Shall I return okay? Or a specific error. */
        free(ptr_list);
        ret = abel_option_okay(NULL);
    }
    return ret;
}

/**
 * @brief Static - Linked list freer
 *
 * This freer is called by dictionary freer to free
 * the collision array.
 */
static struct abel_return_option free_linked_list(
    struct abel_linked_list* ptr_head)
{
    struct abel_return_option ret;
    struct abel_linked_list* ptr_node_tofree = NULL;
    struct abel_linked_list* ptr_current_node = ptr_head;
    struct abel_key_value_pair* ptr_pair = NULL;
    while (ptr_current_node != NULL) {
        ptr_pair = (struct abel_key_value_pair*)ptr_current_node->ptr_data;
        if (ptr_pair->ptr_data != NULL) {
            ret = abel_free_object_ptr(ptr_pair->ptr_data);
            abel_free_pair(ptr_pair);
        }
        ptr_node_tofree = ptr_current_node;
        ptr_current_node = ptr_current_node->next;
        free(ptr_node_tofree);
    }
    return ret;
}

struct abel_return_option abel_free_dict_ptr(struct abel_dict* ptr_dict)
{
    struct abel_return_option ret;
    size_t vector_capacity = ptr_dict->ptr_map->ptr_pair_vector->capacity;
    struct abel_key_value_pair* ptr_acquired_pair = NULL;
    struct abel_object* ptr_acquired_object = NULL;
    struct abel_linked_list* ptr_linked_list_head = NULL;
    for (int i = 0; i < vector_capacity; i++) {
        /* free item array */
        ptr_acquired_pair = ptr_dict->ptr_map->ptr_pair_vector->ptr_array[i];
        if (ptr_acquired_pair != NULL) {
            ptr_acquired_object = ptr_acquired_pair->ptr_data;
            ret = abel_free_object_ptr(ptr_acquired_object);
            abel_free_pair(ptr_acquired_pair);
        }
        ptr_linked_list_head = ptr_dict->ptr_map->ptr_coll_vector->ptr_array[i];
        /* free collision array */
        if (ptr_linked_list_head != NULL) {
            free_linked_list(ptr_linked_list_head);
        }
    }
    abel_free_vector_ptr(ptr_dict->ptr_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_dict->ptr_map->ptr_coll_vector);
    free(ptr_dict->ptr_map);
    free(ptr_dict);
    return ret;
}

struct abel_return_option abel_free_object_ptr(struct abel_object* ptr_object)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    if (ptr_object->ref_count == 1) {
        if (ptr_object->data_type == LIST_TYPE) {
            ret = abel_free_list_ptr(ptr_object->ptr_data);
        } else if (ptr_object->data_type == DICT_TYPE) {
            ret = abel_free_dict_ptr(ptr_object->ptr_data);
        } else {
            free(ptr_object->ptr_data);    // free stored data
        }
        free(ptr_object);    // make sure to free object
    } else {
        /* shared pointer, reduce the ref count only */
        ptr_object->ref_count -= 1;
        ret = abel_option_okay(NULL);
    }
    return ret;
}