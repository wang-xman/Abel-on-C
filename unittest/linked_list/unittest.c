/* Unittest linked list */
#include <assert.h>
#include "linked_list.h"

void test_make_node()
{
    int item0 = 777;
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(&item0);
    assert(ptr_node->next == NULL);
    assert(*(int*)ptr_node->ptr_data == 777);

    assert(abel_linked_list_size(ptr_node) == 1);

    abel_free_linked_list(ptr_node);
}

void test_make_empty_node()
{
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(NULL);
    assert(ptr_node->next == NULL);
    assert(ptr_node->ptr_data == NULL);
    assert(abel_linked_list_size(ptr_node) == 1);

    abel_free_linked_list(ptr_node);
}

void test_linked_list_append()
{
    int item0 = 777;
    double item1 = 3.14;
    int item2 = 45;
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(&item0);
    abel_linked_list_append(ptr_node, &item1);
    abel_linked_list_append(ptr_node, &item2);

    assert(abel_linked_list_size(ptr_node) == 3);
    abel_free_linked_list(ptr_node);
}

/* Emplace */
void test_linked_list_set()
{
    struct abel_return_option ret;
    int item0 = 777;
    double item1 = 3.14;
    int item2 = 45;
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(&item0);
    abel_linked_list_append(ptr_node, &item1);
    abel_linked_list_append(ptr_node, &item2);
    /*
        Now the linked list looks like this 777 -> 3.14 -> 45
    */
    assert(abel_linked_list_size(ptr_node) == 3);

    /*
        Let's set the value on a specific node, say at index 1
    */
    int item_mid = 256;
    ret = abel_linked_list_set(ptr_node, 1, &item_mid);
    /*
        The returned option contains the previous occupant
    */
    assert(*(double*)ret.pointer == 3.14);
    /*
        The present occupant is 256
    */
    assert(*(int*)abel_linked_list_at(ptr_node, 1).pointer == 256);
    /*
        Of course, the size is still 3.
    */
    assert(abel_linked_list_size(ptr_node) == 3);

    abel_free_linked_list(ptr_node);
}

void test_linked_list_at()
{
    int item0 = 777;
    double item1 = 3.14;
    int item2 = 45;
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(&item0);
    abel_linked_list_append(ptr_node, &item1);
    abel_linked_list_append(ptr_node, &item2);
    /*
        Now the list is 777 -> 3.14 -> 45
    */
    assert(abel_linked_list_size(ptr_node) == 3);

    struct abel_return_option ret;
    ret = abel_linked_list_at(ptr_node, 2);
    assert(*(int*)ret.pointer == 45);
    ret = abel_linked_list_at(ptr_node, 0);
    assert(*(int*)ret.pointer == 777);
    ret = abel_linked_list_at(ptr_node, 1);
    assert(*(double*)ret.pointer == 3.14);

    /*
        Node index 3 doesn't exist.
    */
    ret = abel_linked_list_at(ptr_node, 3);
    assert(ret.error.error_type == OUT_OF_RANGE);

    abel_free_linked_list(ptr_node);
}

void test_linked_list_node_at()
{
    int item0 = 777;
    double item1 = 3.14;
    int item2 = 45;
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(&item0);
    abel_linked_list_append(ptr_node, &item1);
    abel_linked_list_append(ptr_node, &item2);
    /*
        Now the list is 777 -> 3.14 -> 45
    */
    assert(abel_linked_list_size(ptr_node) == 3);

    struct abel_return_option ret;
    struct abel_linked_list* ptr_node_at = NULL;
    ret = abel_linked_list_node_at(ptr_node, 2);
    ptr_node_at = ret.pointer;
    assert(*(int*)ptr_node_at->ptr_data == 45);

    ret = abel_linked_list_node_at(ptr_node, 0);
    ptr_node_at = ret.pointer;
    assert(*(int*)ptr_node_at->ptr_data == 777);

    ret = abel_linked_list_node_at(ptr_node, 1);
    ptr_node_at = ret.pointer;
    assert(*(double*)ptr_node_at->ptr_data == 3.14);

    /*
        Node index 3 doesn't exist.
    */
    ret = abel_linked_list_node_at(ptr_node, 3);
    assert(ret.error.error_type == OUT_OF_RANGE);

    abel_free_linked_list(ptr_node);
}

void test_linked_list_erase()
{
    int item0 = 777;
    double item1 = 3.14;
    int item2 = 45;
    struct abel_linked_list* ptr_node = abel_make_linked_list_node(&item0);
    abel_linked_list_append(ptr_node, &item1);
    abel_linked_list_append(ptr_node, &item2);
    /*
        Now the list is 777 -> 3.14 -> 45
    */
    assert(abel_linked_list_size(ptr_node) == 3);

    struct abel_return_option ret;
    struct abel_linked_list* ptr_node_removed = NULL;
    /*
        Erase node at index 1 to have 777 -> 45
    */
    //LinkedList* ptr_new_head = NULL;
    ret = abel_linked_list_erase(&ptr_node, 1);
    ptr_node_removed = ret.pointer;
    assert(abel_linked_list_size(ptr_node) == 2);
    assert(*(double*)ptr_node_removed->ptr_data == 3.14);
    /*
        Must free the disconnected node.
    */
    free(ptr_node_removed);

    /*
        The linked list right now is 777 -> 45.
        I want further erase the node at index 0 to have
        a mono-node 45
    */
    ret = abel_linked_list_erase(&ptr_node, 0);
    ptr_node_removed = ret.pointer;
    assert(abel_linked_list_size(ptr_node) == 1);
    assert(*(int*)ptr_node_removed->ptr_data == 777);
    free(ptr_node_removed);

    /*
        Up to this stage, the linked list is just one node
        left and it is 45. Now, I want to delete that, too.
    */
    ret = abel_linked_list_erase(&ptr_node, 0);
    ptr_node_removed = ret.pointer;
    /*
        Deleting a mono-node results in a NULL pointer and
        of course its size is 0.
    */
    assert(ptr_node == NULL);
    assert(abel_linked_list_size(ptr_node) == 0);
    assert(*(int*)ptr_node_removed->ptr_data == 45);
    free(ptr_node_removed);

    ret = abel_linked_list_erase(&ptr_node, 0);
    assert(ret.is_okay == false);
    // assert(ret.pointer == NULL);
    assert(ret.error.error_type == OUT_OF_RANGE);

    abel_free_linked_list(ptr_node);
}

int main()
{
/* Maker */
    test_make_node();
    test_make_empty_node();
/* Append */
    test_linked_list_append();
/* Emplace */
    test_linked_list_set();
/* get and indexing */
    test_linked_list_at();
    test_linked_list_node_at();
/* erase */
    test_linked_list_erase();
}