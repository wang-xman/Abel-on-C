/**
 * Header object.h
 * Created 21-10-2022
 * Updated 20-11-2022
 *
 * Object wrapper. Definition of Object is in header "typefy.h".
 * 
 * In this header, I only implement functions operate on
 * pointers to Object, not on Object instances. The reason
 * behind is that, since Object wrapper acts as an inter-
 * mediary between the container and the original data, its
 * usage is largel internal. Furthermore, a stack instance
 * of Object may cause ambiguity in the ownership of the
 * original data.
 *
 * Freer
 * 
 * Since all types are defined in header "typefy.h", this
 * header also provides freer functions used to free memory.
 * The resource release in Object is in general a recursive
 * process.
 * 
 * @todo
 * 1. Return option in freers is not properly configured.
 **/
#ifndef ABEL_ON_C_OBJECT_H
#define ABEL_ON_C_OBJECT_H

#include "typefy.h"    // has list and dict

/*
 * Object-pointer makers
 *
 * These functions perform memory copy of an object to heap
 * and return pointers to the newly allocated memory. It
 * guarantees that Object instance is on heap.
 *
 * Params
 *
 * src_data : Source data of primitive or intrinsic type
 *   
 * Return
 *
 * Returns an Object pointer on heap. In case of allocation
 * failure void pointer `NULL` is returned.
 * 
 * Caution
 * 
 * Should the source data hold resource, it is the user's
 * resposibility to release it.
 */

/* Bool */
struct abel_object* abel_make_object_ptr_from_bool_ptr(Bool* ptr_src_data);
struct abel_object* abel_make_object_ptr_from_bool(Bool src_data);
/* Null */
struct abel_object* abel_make_object_ptr_from_null(Null src_data);
struct abel_object* abel_make_object_ptr_from_null_ptr(Null* ptr_src_data);
/* string */
struct abel_object* abel_make_object_ptr_from_string(char* src_str);
/* int */
struct abel_object* abel_make_object_ptr_from_int_ptr(int* ptr_src_data);
struct abel_object* abel_make_object_ptr_from_int(int src_data);
/* double */
struct abel_object* abel_make_object_ptr_from_double(double src_data);
struct abel_object* abel_make_object_ptr_from_double_ptr(double* ptr_src_data);

/*
 * Object pointer to containers
 *
 * Ensure the source container is created on heap.
 * So, call `abel_make_list_ptr` or `abel_make_dict_ptr`
 * to make the container (pointer).
 */

/* list */
struct abel_object* abel_make_object_ptr_from_list_ptr(
        struct abel_list* ptr_src_data);

/* dict */
struct abel_object* abel_make_object_ptr_from_dict_ptr(
        struct abel_dict* ptr_src_data);

/* Generic */
#ifndef abel_make_object_ptr

/*
 * Preferred object-pointer maker
 *
 * It accepts both data and pointer to data.
 *
 * Generic function to return an Object pointer from primitive
 * C and Abel intrinsic types. Depending on the type of source
 * data, a pointer maker is selected.
 */
#define abel_make_object_ptr(src) \
    _Generic(src, \
        Bool : abel_make_object_ptr_from_bool, \
        Bool* : abel_make_object_ptr_from_bool_ptr, \
        Null : abel_make_object_ptr_from_null, \
        Null* : abel_make_object_ptr_from_null_ptr, \
        char* : abel_make_object_ptr_from_string, \
        int : abel_make_object_ptr_from_int, \
        int* : abel_make_object_ptr_from_int_ptr, \
        double : abel_make_object_ptr_from_double, \
        double* : abel_make_object_ptr_from_double_ptr, \
        struct abel_list* : abel_make_object_ptr_from_list_ptr, \
        struct abel_dict* : abel_make_object_ptr_from_dict_ptr \
    )(src)

#endif

/**
 * @brief Termainal-data getters
 *
 * Returns the data stored in an object pointer
 *
 * @param ptr_obj Pointer to the object from which
 *        the data is acquired
 *
 * @return Returns the data (not pointer) the object
 *         wrapper holds.
 *
 * Caution
 *
 * Since there is no check to guarantee that the data
 * stored is of the desired type. Error can occur.
 */
Bool abel_object_get_bool(struct abel_object* ptr_obj);

Null abel_object_get_null(struct abel_object* ptr_obj);

char* abel_object_get_string(struct abel_object* ptr_obj);

int abel_object_get_int(struct abel_object* ptr_obj);

double abel_object_get_double(struct abel_object* ptr_obj);

/* Generic */
#ifndef abel_object_get_data

/**
 * @brief Generic data getter
 *
 * Use this function to get terminal data, not containers.
 * Terminal data types are
 *     [Bool, Null, int, char*, double]
 * 
 * Depending on the last data type flag, the right data
 * getter is selected.
 *
 * Return
 *
 * Data, not the pointer, of the requested.
 * 
 * Caution
 * 
 * These getters don't check the data type of object pointer.
 * Check the data type consistency before using them.
 */
#define abel_object_get_data(ptr_obj, type) \
    _Generic(type, \
        Bool : abel_object_get_bool, \
        Null : abel_object_get_null, \
        char* : abel_object_get_string, \
        int : abel_object_get_int, \
        double : abel_object_get_double \
    )(ptr_obj)

#endif

/* Get container pointer */
struct abel_list* abel_object_get_list_ptr(struct abel_object* ptr_obj);
struct abel_dict* abel_object_get_dict_ptr(struct abel_object* ptr_obj);

/*
 * Acquire data type from object or pointer to object
 *
 * Params
 *
 * obj/ptr_obj: Object or pointer to object from which the data
 *     type is requested
 */
enum data_type abel_object_get_type(struct abel_object* ptr_obj);


/**
 * @brief Returns type string
 *
 * @param ptr_obj: Object or pointer to object from which
 *                 the data type string is requested.
 */
char* abel_object_get_type_string(struct abel_object* ptr_obj);

/* Freers */

/**
 * @brief Container-pointer freers
 *
 * As containers are also wrapped into an object,
 * their freers must be defined with the object freer.
 * 
 * The internal structure of list and dict is an array.
 * In list it is an array of pointers to objects;
 * in dict it is an array of pointers to dict items
 * which hold pointers to objects on heap.
 * 
 * @note Freers are tested together with containers in
 * container modules list.h and dict.h.
 */

/**
 * @brief List pointer freer
 *
 * It frees the resources held in a list pointed to by the
 * pointer. First, all associated objects are freed. Then
 * the list itself. 
 * 
 * However, if the pointer to list is NULL, for example, in
 * the situation where maxcap is 0, there is no need to free
 * any resource.
 */
struct abel_return_option abel_free_list_ptr(struct abel_list* ptr_list);

/**
 * @brief struct abel_dict pointer freer
 *
 * Frees resource held by a dictionary.
 */
struct abel_return_option abel_free_dict_ptr(struct abel_dict* ptr_dict);

/**
 * @brief Object-pointer freer
 *
 * Object pointer freer is designed as the ultimate rubbish
 * collector. When receiving an object pointer, object freer
 * first check the ref count. If it is 1, the only owner of
 * the object it has instructed the freer to delete it. So,
 * just free it. Should the ref count be higher than 1, there
 * must be other owners - multiple ownership - and the freer
 * will only reduce the ref count by 1.
 *
 * Caution
 * 
 * Always call this function to free object pointer created
 * using `abel_make_object_ptr` function.
 */
struct abel_return_option abel_free_object_ptr(struct abel_object* ptr_object);

#endif