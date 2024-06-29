/* Source vector.c */
#include "vector.h"

/**
 * @brief Static - Compute capacity for vector
 * 
 * Given the size of a vector, capacity is computed as
 * the next higher power of 2. However, the minimal
 * value of the capacity is 2, if the size of the vector
 * is no higher than 2. Keeping 2 as the minimum avoids
 * the unecessary re-allocation in the case of removing
 * the last element from a vector of 2.
 * 
 * @param size Intended size of the vector
 * @return An unsigned int that is the capacity of the
 *         vector.
 */
static size_t vector_capacity_from_size(size_t size)
{
    size_t capacity = 2;
    if (size > 2) {
        capacity = next_power_of_two(size);
    }
    return capacity;
}

/* Maker */

struct abel_vector abel_make_vector(size_t size)
{
    struct abel_vector vector;
    void* ptr_calloced = NULL;
    if (size <= 2) {
        vector.capacity = 2;    
    } else {
        vector.capacity = next_power_of_two(size);
    }
    vector.size = size;
    ptr_calloced = calloc( vector.capacity, sizeof(ptr_calloced) );
    if (ptr_calloced != NULL) {
        vector.ptr_array = ptr_calloced;
    } else {
        vector.ptr_array = NULL;
    }
    return vector;
}

void abel_free_vector(struct abel_vector* ptr_vec)
{
    free(ptr_vec->ptr_array);
}

struct abel_vector* abel_make_vector_ptr(size_t size)
{
    struct abel_vector* ptr_vector = malloc( sizeof(*ptr_vector) );
    void* ptr_calloced = NULL;
    size_t capacity = 0;
    if (size <= 2) {
        capacity = 2;    
    } else {
        capacity = next_power_of_two(size);
    }
    if (ptr_vector != NULL) {
        ptr_calloced = calloc( capacity, sizeof(ptr_calloced) );
        if (ptr_calloced != NULL) {
            ptr_vector->capacity = capacity;
            ptr_vector->size = size;
            ptr_vector->ptr_array = ptr_calloced;
        } else {
            ptr_vector = NULL;
        }
    } else {
        ptr_vector = NULL;
    }
    return ptr_vector;
}

/* Freer */

void abel_free_vector_ptr(struct abel_vector* ptr_vec)
{
    free(ptr_vec->ptr_array);
    free(ptr_vec);
}

/* Checker */

size_t abel_vector_size(struct abel_vector* ptr_vec)
{
    return ptr_vec->size;
}

size_t abel_vector_capacity(struct abel_vector* ptr_vec)
{
    return ptr_vec->capacity;
}

Bool abel_vector_is_empty(struct abel_vector* ptr_vec)
{
    Bool ret = true;
    if (ptr_vec->size > 0) {
        ret = false;
    }
    return ret;
}

/* Append */

struct abel_return_option abel_vector_append(struct abel_vector* ptr_vec,
                                             void* ptr_src)
{
    struct abel_return_option ret;
    if (ptr_vec->capacity == 2 && ptr_vec->size == 0) {
        /* minmal capacity vector */
        ptr_vec->ptr_array[0] = ptr_src;
        ptr_vec->size += 1;
        ret = abel_option_okay(NULL);
    } else if (ptr_vec->capacity == 2 && ptr_vec->size == 1) {
        ptr_vec->ptr_array[1] = ptr_src;
        ptr_vec->size += 1;
        ret = abel_option_okay(NULL);
    } else {
        if (ptr_vec->size == ptr_vec->capacity) {
            /* compute new capacity */
            ptr_vec->capacity = vector_capacity_from_size(ptr_vec->size + 1);
            ptr_vec->ptr_array = realloc( ptr_vec->ptr_array,
                    ptr_vec->capacity * sizeof(*ptr_vec->ptr_array) );
        }
        if (ptr_vec->ptr_array != NULL) {
            ptr_vec->ptr_array[ptr_vec->size] = ptr_src;
            ptr_vec->size += 1;
            ret = abel_option_okay(NULL);
        } else {
            ret = abel_option_error( error_realloc_failure() );
        }
    }
    return ret;
}

struct abel_return_option abel_vector_push_back(struct abel_vector* ptr_vec,
                                                void* ptr)
{
    return abel_vector_append(ptr_vec, ptr);
}

/* Emplace */

struct abel_return_option abel_vector_emplace(struct abel_vector* ptr_vec,
                                              size_t idx, void* ptr_src)
{
    struct abel_return_option ret;
    if (idx >= ptr_vec->size) {
        ret = abel_option_error( error_out_of_range() );
    } else {
        ret = abel_option_okay(ptr_vec->ptr_array[idx]);
        ptr_vec->ptr_array[idx] = ptr_src;
    }
    return ret;
}

struct abel_return_option abel_vector_set(struct abel_vector* ptr_vec,
                                          size_t idx, void* ptr_src)
{
    return abel_vector_emplace(ptr_vec, idx, ptr_src);
}

/* Insert */

struct abel_return_option abel_vector_insert(struct abel_vector* ptr_vec,
                                             size_t idx, void* ptr_src)
{
    struct abel_return_option ret;
    size_t size_of_mem = 0;
    if (idx == ptr_vec->size) {    /* effectively append */
        return abel_vector_append(ptr_vec, ptr_src);
    } else if (ptr_vec->capacity == 2 && ptr_vec->size == 1 && idx == 0) {
        /* special case: minial capacity, only index 0 taken, insert at 0 */
        /* in this case, there is no need to move memory */
        ptr_vec->ptr_array[1] = ptr_vec->ptr_array[0];
        ptr_vec->ptr_array[0] = ptr_src;
        ptr_vec->size += 1;
        ret = abel_option_okay(NULL);
        return ret;
    } else if (idx < ptr_vec->size) {    // insert
        /* check if realloc is needed */
        if (ptr_vec->size + 1 > ptr_vec->capacity) {    // need realloc
            /* compute new capacity and re-alloc */
            ptr_vec->capacity = vector_capacity_from_size(ptr_vec->size + 1);
            ptr_vec->ptr_array = realloc( ptr_vec->ptr_array,
                    ptr_vec->capacity * sizeof(*ptr_vec->ptr_array) );
            if (ptr_vec->ptr_array == NULL) {    // realloc failure
                ret = abel_option_error( error_realloc_failure() );
                return ret;
            }
        }
        /* compute the size of mem to be moved */
        size_of_mem = (ptr_vec->size - idx) * sizeof(*ptr_vec->ptr_array);
        /* move the block */
        memmove(ptr_vec->ptr_array + idx + 1,
                ptr_vec->ptr_array + idx, size_of_mem);
        ptr_vec->size += 1;
        /* place the new */
        ptr_vec->ptr_array[idx] = ptr_src;
        ret = abel_option_okay(NULL);
        return ret;
    } else {    // index is out of range
        ret = abel_option_error( error_out_of_range() );
        return ret;
    }
}

/* Getter */

struct abel_return_option abel_vector_get(const struct abel_vector* ptr_vec, size_t idx)
{
    struct abel_return_option ret;
    if (ptr_vec->size == 0 && idx == 0) {    // get from an empty vector
        ret = abel_option_error( error_out_of_range() );
    } else {
        if (idx >= ptr_vec->size) {
            ret = abel_option_error( error_out_of_range() );
        } else {
            ret = abel_option_okay(ptr_vec->ptr_array[idx]);
        }
    }
    return ret;
}

struct abel_return_option abel_vector_at(struct abel_vector* ptr_vec, size_t idx)
{
    return abel_vector_get(ptr_vec, idx);
}

struct abel_return_option abel_vector_back(const struct abel_vector* ptr_vec)
{
    struct abel_return_option ret;
    if (ptr_vec->size == 0) {
        ret = abel_option_error( error_out_of_range() );
    } else {
        ret = abel_vector_get(ptr_vec, ptr_vec->size - 1);
    }
    return ret;
}

struct abel_return_option abel_vector_front(struct abel_vector* ptr_vec)
{
    struct abel_return_option ret;
    if (ptr_vec->size == 0) {
        ret = abel_option_error( error_out_of_range() );
    } else {
        ret = abel_vector_get(ptr_vec, 0);
    }
    return ret;
}

/* Delete */

struct abel_return_option abel_vector_delete(struct abel_vector* ptr_vec,
                                             size_t idx)
{
    struct abel_return_option ret;
    size_t size_of_mem = 0;
    size_t new_capacity;
    void* ptr_previous_occupant = NULL;
    void* ptr_realloced = NULL;
    if (idx >= ptr_vec->size || ptr_vec->size == 0) {
        /* to delete, index < size or non-empty vector */
        ret = abel_option_error( error_out_of_range() );
    } else if (ptr_vec->capacity ==2) {    // minimal capacity vector
        if (ptr_vec->size == 2) {    // size is 2
            if (idx == 0) {
                /* return and delete front */
                ret = abel_option_okay(ptr_vec->ptr_array[0]);
                ptr_vec->ptr_array[0] = ptr_vec->ptr_array[1];
                ptr_vec->ptr_array[1] = NULL;
            } else {
                /* idx can only be 1, return it and set it to NULL */
                ret = abel_option_okay(ptr_vec->ptr_array[1]);
                ptr_vec->ptr_array[1] = NULL;
            }
        } else {
            /* size must be 1, idx can only be 0, result is an empty vector */
            ret = abel_option_okay(ptr_vec->ptr_array[0]);
            ptr_vec->ptr_array[0] = NULL;
        }
        ptr_vec->size -= 1;
    } else {    /* non-minimal capacity */
        ptr_previous_occupant = ptr_vec->ptr_array[idx];
        /* compute the total amount of memory to be moved */
        size_of_mem = (ptr_vec->capacity - idx - 1) * sizeof(*ptr_vec->ptr_array);
        /* desination is at idx, source is idx+1 */
        memmove(ptr_vec->ptr_array + idx,
                ptr_vec->ptr_array + idx + 1, size_of_mem);
        /* compute new capacity using new size */
        new_capacity = vector_capacity_from_size(ptr_vec->size - 1);
        /* if new capacity is smaller, then relocate */
        if (new_capacity < ptr_vec->capacity) {
            ptr_realloced = realloc( ptr_vec->ptr_array,
                    new_capacity * sizeof(*ptr_vec->ptr_array) );
            if (ptr_realloced != NULL) {
                ptr_vec->ptr_array = ptr_realloced;
                ptr_vec->capacity = new_capacity;
                ptr_vec->size -= 1;
                ret = abel_option_okay(ptr_previous_occupant);
            } else {
                ret = abel_option_error( error_realloc_failure() );
            }
        } else {
            ptr_vec->size -= 1;
            ret = abel_option_okay(ptr_previous_occupant);
        }
    }
    return ret;
}

struct abel_return_option abel_vector_erase(struct abel_vector* ptr_vec,
                                            size_t idx)
{
    return abel_vector_delete(ptr_vec, idx);
}

struct abel_return_option abel_vector_pop_back(struct abel_vector* ptr_vec)
{
    struct abel_return_option ret;
    if (ptr_vec->size == 0) {    // empty vector
        ret = abel_option_error( error_out_of_range() );
    } else {
        ret = abel_vector_erase(ptr_vec, ptr_vec->size - 1);
    }
    return ret;
}

struct abel_return_option abel_vector_pop_front(struct abel_vector* ptr_vec)
{
    struct abel_return_option ret;
    if (ptr_vec->size == 0) {    // empty vector
        ret = abel_option_error( error_out_of_range() );
    } else {
        ret = abel_vector_erase(ptr_vec, 0);
    }
    return ret;
}