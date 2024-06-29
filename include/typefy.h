/**
 * Header typefy.h
 * Created 21-10-2022
 * Updated 25-11-2022
 *
 * Declarations of types used in Abel.
 **/
#ifndef ABEL_ON_C_TYPEFY_H
#define ABEL_ON_C_TYPEFY_H

#include <ctype.h>
#include "common.h"

/**
 * @brief Type Null
 *
 * Type Null is defined as an enum that represents the
 * type `null` used in JSON. The exact interpretation
 * of such type is left to the user of the library.
 */
typedef enum abel_null_type { null } Null;

/**
 * @brief Data type
 *
 * An enum of all types used in Abel.
 **/
enum data_type {
    OBJECT_TYPE,  //0
    BOOL_TYPE,    //1
    NULL_TYPE,    //2
    STRING_TYPE,  //3
    INTEGER_TYPE,  //4
    DOUBLE_TYPE,   //5
    COMPLEX_TYPE,  //6
    VECTOR_TYPE,   //7
    LIST_TYPE,    //8
    DICT_TYPE    //9
};

/**
 * Data type string
 *
 * An array of strings that stores the string representation
 * for each variant in the data types above.
 **/
extern char* DATA_TYPE_STRING[];

/**
 * @brief Complex struct
 *
 * Abel library provides a complex struct to represent complex
 * number. Both real and imaginary parts are both double.
 * 
 * Fields
 * 
 * real : A double type number representing the real part
 * 
 * imag : A double type number representing the real part
 * 
 * @todo No implementation yet. Move this into generic.
 */
struct abel_complex {
    double real;
    double imag;
};
typedef struct abel_complex Complex;

/**
 * Object struct
 *
 * Object is a wrapper object. It stores a pointer to the
 * original data, the size of the original data it wraps,
 * the type of the original data, and a reference counter.
 *
 * This wrapper is used internally to provide homogeneous
 * data type accepted by the Abel containers, i.e. list
 * and dictionary. As containers are dynamically allocated
 * on heap, pointers to Objects - instead of the instance -
 * are used extensively in Abel to provide a systematic way
 * of resource management.
 * 
 * The reference counter, in particular, contributes to the
 * resource management. It registers the number of containers
 * which reference the same object and is thus providing an
 * indicator for unique or shared ownership. Note that only
 * containers can instruct how the ref count is adjusted.
 * With this ref count, Object wrapper behaves as a simplified
 * smart pointer. I have no intention to (re)invent a full
 * (re)implementation of smart pointer in C. Object wrapper
 * provides a restricted, but fairly sufficient, mechanism
 * to guarantee the library is free of memory leak.
 *
 * Fields
 *
 * ptr_data : Pointer (`void` type) to the original data.
 *     It is chosen to be `void*` to accept any type,
 *     including container types such as list and dict
 *
 * data_size : Size of original data type. For example, if
 *     the original data is `int`, the corresponding data
 *     size is given by `sizeof( (int)var )`
 *
 * data_type : Data type of the original data. It must be
 *     a variant in enum `DataType`
 * 
 * ref_count : Number of references made by containers
 *
 * Caution
 *
 * When used in Abel containers, please ensure the Object
 * and the data it holds are BOTH created on heap.
 * 
 * @see object.h
 */
struct abel_object {
    void* ptr_data;
    enum data_type data_type;
    size_t data_size;
    size_t ref_count;
};
//typedef struct abel_object Object;
//typedef struct abel_object* object_ptr;

/**
 * List struct
 *
 * List is a container type. It wraps a Vector that is
 * dynamically alocated on heap. The list container used
 * in Abel has data type set to `OBJECT_TYPE`, meaning that
 * all pointers are pointing to Object instances.
 *
 * Fields
 *
 * ptr_vector : Pointer to internal vector allocated on heap
 *   
 * data_type : Data type of the objects that are pointed to
 *     by the pointers stored in vector. This field is used
 *     mostly to cast a void pointer to its designated type
 *   
 * @see list.h
 */
struct abel_list {
    struct abel_vector* ptr_vector;
    enum data_type data_type;
};
//typedef struct abel_list List;
//typedef struct abel_list* list_ptr;

/**
 * @brief Dictionary struct
 *
 * Abel dictionary is a wrapper struct of an internal map.
 * It carries a field `DataType data_type` to register the
 * type of data associated with the key.
 *
 * Fields
 *
 * ptr_map : Pointer to internal map allocated on heap
 *   
 * data_type : Data type of the values that are associated
 *     with the keys. Its primary use is casting a void pointer
 *     to its designated type.
 *  
 * @see dict.h
 **/
struct abel_dict {
    struct abel_map* ptr_map;
    enum data_type data_type;
};
//typedef struct abel_dict Dict;
//typedef struct abel_dict* dict_ptr;

#endif