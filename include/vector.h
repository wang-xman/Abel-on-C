/**
 * Header vector.h
 * 
 * General-purpose vector stored heterogenerous content.
 * 
 * Vector has its internal array allocated on heap and
 * is thus holding resource which must be freed.
 * 
 * Pointer of type `vector_ptr` is always pointing to a
 * vector created on heap.
 * 
 * @todo When construct, allocate, reallocate, check the
 *       capacity against the MAX_CAPACITY. Better define
 *       this value in typefy.
 * 
 * Functions
 * 
 * - Maker
 *     Vector abel_make_vector(size_t size);
 *     vector_ptr abel_make_vector_ptr(size_t size);
 * 
 * - Freer
 *     void abel_free_vector(Vector* ptr_vec);
 *     void abel_free_vector_ptr(vector_ptr ptr_vec);
 * 
 * - Checker
 *     size_t abel_vector_size(Vector* ptr_vec);
 *     size_t abel_vector_capacity(Vector* ptr_vec);
 *     Bool abel_vector_is_empty(Vector* ptr_vec);
 * 
 * - Append
 *     struct abel_return_option abel_vector_append(Vector* ptr_vec, void* ptr_src);
 *     struct abel_return_option abel_vector_push_back(Vector* ptr_vec, void* ptr_src);
 *     
 * - Insert
 *     struct abel_return_option abel_vector_insert(Vector* ptr_vec, size_t idx, void* ptr_src);
 * 
 * - Emplace : Returns the previous occupant
 *     struct abel_return_option abel_vector_emplace(Vector* ptr_vec, size_t idx, void* ptr_src);
 *     struct abel_return_option abel_vector_set(Vector* ptr_vec, size_t idx, void* ptr_src);
 * 
 * - Getter
 *     struct abel_return_option abel_vector_get(Vector* ptr_vec, size_t idx);
 *     struct abel_return_option abel_vector_at(Vector* ptr_vec, size_t idx);
 *     struct abel_return_option abel_vector_back(Vector* ptr_vec);
 *     struct abel_return_option abel_vector_front(Vector* ptr_vec);
 * 
 * - Delete : Returns the previous occupant
 *     struct abel_return_option abel_vector_delete(Vector* ptr_vec, size_t idx);
 *     struct abel_return_option abel_vector_erase(Vector* ptr_vec, size_t idx);
 *     struct abel_return_option abel_vector_pop_back(Vector* ptr_vec);
 *     struct abel_return_option abel_vector_pop_front(Vector* ptr_vec);
 */
#ifndef ABEL_ON_C_VECTOR_H
#define ABEL_ON_C_VECTOR_H

#include "option.h"

/**
 * @brief General-purpose vector
 *
 * Abel vector is a dynamically allocated structure. It
 * stores on heap an array of void pointers that point to
 * the desired data. In principle, it may store any type of
 * data.
 * 
 * Note that vector is considered as resource holding.
 * 
 * Fields
 * 
 * ptr_array : Pointer to an array created on heap. Size of
 *             this array is given by field `capacity`.
 *
 * size : Total number of elements that the array is supposed
 *        to store. This defines the range of index.
 * 
 * capacity : The capacity before reallocate the memory.
 *            This is not always identical to the size of the
 *            vector and it is monitored by the vector.
 * 
 * @note Vector is not responsible for the resources which it
 *       is pointing at. Resource management of such data must
 *       be done by caller.
 */
struct abel_vector {
    void** ptr_array;
    size_t size;
    size_t capacity;
};

/**
 * @brief Vector instance maker
 *
 * Size of the vector is given by the argument and it can
 * be 0. However, the capacity is the next highest power
 * of 2 of the given size. The minimal capacity is 2.
 * 
 * Internal array stores void pointers which are initialised
 * to NULL.
 * 
 * @param size Total number of elements to be stored
 *               in the newly created vector.
 * @return A vector instance.
 * @note Should calloc fail, the internal array is NULL.
 */
struct abel_vector abel_make_vector(size_t size);

/**
 * @brief Free vector's internal array
 * 
 * Use this to free vector created on stack.
 */
void abel_free_vector(struct abel_vector* ptr_vec);

/**
 * @brief Vector pointer maker
 *
 * Vector is created on heap and the pointer pointing at it
 * is returned.
 * 
 * @param size Total number of elements to be stored in
 *             the newly created vector.
 * @return Pointer to a vector instance created on heap.
 *         Should memory allocation fail, return is NULL.
 */
struct abel_vector* abel_make_vector_ptr(size_t size);

/**
 * @brief Free an on-heap vector
 * 
 * @note Vector is not responsible for the resources the
 *       elements in the internal array are pointing to.
 */
void abel_free_vector_ptr(struct abel_vector* ptr_vec);

/**
 * @brief Size of a vector
 * 
 * Size of a vector is the total number of "seats" that can
 * be taken, not the capacity. Zero sized vector returns
 * of course 0.
 *  
 * @param ptr_vec Pointer to the vector of which the size
 *                is requested.
 */
size_t abel_vector_size(struct abel_vector* ptr_vec);

/**
 * @brief Capacity of a vector
 * 
 * Capacity is the number of elements the vector can hold
 * before re-allocation is required.
 *  
 * @param ptr_vec Pointer to the vector of which the size
 *                is requested.
 */
size_t abel_vector_capacity(struct abel_vector* ptr_vec);

/**
 * @brief Check if vector is empty
 * 
 * @param ptr_vec Pointer to the vector.
 */
Bool abel_vector_is_empty(struct abel_vector* ptr_vec);

/**
 * @brief Vector append
 *
 * Appends an element at the end of the destination vector.
 * Note that appending increases the size of the vector.
 * 
 * Should the size reach the maxcap, memory reallocation is
 * thus required.
 * 
 * @param ptr_vec Pointer to the destination vector to
 *                which the source pointer is appended.
 * @param ptr_src Source pointer to be appended to the
 *                destination vector.
 * @return Option instance. If success, is_okay is true and
 *         pointer is NULL. If failure, is_error is true and
 *         error contains the error.
 */
struct abel_return_option abel_vector_append(struct abel_vector* ptr_vec,
                                             void* ptr_src);

/**
 * @brief Vector push back
 * 
 * Functionality, call signature, and return are identical
 * to append. Sometimes user may prefer this name.
 */
struct abel_return_option abel_vector_push_back(struct abel_vector* ptr_vec,
                                                void* ptr_src);

/**
 * @brief Emplace a value at given position
 * 
 * If index is valid, value at the given valid index is
 * replaced by the argument `ptr_src` and the previous value
 * is returned in struct abel_return_option.
 * Otherwise, error is returned in option.
 * 
 * Caution that emplace a value on an empty vector results
 * in error, use `abel_vector_append` or `abel_vector_push_back`. 
 * 
 * @param ptr_vec Pointer to the vector on which the value
 *                is placed.
 * @param idx Index on the vector.
 * @param ptr_src Source pointer to be placed in the vector
 *                at the given index.
 * @return Option instance.
 *         - If success, flag `is_okay` is `true` and `pointer`
 *           stores the PREVIOUS occupant at the given index.
 *         - If failure, flag `is_error` is `true` and `error`
 *           stores the error instance.
 * @note Caution. It is the caller's responsibility to manage
 *       the returned resource held by the previous occupant.
 */
struct abel_return_option abel_vector_emplace(struct abel_vector* ptr_vec,
                                              size_t idx, void* ptr_src);

/**
 * @brief Set a value at given index
 * @note Identical and an alias to `abel_vector_emplace`
 */
struct abel_return_option abel_vector_set(struct abel_vector* ptr_vec,
                                          size_t idx, void* ptr_src);

/**
 * @brief Insert at a given index
 *
 * Inserting a new element before the element at specified
 * position and the size of the vector is thus inrease by 1.
 * 
 * Index must not greater than the size of the vector.
 * If the given index equals the size, insertion is thus
 * identical to appending an element at the end, including
 * the scenario that inserting into an empty array at 0.
 * 
 * Example
 * 
 * Consider a vector ["Jane", "Alan", "Henry"]. Inserting
 * "Mike" at index 1 results in
 *     ["Jane", "Mike", "Alan", "Henry"]
 * 
 * Index for insertion must be within the range of valid
 * indices.
 * 
 * @param ptr_vec Target vector to which the element is
 *                inserted.
 * @param idx Index at which the element is inserted.
 * @param ptr_src Element to be inserted into vector.
 * @return struct abel_return_option instance.
 *         - If success, flag `is_okay` is `true` and pointer
 *           is NULL.
 *         - If failure, flag `is_error` is `true` and error
 *           stores the error message.
 */
struct abel_return_option abel_vector_insert(struct abel_vector* ptr_vec,
                                             size_t idx, void* ptr_src);

/**
 * @brief Get a pointer from the internal array
 *
 * Retrieves the pointer stored at given index.
 * 
 * @param ptr_vec Pointer to the vector on which the value
 *                is placed.
 * @param idx Index of the element to be retrieved.
 * @return struct abel_return_option instance.
 *         - Per success, flag is_okay is true and pointer
 *           stores the desired pointer (void).
 *         - Per error, flag is_error is true and error stores
 *           the exact error message. Access to the index 0
 *           of an empty vector result in OUT_OF_RANGE.
 */
struct abel_return_option abel_vector_get(const struct abel_vector* ptr_vec,
                                          size_t idx);

/**
 * @brief Get a pointer from the internal array
 * 
 * Completely identical to `abel_vector_get` function.
 */
struct abel_return_option abel_vector_at(struct abel_vector* ptr_vec,
                                         size_t idx);

/**
 * @brief Retrieves the last element
 * 
 * Retrieves and returns the last element in the vector
 * 
 * @return struct abel_return_option instance.
 *         - Per success, flag is_okay is true and requested
 *           pointer is stored in `pointer` of the union.
 *         - Per failure, flag is_error is true and error is
 *           stored in `error`. If the vector is empty, option
 *           contains `OUT_OF_RANGE` error.
 */
struct abel_return_option abel_vector_back(const struct abel_vector* ptr_vec);

/**
 * @brief Retrieves the first element
 * 
 * Retrieves and returns the first element in the vector
 * 
 * @return struct abel_return_option instance.
 *         - Per success, flag is_okay is true and result is
 *           stored in `pointer` of the union.
 *         - If the vector is empty, option stores `OUT_OF_RANGE`
 *           error.
 */
struct abel_return_option abel_vector_front(struct abel_vector* ptr_vec);

/* Delete */

/**
 * @brief Delete at a given index
 * 
 * If the index is valid, deletes the element at given index
 * and reduces size by 1. The occupant at the given index is
 * returned in option. All subsequent elements will move up.
 * Should a smaller capacity is needed, realloc is performed.
 *
 * @param ptr_vec Target vector from which the element is
 *                deleted.
 * @param idx Index at which the element is deleted. 
 * @return struct abel_return_option instance.
 *         - Per success, flag is_okay is true and pointer
 *           stores the previous occupant at the given index.
 *         - Per error, flag is_error is true and error is
 *           stored in `error`.
 */
struct abel_return_option abel_vector_delete(struct abel_vector* ptr_vec,
                                             size_t idx);

/**
 * @brief Erase the element at given index
 * 
 * Completely identical to `abel_vector_delete`.
 */
struct abel_return_option abel_vector_erase(struct abel_vector* ptr_vec,
                                            size_t idx);

/**
 * @brief Erase the last element
 * 
 * Calls `abel_vector_delete` to delete the last element.
 * 
 * @param ptr_vec Target vector from which the last element
 *                is deleted.
 */
struct abel_return_option abel_vector_pop_back(struct abel_vector* ptr_vec);

/**
 * @brief Erase the first element
 * 
 * Calls `abel_vector_delete` to delete the first element.
 * 
 * @param ptr_vec Target vector from which the first element
 *                is deleted.
 */
struct abel_return_option abel_vector_pop_front(struct abel_vector* ptr_vec);

#endif