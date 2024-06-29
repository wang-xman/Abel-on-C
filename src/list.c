/**
 * Source list.c
 * Created 21-10-2022
 * Updated 28-11-2022
 **/
#include "list.h"

/* Maker */

struct abel_list abel_make_list(size_t size)
{
    struct abel_list list;
    list.ptr_vector = abel_make_vector_ptr(size);
    list.data_type = OBJECT_TYPE;
    return list;
}

struct abel_list* abel_make_list_ptr(size_t size)
{
    struct abel_list* ptr_list = malloc( sizeof(*ptr_list) ); // instance itself
    ptr_list->ptr_vector = abel_make_vector_ptr(size);
    ptr_list->data_type = OBJECT_TYPE;
    return ptr_list;
}

/* Checker */

size_t abel_list_size(struct abel_list* ptr_list)
{
    return ptr_list->ptr_vector->size;
}

size_t abel_list_capacity(struct abel_list* ptr_list)
{
    return ptr_list->ptr_vector->capacity;
}

Bool abel_list_is_empty(struct abel_list* ptr_list)
{
    if (abel_list_size(ptr_list) == 0) {
        return true;
    } else {
        return false;
    }
}

Bool abel_list_is_valid_index(struct abel_list* ptr_list, size_t idx)
{
    if ( idx < abel_list_size(ptr_list) ) {
        return true;
    } else {
        return false;
    }
}

/* struct abel_list append */

/**
 * @brief Static - Append to internal vector
 *
 * This static function calls the append method of
 * Vector. Before appending the object, it increases
 * the ref count of the object by 1.
 * 
 * @note Vector append returns option that contains
 *       either NULL pointer or error.
 */
static struct abel_return_option append_object_ptr_to_list(
        struct abel_list* ptr_list, struct abel_object* ptr_obj)
{
    ptr_obj->ref_count += 1;    /* increase the ref count by 1 */
    return abel_vector_append(ptr_list->ptr_vector, ptr_obj);
}

/* Bool */
struct abel_return_option abel_list_append_bool_ptr(
        struct abel_list* ptr_list, Bool* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_append_bool(
        struct abel_list* ptr_list, Bool src_data)
{
    return abel_list_append_bool_ptr(ptr_list, &src_data);
}

/* Null */
struct abel_return_option abel_list_append_null_ptr(
        struct abel_list* ptr_list, Null* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_append_null(
        struct abel_list* ptr_list, Null src_data)
{
    return abel_list_append_null_ptr(ptr_list, &src_data);
}

/* string */
struct abel_return_option abel_list_append_string(
        struct abel_list* ptr_list, char* src_str)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(src_str);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

/* int */
struct abel_return_option abel_list_append_int_ptr(
        struct abel_list* ptr_list, int* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_append_int(
        struct abel_list* ptr_list, int src_data)
{
    return abel_list_append_int_ptr(ptr_list, &src_data);
}

/* double */
struct abel_return_option abel_list_append_double_ptr(
        struct abel_list* ptr_list, double* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_append_double(
        struct abel_list* ptr_list, double src_data)
{
    return abel_list_append_double_ptr(ptr_list, &src_data);
}

/* struct abel_list pointer */
struct abel_return_option abel_list_append_list_ptr(
        struct abel_list* ptr_list, struct abel_list* ptr_src_list)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    /* Caution. Point to the source list is used directly */
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_list);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

/* struct abel_dict pointer */
struct abel_return_option abel_list_append_dict_ptr(
        struct abel_list* ptr_list, struct abel_dict* ptr_src_dict)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    /* Caution. Point to the source list is used directly */
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_dict);
    ret_option = append_object_ptr_to_list(ptr_list, ptr_object_on_heap);
    return ret_option;
}

/* struct abel_list set */

/**
 * @brief Static - Set object pointer on list
 *
 * Emplaces an object pointer at the given index. It calls
 * Vector's set function to emplace and return the previous
 * occupant. If insertion is successful, it increaes the ref
 * count of the object. Futher if the previous occupant is
 * not NULL, it frees that object by calling object freer.
 * 
 * @param ptr_vec Pointer to the list in which new element
 *        is to be set
 * @param idx Index in the list at which the new element is
 *        set
 * @param ptr_src struct abel_object pointer to be set
 * @return struct abel_return_option instance.
 *         - If success, flag is_okay is true and pointer is
 *           set to NULL.
 *         - If failure, flag is_error is true and error is
 *           returned in the option.
 * @note Vector set function returns the previous occupant at
 *       the given index.
 */
static struct abel_return_option set_object_ptr_on_list(
        struct abel_list* ptr_list, size_t idx, struct abel_object* ptr_src)
{
    struct abel_return_option ret_from_vector;
    ret_from_vector = abel_vector_set(ptr_list->ptr_vector, idx, ptr_src);
    if (ret_from_vector.is_okay == true) {
        /* increase the ref count only if insertion succeeds */
        ptr_src->ref_count += 1;
        /* free previous occupant if any */
        if (ret_from_vector.pointer != NULL) {
            abel_free_object_ptr( (struct abel_object*)ret_from_vector.pointer );
            ret_from_vector.pointer = NULL;
        }
    }
    return ret_from_vector;
}

/* Bool */
struct abel_return_option abel_list_set_bool_ptr(
        struct abel_list* ptr_list, size_t idx, Bool* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_set_bool(
        struct abel_list* ptr_list, size_t idx, Bool src_data)
{
    return abel_list_set_bool_ptr(ptr_list, idx, &src_data);
}

/* Null */
struct abel_return_option abel_list_set_null_ptr(
        struct abel_list* ptr_list, size_t idx, Null* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_set_null(
        struct abel_list* ptr_list, size_t idx, Null src_data)
{
    return abel_list_set_null_ptr(ptr_list, idx, &src_data);
}

/* string */
struct abel_return_option abel_list_set_string(
        struct abel_list* ptr_list, size_t idx, char* src_str)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(src_str);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

/* int */
struct abel_return_option abel_list_set_int_ptr(
        struct abel_list* ptr_list, size_t idx, int* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_set_int(
        struct abel_list* ptr_list, size_t idx, int src_data)
{
    return abel_list_set_int_ptr(ptr_list, idx, &src_data);
}

/* double */
struct abel_return_option abel_list_set_double_ptr(
        struct abel_list* ptr_list, size_t idx, double* ptr_src_data)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_data);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

struct abel_return_option abel_list_set_double(
        struct abel_list* ptr_list, size_t idx, double src_data)
{
    return abel_list_set_double_ptr(ptr_list, idx, &src_data);
}

/* struct abel_list pointer */
struct abel_return_option abel_list_set_list_ptr(
        struct abel_list* ptr_list, size_t idx, struct abel_list* ptr_src_list)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    /* Caution. Point to the source list is used directly */
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_list);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

/* struct abel_dict pointer */
struct abel_return_option abel_list_set_dict_ptr(
        struct abel_list* ptr_list, size_t idx, struct abel_dict* ptr_src_dict)
{
    struct abel_object* ptr_object_on_heap = NULL;
    struct abel_return_option ret_option;
    /* Caution. Point to the source list is used directly */
    ptr_object_on_heap = abel_make_object_ptr(ptr_src_dict);
    ret_option = set_object_ptr_on_list(ptr_list, idx, ptr_object_on_heap);
    return ret_option;
}

/* Getter */

struct abel_object* abel_list_get_object_pointer(struct abel_list* ptr_list, size_t index)
{
    struct abel_return_option return_from_vector;
    return_from_vector = abel_vector_get(ptr_list->ptr_vector, index);
    if (return_from_vector.is_okay == true) {
        return (struct abel_object*)return_from_vector.pointer;
    } else {
        return NULL;
    }
}

enum data_type abel_list_get_data_type(struct abel_list* ptr_list, size_t index)
{
    struct abel_object* ptr_obj = abel_list_get_object_pointer(ptr_list, index);
    return ptr_obj->data_type;
}

Bool abel_list_get_bool(struct abel_list* ptr_list, size_t index)
{
    return abel_object_get_bool(
            abel_list_get_object_pointer(ptr_list, index));
}

Null abel_list_get_null(struct abel_list* ptr_list, size_t index)
{
    return abel_object_get_null(
            abel_list_get_object_pointer(ptr_list, index));
}

int abel_list_get_int(struct abel_list* ptr_list, size_t index)
{
    return abel_object_get_int(
            abel_list_get_object_pointer(ptr_list, index));
}

double abel_list_get_double(struct abel_list* ptr_list, size_t index)
{
    return abel_object_get_double(
            abel_list_get_object_pointer(ptr_list, index));
}

/* Delete */

struct abel_return_option abel_list_delete(struct abel_list* ptr_list, size_t idx)
{
    struct abel_return_option ret;
    struct abel_return_option ret_vector_get;
    if ( abel_list_is_valid_index(ptr_list, idx) ) {
        /* current list is the only owner of the object */
        ret_vector_get = abel_vector_get(ptr_list->ptr_vector, idx);
        if (ret_vector_get.pointer != NULL) {
            abel_free_object_ptr( (struct abel_object*)ret_vector_get.pointer );
            ret = abel_vector_delete(ptr_list->ptr_vector, idx);
        } else {
            ret = ret_vector_get;
        }
    } else {
        ret = abel_option_error( error_out_of_range() );
    }
    return ret;
}