/**
 * Header linked_list.h
 * 
 * General-purpose singly-linked list.
 * 
 * Linked list is a container. Each node is a reference
 * to the item to be linked together.
 * 
 * The resouces that are pointed by the linked-list nodes
 * are not the responsibility of the linked list. User
 * of the linked list must manage that.
 * 
 * Functions
 * 
 * - Maker
 *     linked_list_ptr abel_make_linked_list_node(void* ptr_data);
 * 
 * - Freer
 *     Option abel_free_linked_list(linked_list_ptr ptr_head);
 * 
 * - Checker
 *     size_t abel_linked_list_size(LinkedList* ptr_head)
 * 
 * - Append
 *     Option abel_linked_list_append(LinkedList* ptr_head, void* ptr_data)
 * 
 * - Emplace: returns previous occupant
 *     Option abel_linked_list_emplace(LinkedList* ptr_head, size_t idx, void* ptr_data);
 *     Option abel_linked_list_set(LinkedList* ptr_head, size_t idx, void* ptr_data);
 * 
 * - Getter
 *     Option abel_linked_list_at(LinkedList* ptr_head, size_t idx)
 * 
 * @todo 1. Modify the comments in accordance with the change made
 *          in return option. 
 *       2. Implement more functions.
 */
#ifndef ABEL_ON_C_LINKED_LIST_H
#define ABEL_ON_C_LINKED_LIST_H

#include "option.h"    // has generic.h

/**
 * @brief General-purpose linked list
 * 
 * Singly linked list that can reference any data type.
 * 
 * Fields
 * 
 * ptr_data : Pointer to the source data referenced by
 *            a linked list node.
 * 
 * next : Pointer to the next node
 */
struct abel_linked_list {
    void* ptr_data;
    struct abel_linked_list* next;
};

/* Maker */

/**
 * @brief Make a linked-list node
 *
 * Constructs a linked list node on heap and returns the
 * pointer to it. Field `next` is set to NULL. To make an
 * empty node, pass NULL as the argument.
 * 
 * @param ptr_data : Pointer to the item which is pointed
 *                   by the linked list node.
 * @return Pointer to the node created on heap.
 */
struct abel_linked_list* abel_make_linked_list_node(void* ptr_data);

/* Freer */

/**
 * @brief Free linked list
 * 
 * This freer only frees the resources allocated for the
 * linked list.
 * 
 * @param ptr_head : Pointer to the head node
 * @note Important. The resouces that are pointed by the
 *       linked-list nodes are not the responsibility of
 *       the linked list. User of the linked list must
 *       manage that.
 * @todo What error shall I return?
 */
struct abel_return_option abel_free_linked_list(struct abel_linked_list* ptr_head);

/* Checker */

/**
 * @brief Size of the linked list.
 *
 * Size of a linked list is the total number of nodes that
 * are linked. If there is only one node, the size is 1.
 * 
 * @return Total number of nodes that are linked.
 */
size_t abel_linked_list_size(struct abel_linked_list* ptr_head);

/* Append */

/**
 * @brief Append a node to linked list
 * 
 * Append the node at the end of the list. Find the current
 * tail node and link its `next` to the new tail node.
 * 
 * @param ptr_head : Pointer to the head node
 * @param ptr_data : Pointer to the item to be appended to
 *                   the linked list
 */
struct abel_return_option abel_linked_list_append(
    struct abel_linked_list* ptr_head, void* ptr_data);

/* Emplace */

/**
 * @brief Emplace data at a node located by index
 *
 * @param ptr_head : Pointer to the head node.
 * @param ptr_data : Pointer to the data to be place at
 *                   the node of the given index.
 * @return Option that contains either the previous occupant
 *         at the given node, or an error.
 */
struct abel_return_option abel_linked_list_emplace(
    struct abel_linked_list* ptr_head, size_t idx, void* ptr_data);

/**
 * @brief Set data of a node at given index
 *
 * @note Identical and alias to `abel_linked_list_emplace`.
 */
struct abel_return_option abel_linked_list_set(
    struct abel_linked_list* ptr_head, size_t idx, void* ptr_data);

/* Getter */

/**
 * @brief Get an item at given index
 *
 * Head node is at index 0, the next is 1, and so on.
 * Index must be less than the total number of nodes.
 * 
 * It returns the data at which the node of given index
 * is pointing, not the node itself. To return the node
 * itself, call `_node_at`.
 * 
 * @param ptr_head : Pointer to the head node.
 * @param idx : Index of the node to be acccessed.
 * @return Option instance.
 *         - If index is within range, flag is_okay is true
 *           and the pointer to data is stored in pointer.
 *         - If fails, flag is_error is true and the error
 *           is returned in `error`.
 */
struct abel_return_option abel_linked_list_at(
    struct abel_linked_list* ptr_head, size_t idx);

/**
 * @brief Get a node at given index
 *
 * Head node is at index 0, the next is 1, and so on.
 * Index must be less than the total number of nodes.
 * 
 * It returns the pointer to the node at the given index
 * not the data the node is pointing at. To return the
 * data, call `_at`. Therefore, the returned pointer must
 * be casted back to `LinkedList*` before accessing its
 * fields.
 * 
 * @param ptr_head : Pointer to the head node.
 * @param idx : Index of the node to be acccessed.
 * @return Option instance.
 *         - If index is within range, flag is_okay is true
 *           and the pointer to the node is stored in pointer.
 *         - If fails, flag is_error is true and the error
 *           is returned in `error`.
 */
struct abel_return_option abel_linked_list_node_at(
    struct abel_linked_list* ptr_head, size_t idx);

/* Delete */

/**
 * @brief Erase a node at given index
 * 
 * For a valid index, it returns a pointer to that node
 * and disconnects that node from the linked list. In case
 * the head node is erased, the pointer to the new head
 * node will replace the old. This is why the first argument
 * must be a pointer to the pointer to the head node.
 * 
 * Should the linked list has only one node which is to be
 * erased, the head will be set to NULL.
 * 
 * @param ptr_ptr_head : Pointer to the pointer of head node.
 *                       As the head node can be erased, too,
 *                       it must be replaced by a new head.
 * @param idx : Index of the node to be erased.
 * @return Option instance.
 *         - If index is within range, flag is_okay is true
 *           and the pointer to the disconnected NODE is stored
 *           in pointer.
 *         - If fails, flag is_error is true and the error
 *           is returned in `error`.
 */
struct abel_return_option abel_linked_list_erase(
    struct abel_linked_list** ptr_ptr_head, size_t idx);

/* TO BE IMPLEMENTED */
/**
 * @brief Insert a node after the given index
 *
 * @todo implement this function
 */
struct abel_return_option abel_linked_list_insert_after(
    struct abel_linked_list* ptr_head, size_t idx, void* ptr_data);
#endif