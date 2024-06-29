/**
 * Header list.h
 * Created 21-10-2022
 * Updated 28-11-2022
 * 
 * struct abel_list is a wrapper object over a vector that is its
 * internal storage. struct abel_list has a field `data_type` that
 * is set to Object to specify the data stored in list.
 * 
 * Appending an object to a list increases the ref count
 * of that object by 1. Deleting an object is removing it
 * from the vector's array of pointers and the object is
 * further passed to Object freer, i.e. de facto garbage
 * collector.
 * 
 * @note
 * 
 * struct abel_list freer `abel_free_list_ptr(struct abel_list* ptr_list)`
 * is defined in `object.h` together with other container
 * and object freers.
 **/
#ifndef ABEL_ON_C_LIST_H
#define ABEL_ON_C_LIST_H

#include "object.h"

/* Maker */

/**
 * @brief struct abel_list-instance maker
 * 
 * The internal vector is created on heap. In case of memory
 * allocation failure, a void pointer NULL is assgined to the
 * internal vector. Field `data_type` is set to OBJECT_TYPE
 * 
 * @param size Number of elements the list stores. If its
 *        value is less than 2, an empty list is created
 *        with capacity 2. See vector.
 * @return A list instance
 */
struct abel_list abel_make_list(size_t size);

/**
 * @brief On-heap list maker
 * 
 * Makes a struct abel_list instance on heap.
 *
 * @param size Number of elements the list stores. If its
 *        value is no greater than 2, an empty list
 *        is created with capacity 2. See vector.
 * @return A pointer to the list object newly created on heap.
 * @note Should malloc fail, return NULL.
 **/
struct abel_list* abel_make_list_ptr(size_t size);

/* Checker */

/**
 * @brief Size, capacity, empty, valid index checkers
 *
 * Returns the size or capacity of a list.
 */
size_t abel_list_size(struct abel_list* ptr_list);
size_t abel_list_capacity(struct abel_list* ptr_list);

Bool abel_list_is_empty(struct abel_list* ptr_list);
Bool abel_list_is_valid_index(struct abel_list* ptr_list, size_t idx);

/**
 * @brief struct abel_list append
 * 
 * The public interface is mostly about appending the
 * primitive and instrinsic types to a list.
 * 
 * In all append function, the actual action to append
 * is delegated to the append function of Vector.
 * 
 * @param ptr_list : Pointer to the target list
 * @param src_data / ptr_src_data: Source data or the
 *        pointer to the source data to be appended to
 *        the struct abel_list
 * @return struct abel_return_option instance. If success, flag is_okay is true
 *         and pointer is NULL. If failure, is_error is true
 *         and error contains the error.
 */

/* Bool */
struct abel_return_option abel_list_append_bool_ptr(
        struct abel_list* ptr_list, Bool* ptr_src_data);
struct abel_return_option abel_list_append_bool(
        struct abel_list* ptr_list, Bool src_data);
/* Null */
struct abel_return_option abel_list_append_null_ptr(
        struct abel_list* ptr_list, Null* ptr_src_data);
struct abel_return_option abel_list_append_null(
        struct abel_list* ptr_list, Null src_data);
/* string */
struct abel_return_option abel_list_append_string(
        struct abel_list* ptr_list, char* src_str);
/* int */
struct abel_return_option abel_list_append_int_ptr(
        struct abel_list* ptr_list, int* ptr_src_data);
struct abel_return_option abel_list_append_int(
        struct abel_list* ptr_list, int src_data);
/* double */
struct abel_return_option abel_list_append_double_ptr(
        struct abel_list* ptr_list, double* ptr_src_data);
struct abel_return_option abel_list_append_double(
        struct abel_list* ptr_list, double src_data);
/* struct abel_list pointer */
struct abel_return_option abel_list_append_list_ptr(
        struct abel_list* ptr_list, struct abel_list* ptr_src_list);
/* dict pointer */
struct abel_return_option abel_list_append_dict_ptr(
        struct abel_list* ptr_list, struct abel_dict* ptr_src_dict);

/**
 * @brief Generic append method
 * 
 * @note Using this method to directly append a rvalue
 *       of Bool, such as `abel_list_append(..., true)`
 *       will result in integer data type.
 */
#ifndef abel_list_append

#define abel_list_append(ptr_list, src) \
    _Generic(src, \
        Bool : abel_list_append_bool, \
        Bool* : abel_list_append_bool_ptr, \
        Null : abel_list_append_null, \
        Null* : abel_list_append_null_ptr, \
        char* : abel_list_append_string, \
        int : abel_list_append_int, \
        int* : abel_list_append_int_ptr, \
        double : abel_list_append_double, \
        double* : abel_list_append_double_ptr, \
        struct abel_list* : abel_list_append_list_ptr, \
        struct abel_dict* : abel_list_append_dict_ptr \
    )(ptr_list, src)

#endif

/**
 * @brief Setter
 * 
 * Setter places an object at the given index.
 * 
 * @param ptr_list Pointer to the target list
 * @param src_data(ptr_src_data) Source data or the pointer
 *        to the source data to be appended to the struct abel_list.
 * @return struct abel_return_option instance.
 *         - If success, flag is_okay is true and pointer is
 *           set to NULL.
 *         - If failure, flag is_error is true and error is
 *           returned in the option.
 */

/* Bool */
struct abel_return_option abel_list_set_bool_ptr(
        struct abel_list* ptr_list, size_t idx, Bool* ptr_src_data);
struct abel_return_option abel_list_set_bool(
        struct abel_list* ptr_list, size_t idx, Bool src_data);

/* Null */
struct abel_return_option abel_list_set_null_ptr(
        struct abel_list* ptr_list, size_t idx, Null* ptr_src_data);
struct abel_return_option abel_list_set_null(
        struct abel_list* ptr_list, size_t idx, Null src_data);

/* string */
struct abel_return_option abel_list_set_string(
        struct abel_list* ptr_list, size_t idx, char* src_str);

/* int */
struct abel_return_option abel_list_set_int_ptr(
        struct abel_list* ptr_list, size_t idx, int* ptr_src_data);
struct abel_return_option abel_list_set_int(
        struct abel_list* ptr_list, size_t idx, int src_data);

/* double */
struct abel_return_option abel_list_set_double_ptr(
        struct abel_list* ptr_list, size_t idx, double* ptr_src_data);
struct abel_return_option abel_list_set_double(
        struct abel_list* ptr_list, size_t idx, double src_data);

/* struct abel_list pointer */
struct abel_return_option abel_list_set_list_ptr(
        struct abel_list* ptr_list, size_t idx, struct abel_list* ptr_src_list);

/* struct abel_dict pointer */
struct abel_return_option abel_list_set_dict_ptr(
        struct abel_list* ptr_list, size_t idx, struct abel_dict* ptr_src_dict);

/**
 * @brief Generic set
 * 
 * @note Using this method to directly set a rvalue
 *       of Bool, such as `abel_list_set(..., true)`
 *       will result in an integer data type. To avoid
 *       this, use forced data conversion such as
 *       `abel_list_set(..., (Bool)true)`
 */
#ifndef abel_list_set

#define abel_list_set(ptr_list, idx, src) \
    _Generic(src, \
        Bool : abel_list_set_bool, \
        Bool* : abel_list_set_bool_ptr, \
        Null : abel_list_set_null, \
        Null* : abel_list_set_null_ptr, \
        char* : abel_list_set_string, \
        int : abel_list_set_int, \
        int* : abel_list_set_int_ptr, \
        double : abel_list_set_double, \
        double* : abel_list_set_double_ptr, \
        struct abel_list* : abel_list_set_list_ptr, \
        struct abel_dict* : abel_list_set_dict_ptr \
    )(ptr_list, idx, src)

#endif

/* Getter */

/**
 * @brief Get an element from list
 *
 * Since a list stores an array of object pointers, this
 * is the most basic getter method. It calls get method
 * on the internal vector.
 *
 * @param ptr_list : Pointer to the source list from which
 *                   an element is requested.
 * @param index : Index on the list.
 * @return Should the index is out of range, a NULL is
 *         returned.
 * @todo No, the return must be polished. Better return an
 *       option or result.
 */
struct abel_object* abel_list_get_object_pointer(
        struct abel_list* ptr_list, size_t index);

/**
 * @brief Get data type of the stored object
 * 
 * As everything stored in list is wrapped in Object, it
 * is very often necessary to enquire the data type before
 * selecting the matching getter.
 * 
 * @param ptr_list Pointer to the source list.
 * @param index Index of the element to be accessed.
 * @return Data type of the store object.
 * 
 * @todo How to return error? Such as out of range?
 */
enum data_type abel_list_get_data_type(struct abel_list* ptr_list, size_t index);

/**
 * Terminal-data getters
 *
 * It calls `abel_object_get_[type]` function to convert
 * the object pointer (acquired from a list) into the
 * desired type.
 * 
 * Caution
 * 
 * Validate the index before calling these getters.
 * 
 * @note Get dict_ptr is in container.h
 */
Bool abel_list_get_bool(struct abel_list* ptr_list, size_t index);
Null abel_list_get_null(struct abel_list* ptr_list, size_t index);
int abel_list_get_int(struct abel_list* ptr_list, size_t index);
double abel_list_get_double(struct abel_list* ptr_list, size_t index);

/*
 * Generic
 *
 * The last argument must be an expression of the desired
 * type.
 *
 * Caution
 * 
 * Validate the index before calling these getters, as they
 * don't report error.
 */
#ifndef abel_list_get

#define abel_list_get(ptr_list, idx, type_flag) \
    _Generic(type_flag, \
        Bool : abel_list_get_bool, \
        Null : abel_list_get_null, \
        int : abel_list_get_int, \
        double : abel_list_get_double \
    )(ptr_list, idx)
#endif

/**
 * @brief Delete an element from list
 * 
 * It calls Vector's delete function which, per valid index,
 * removes the element from array and returns it in option.
 * The pointer to object stored in the option is further
 * passed to `abel_free_object_ptr` rubbish collector.
 * 
 * @param ptr_list : Pointer to the list from which the
 *                   indexed element is to be deleted.
 * @param idx : Index of the item to be deleted.
 * @return An struct abel_return_option instance.
 *         - If success, flag is_okay is true and pointer is
 *           set to NULL.
 *         - If failure, flag is_errror is true and error is
 *           stored in the option.
 * @todo Polish the returned option. This must be done from
 *       Object.
 */
struct abel_return_option abel_list_delete(struct abel_list* ptr_list, size_t idx);

#endif