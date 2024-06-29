/**
 * Header map.h
 *
 * General-purpose hash map.
 * 
 * Map consists of two vectors, one for pair registration
 * and the other for collision resolution.
 * 
 * Functions
 * 
 * - Maker
 *     map_ptr abel_make_map_ptr()
 * - Checker
 *     size_t abel_map_size(Map* ptr_map);
 * - Insert
 *     Option abel_map_insert(Map* ptr_map, char* key_str, void* ptr_data);
 * - Getter
 *     Option abel_map_find(Map* ptr_map, char* key_str);
 *     Option abel_map_at(Map* ptr_map, char* key_str);
 * - Assign
 *     Option abel_map_assign(Map* ptr_map, char* key_str, void* ptr_data);
 **/
#ifndef ABEL_ON_C_MAP_H
#define ABEL_ON_C_MAP_H
# include <stdint.h>

#include "vector.h"
#include "linked_list.h"

/**
 * @brief General-purpose (hash) map
 * 
 * Abel map is a hash map that associates a key with a
 * void pointer to any data type, i.e. "key-value pair"
 * or simply "pair".
 * 
 * Fields
 * 
 * ptr_pair_vector : Pointer to a Vector that is used to
 *                   store the key-value pairs.
 * ptr_coll_vector : Pointer to a Vector that is used to
 *                   register keys that collide with the
 *                   existing ones in item vector.
 * size : Total number of pairs stored in the map,
 *        including the ones in both item and collision
 *        vectors.
 * @note Field `size` is not the size of the vector. The
 *       size of the vector is determined internally and
 *       is set to a default value.
 */
struct abel_map {
    struct abel_vector* ptr_pair_vector;
    struct abel_vector* ptr_coll_vector;
    size_t size;
};

/**
 * @brief Key-value pair
 * 
 * Key-value pair is the object that a map stores.
 * The internal vectors of a map store pointers to
 * the pair objects usually created on heap.
 */
struct abel_key_value_pair {
    char* key;
    void* ptr_data;
};

/**
 * @brief Free a pair
 * 
 * When used internally, a pair is always created on heap.
 * Special attention shall be paid to the memory allocated
 * to store the key string.
 * 
 * @param ptr_pair : Pointer to the pair instance to be
 *                   freed.
 * @return Option that contains the pointer pointing at
 *         at the data (value), no matter it is NULL or
 *         not.
 */
struct abel_return_option abel_free_pair(struct abel_key_value_pair* ptr_pair);

/**
 * @brief Make a map on heap
 * 
 * A map instance is created on heap together with
 * its internal vectors. Map size is set to 0, i.e.
 * an empty map.
 * 
 * @return Pointer to the map instance created on heap.
 */
struct abel_map* abel_make_map_ptr();

/**
 * @brief Map size
 * 
 * Map size is the number of pairs stored in it.
 */
size_t abel_map_size(struct abel_map* ptr_map);

/**
 * @brief Insert key-value into map
 * 
 * @param ptr_map : Pointer to the target map.
 * @param key_str : Key in pair to be inserted into map.
 * @param ptr_data : Pointer to the data to be associated
 *                   with the key, i.e the actual value the
 *                   pair is pointing at.
 * @return Option instance.
 *         - Per success, flag is_okay is true and `pointer`
 *           contains the pair that has just been inserted.
 *         - Per error, flag is_error is true and error stores
 *           the specific error.
 */
struct abel_return_option abel_map_insert(
    struct abel_map* ptr_map, char* key_str, void* ptr_data);

/**
 * @brief Find pair which has the given key
 * 
 * Find in the map the pair that has the given key and
 * returns a pointer to that pair. If the key doesn't
 * exist, KEY_NOT_FOUND error is returned.
 * 
 * @param ptr_map : Pointer to the target map
 * @param key_str : Key in pair to be searched
 * @return Option instance.
 *         - If success, flag `is_okay` is `true` and `pointer`
 *           stores the PAIR that has the given key.
 *         - If failure, flag is_error is true and error is
 *           KEY_NOT_FOUND. 
 */
struct abel_return_option abel_map_find(
    struct abel_map* ptr_map, char* key_str);

/**
 * @brief Get value by key
 * 
 * @param ptr_map : Pointer to the target map
 * @param key_str : Key in pair to be inserted into map
 * @return Option instance.
 *         - If success, flag is_okay is true and pointer
 *           contains the pointer to the value associated
 *           with the given key.
 *         - If failure, or error KEY_NOT_FOUND is in the
 *           option.
 * @note Function `_find` returns pair, but `_at` returns
 *       value. That is the difference.
 */
struct abel_return_option abel_map_at(
    struct abel_map* ptr_map, char* key_str);

/**
 * @brief Assign a value to the key
 * 
 * If the key exists, its current value is replace by the
 * argument ptr_data. The previous pair is returned. If the
 * key doesn't exist, error is returned.
 * 
 * @param ptr_map : Pointer to the target map
 * @param key_str : Key in pair to be inserted into map
 * @param ptr_data : Pointer to the data to be associated
 *                   with the key, i.e the actual value the
 *                   pair is pointing at.
 * @return Option instance.
 *         - Per success, flag is_okay is true and the previous
 *           pair with the given key is returned.
 *         - Per failure, flag is_error is true and KEY_NOT_FOUND
 *           error is returned.
 */
struct abel_return_option abel_map_assign(
    struct abel_map* ptr_map, char* key_str, void* ptr_data);

/**
 * @brief Erase a key-value pair
 * 
 * If key exists, the pair is disconnected from the vector
 * or linked list and it is returned. Otherwise, returns
 * KEY_NOT_FOUND error. Should the pair be found on a linked
 * list, the node that was connected to the pair is erased
 * and freed.
 * 
 * @return Option instance.
 *         - If success, flag `is_okay` is `true` and the PAIR
 *           with the given key is returned in pointer.
 *         - If failure, flag `is_error` is `true` and the
 *           error is stored in `error`. 
 * @note If the key-value pair is found on the linked list,
 *       i.e. key collision case, the node is erased and freed.
 */
struct abel_return_option abel_map_erase(
    struct abel_map* ptr_map, char* key_str);

#endif