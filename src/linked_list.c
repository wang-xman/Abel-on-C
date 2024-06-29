/* Source linked_list.c */
#include "linked_list.h"

/* Maker */

struct abel_linked_list* abel_make_linked_list_node(void* ptr_data)
{
    struct abel_linked_list* ptr_head = NULL;
    ptr_head = malloc( sizeof(*ptr_head) );
    if (ptr_head != NULL) {
        ptr_head->ptr_data = ptr_data;
        ptr_head ->next = NULL;
    }
    return ptr_head;
}

/* Freer */

struct abel_return_option abel_free_linked_list(struct abel_linked_list* ptr_head)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    struct abel_linked_list* ptr_node_tofree = NULL;
    struct abel_linked_list* ptr_current_node = ptr_head;
    while (ptr_current_node != NULL) {
        ptr_node_tofree = ptr_current_node;
        ptr_current_node = ptr_current_node->next;
        free(ptr_node_tofree);
    }
    return ret;
}

/* Checker */

size_t abel_linked_list_size(struct abel_linked_list* ptr_head)
{
    struct abel_linked_list* ptr_current_node = ptr_head;
    size_t length = 0;
    while (ptr_current_node != NULL) {
        length += 1;
        ptr_current_node = ptr_current_node->next;
    }
    return length;
}

/* Append */

struct abel_return_option abel_linked_list_append(
    struct abel_linked_list* ptr_head, void* ptr_data)
{
    struct abel_return_option ret;
    struct abel_linked_list* ptr_current_node = ptr_head;
    struct abel_linked_list* ptr_tail_node = NULL;
    while (ptr_current_node->next != NULL) {    // find tail
        ptr_current_node = ptr_current_node->next;
    }
    ptr_tail_node = abel_make_linked_list_node(ptr_data);    // new tail node
    if (ptr_tail_node != NULL) {
        ptr_current_node->next = ptr_tail_node;   // link new tail to current
        ret = abel_option_okay(NULL);
    } else {
        ret = abel_option_error( error_malloc_failure() );
    }
    return ret;
}

/* Emplace */

struct abel_return_option abel_linked_list_emplace(
    struct abel_linked_list* ptr_head, size_t idx, void* ptr_data)
{
    struct abel_return_option ret;
    size_t counter = 0;
    struct abel_linked_list* ptr_current_node = ptr_head;
    while (ptr_current_node->next != NULL) {    // find current tail
        if (counter == idx) {
            break;
        } else {
            counter += 1;
            ptr_current_node = ptr_current_node->next;
        }
    }
    if (counter == idx) {
        /* return the old and place the new */
        ret = abel_option_okay(ptr_current_node->ptr_data);
        ptr_current_node->ptr_data = ptr_data;
    } else {
        /* Index out of range, return error */
        ret = abel_option_error( error_out_of_range() );
    }
    return ret;
}

struct abel_return_option abel_linked_list_set(
    struct abel_linked_list* ptr_head, size_t idx, void* ptr_data)
{
    return abel_linked_list_emplace(ptr_head, idx, ptr_data);
}

/* Getter */

struct abel_return_option abel_linked_list_at(
    struct abel_linked_list* ptr_head, size_t idx)
{
    struct abel_return_option ret;
    size_t counter = 0;
    struct abel_linked_list* ptr_current_node = ptr_head;
    while (ptr_current_node->next != NULL) {    // find current tail
        if (counter == idx) {
            break;
        } else {
            counter += 1;
            ptr_current_node = ptr_current_node->next;
        }
    }
    if (counter == idx) {
        ret = abel_option_okay(ptr_current_node->ptr_data);
    } else {
        /* Index out of range, return error */
        ret = abel_option_error( error_out_of_range() );
    }
    return ret;
}

struct abel_return_option abel_linked_list_node_at(
    struct abel_linked_list* ptr_head, size_t idx)
{
    struct abel_return_option ret;
    size_t counter = 0;
    struct abel_linked_list* ptr_current_node = ptr_head;
    while (ptr_current_node->next != NULL) {    // find current tail
        if (counter == idx) {
            break;
        } else {
            counter += 1;
            ptr_current_node = ptr_current_node->next;
        }
    }
    if (counter == idx) {
        ret = abel_option_okay(ptr_current_node);
    } else {
        /* Index out of range, return error */
        ret = abel_option_error( error_out_of_range() );
    }
    return ret;
}

struct abel_return_option abel_linked_list_erase(
    struct abel_linked_list** ptr_ptr_head, size_t idx)
{
    struct abel_return_option ret;
    size_t counter = 0;
    struct abel_linked_list* ptr_previous_node = NULL;
    struct abel_linked_list* ptr_current_node = *ptr_ptr_head;
    /* If head is NULL, nothing to erase */
    if (ptr_current_node != NULL) {
        while (ptr_current_node != NULL) {    // iterate over entire list
            /* current node is to be deleted */
            if (counter == idx) {
                /*
                    If previous node is NULL, which only happens when
                    the current node is at the "old" head, the next
                    becomes new head. This include the case in which
                    there is only 1 node and it is connected to the
                    desired key.
                */
                if (ptr_previous_node == NULL) {
                    if (ptr_current_node->next == NULL) {
                        *ptr_ptr_head = NULL;
                    } else {
                        *ptr_ptr_head = ptr_current_node->next;
                    }
                } else {    /* current not at head */
                    if (ptr_current_node->next == NULL) {   // current is tail
                        ptr_previous_node->next = NULL;    
                    } else {
                        ptr_previous_node->next = ptr_current_node->next;    
                    }
                }
                break;
            } else {
                counter += 1;
                ptr_previous_node = ptr_current_node;
                ptr_current_node = ptr_current_node->next;    // move to next
            }
        }
        if (counter == idx) {
            ret = abel_option_okay(ptr_current_node);
        } else {
            /* Index out of range, return error */
            ret = abel_option_error( error_out_of_range() );
        }
    } else {
        ret = abel_option_error( error_out_of_range() );
    }
    return ret;
}