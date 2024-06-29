/* Unittest map */
#include <assert.h>
#include "map.h"

void test_abel_map_make()
{
    struct abel_map* ptr_test_map = abel_make_map_ptr();
    assert(abel_vector_size(ptr_test_map->ptr_pair_vector) == 2048);
    assert(abel_vector_size(ptr_test_map->ptr_coll_vector) == 2048);
    assert(ptr_test_map->size == 0);
    
    /* Clean up */
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

void test_abel_map_insert()
{
    /* Get the return option ready */
    struct abel_return_option ret;
    /* First, make a map on heap. */
    struct abel_map* ptr_test_map = abel_make_map_ptr();
    /* I want to insert pair ("Hello", 777) into the map */
    char* test_key = "Hello"; // converted to 975
    int test_value = 777;

    /* Insert. It is the first time, so the return shall be okay. */
    ret = abel_map_insert(ptr_test_map, test_key, &test_value);
    assert(ptr_test_map->size == 1);
    assert(ret.pointer != NULL);
    assert( *(int*)((struct abel_key_value_pair*)ret.pointer)->ptr_data == 777);
    /* Let me access the pair vector directly. */
    struct abel_key_value_pair* ptr_pair_aquired = abel_vector_at(ptr_test_map->ptr_pair_vector, 975).pointer;
    assert(strcmp(ptr_pair_aquired->key, "Hello") == 0);

    /* Now let me insert again a pair that has the same key */
    struct abel_return_option ret2 = abel_map_insert(ptr_test_map, test_key, &test_value);
    assert(ret2.error.error_type == KEY_EXISTS);

    /* Clean up */
    abel_free_pair( (struct abel_key_value_pair*)ret.pointer );
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

/**
 * @brief Test key collision
 * 
 * In this test, a pair with key "World" is manually placed at
 * the position corresponding to key "Hello", ie index 975.
 * The pair with key "Hello" will be place by using the public
 * interface, to simulate the key collision.
 * 
 * The result of this test is that the new pair shall be stored
 * collision vector.
 */
void test_key_collision_first_register()
{
    struct abel_return_option ret; /* Get the return option ready */
    /* 
        First, make a map on heap.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();

    /*
        Now I manually place a pair at index 975
    */
    char* test_key_1 = "World";
    double test_value_1 = 3.14;
    /*
        To do that, I have manually make a pair on heap ... 
    */
    struct abel_key_value_pair* ptr_pair_1 = NULL;
    ptr_pair_1 = malloc( sizeof(*ptr_pair_1) );
    ptr_pair_1->key = malloc(strlen(test_key_1) + 1);
    strcpy(ptr_pair_1->key, test_key_1);
    ptr_pair_1->ptr_data = &test_value_1;
    /*
        ... and place it at index 975 on pair vector
    */
    ptr_test_map->ptr_pair_vector->ptr_array[975] = ptr_pair_1;
    ptr_test_map->size += 1;

    /*
        Then, call public interface to insert another pair
    */
    char* test_key_2 = "Hello";
    int test_value_2 = 777;
    ret = abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    /*
        Returned shall be this pair stored in collision vector
    */
    struct abel_key_value_pair* ptr_ret_pair = ret.pointer;
    assert(strcmp(ptr_ret_pair->key, "Hello") == 0);
    assert(*(int*)(ptr_ret_pair->ptr_data) == 777);
    assert(ptr_test_map->size == 2);

    /*
        Okay, let me check this directly in the collision vector
    */
    struct abel_key_value_pair* ptr_new_pair
        = ( (struct abel_linked_list*)ptr_test_map->ptr_coll_vector->ptr_array[975] )->ptr_data;
    /*
        Key in this pair is "Hello"
    */
    assert(strcmp(ptr_new_pair->key, "Hello") == 0);
    /*
        Value in this pair is 777
    */
    int value = *(int*)(ptr_new_pair->ptr_data);
    assert(value == 777);

    /*
        After all this, if I attempt inserting a pair ("Hello", 123)
        the return shall be error typed KEY EXISTS.
    */
    struct abel_return_option ret2
        = abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    assert(ret2.error.error_type == KEY_EXISTS);

    /* Clean up */
    abel_free_pair(ptr_pair_1);
    abel_free_pair( (struct abel_key_value_pair*)ret.pointer );
    /* Caution. Must free the linked list. */
    free(ptr_test_map->ptr_coll_vector->ptr_array[975]);
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

/**
 * @brief Test key collision with collided key.
 * 
 * In this test, I will manually occupy the elements indexed 975
 * in both pair vector and collision vector, to simulate the case
 * that for that index, there already exists a collided key.
 * 
 */
void test_key_collision_second_register()
{
    /* 
        First, make a map on heap and get retur option ready.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();
    struct abel_return_option ret;

    /*
        Now I manually place a pair at index 975 on pair vector
    */
    char* test_key_1 = "World";
    double test_value_1 = 3.14;
    /*
        To do that, I have manually make a pair on heap, because
        the pair maker is a static function. :(
    */
    struct abel_key_value_pair* ptr_pair_1 = NULL;
    ptr_pair_1 = malloc( sizeof(*ptr_pair_1) );
    ptr_pair_1->key = malloc(strlen(test_key_1) + 1);
    strcpy(ptr_pair_1->key, test_key_1);
    ptr_pair_1->ptr_data = &test_value_1;
    /*
        ... and place it at index 975 on pair vector.
        Don't forget to manually adjust the map size.
    */
    ptr_test_map->ptr_pair_vector->ptr_array[975] = ptr_pair_1;
    ptr_test_map->size += 1;

    /*
        Second, I will associate another pair with the index 975
        on collision vector, to simulate the scenario that one
        collided key was already registered. But, this time, 
        I need linked list too.
    */
    char* test_key_2 = "Vader";
    int test_value_2 = 543;
    /*
        Again, I need to manually make the pair.
    */
    struct abel_key_value_pair* ptr_pair_2 = NULL;
    ptr_pair_2 = malloc( sizeof(*ptr_pair_2) );
    ptr_pair_2->key = malloc(strlen(test_key_2) + 1);
    strcpy(ptr_pair_2->key, test_key_2);
    ptr_pair_2->ptr_data = &test_value_2;
    /*
        And of course, manually place it in the collision vector.
    */
    abel_vector_emplace(ptr_test_map->ptr_coll_vector, 975,
                        abel_make_linked_list_node(ptr_pair_2) );
    ptr_test_map->size++;    /* increase map size */

    /*
        Up to now, we have two pairs ("World", 3.14) and ("Vader", 543) 
        registered on the map. They are both occupying index 975. If I
        further insert ("Hello", 777), which is a key collision, but this
        pair will be registered as the key is different. However, the
        registration this time shall be on the collision vector with
        a linked list node.
    */
    char* test_key_3 = "Hello";
    int test_value_3 = 777;
    ret = abel_map_insert(ptr_test_map, test_key_3, &test_value_3);
    assert(*(int*)((struct abel_key_value_pair*)ret.pointer)->ptr_data == 777);

    /*
        This newly added pair is found by searching along the linked
        list. Fine! Let me go along that manually. The head node is
        associated with ("Vader", 543)
    */
    struct abel_linked_list* ptr_node_for_2
        = (struct abel_linked_list*)(ptr_test_map->ptr_coll_vector->ptr_array[975]);
    struct abel_key_value_pair* ptr_acquired_pair_2 = ptr_node_for_2->ptr_data;
    assert(*(int*)ptr_acquired_pair_2->ptr_data == 543);
    assert(strcmp(ptr_acquired_pair_2->key, "Vader") == 0);
    /*
        Next node on the linked list is associated with 
    */
    struct abel_linked_list* ptr_node_for_3 = ptr_node_for_2->next;
    struct abel_key_value_pair* ptr_acquired_pair_3 = ptr_node_for_3->ptr_data;
    assert(*(int*)ptr_acquired_pair_3->ptr_data == 777);
    assert(strcmp(ptr_acquired_pair_3->key, "Hello") == 0);

    /* Clean up */
    abel_free_pair(ptr_pair_1);
    abel_free_pair(ptr_pair_2);
    abel_free_pair( (struct abel_key_value_pair*)ret.pointer );
    /* Caution. Must free the linked list nodes . */
    free( ((struct abel_linked_list*)(ptr_test_map->ptr_coll_vector->ptr_array[975]))->next);
    free( ptr_test_map->ptr_coll_vector->ptr_array[975] );
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

void test_map_find()
{
    /* 
        First, make a map on heap and get retur option ready.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();

    /*
        Now, insert some pairs
    */
    char* test_key_1 = "double";
    double test_value_1 = 3.14;
    struct abel_return_option ret1
        = abel_map_insert(ptr_test_map, test_key_1, &test_value_1);
    assert(abel_map_size(ptr_test_map) == 1);

    char* test_key_2 = "int";
    int test_value_2 = 777;
    struct abel_return_option ret2
        = abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    assert(abel_map_size(ptr_test_map) == 2);

    /*
        Okay, let's get pairs.
    */
    struct abel_return_option ret3 = abel_map_find(ptr_test_map, test_key_1);
    assert(ret3.pointer != NULL);
    struct abel_key_value_pair* ptr_pair_1 = ret3.pointer;
    assert(*(double*)(ptr_pair_1->ptr_data) == 3.14);

    struct abel_return_option ret4 = abel_map_find(ptr_test_map, test_key_2);
    assert(ret4.pointer != NULL);
    struct abel_key_value_pair* ptr_pair_2 = ret4.pointer;
    assert(*(int*)(ptr_pair_2->ptr_data) == 777);

    /*
        Let me try to retrieve a key that doesn't exist.
    */
    struct abel_return_option ret5 = abel_map_find(ptr_test_map, "some_key");
    assert(ret5.error.error_type = KEY_NOT_FOUND);
    
    abel_free_pair(ret1.pointer);
    abel_free_pair(ret2.pointer);
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

void test_map_at()
{
    /* 
        First, make a map on heap and get retur option ready.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();

    /*
        Now, insert some pairs
    */
    char* test_key_1 = "double";
    double test_value_1 = 3.14;
    struct abel_return_option ret1
        = abel_map_insert(ptr_test_map, test_key_1, &test_value_1);
    assert(abel_map_size(ptr_test_map) == 1);

    char* test_key_2 = "int";
    int test_value_2 = 777;
    struct abel_return_option ret2
        = abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    assert(abel_map_size(ptr_test_map) == 2);

    /*
        Okay, let's get.
    */
    struct abel_return_option ret3 = abel_map_at(ptr_test_map, test_key_1);
    assert(ret3.pointer != NULL);
    double value_1 = *(double*)ret3.pointer;
    assert(value_1 == 3.14);

    struct abel_return_option ret4 = abel_map_at(ptr_test_map, test_key_2);
    assert(ret4.pointer != NULL);
    int value_2 = *(int*)ret4.pointer;
    assert(value_2 == 777);

    /*
        Let me try to retrieve a key that doesn't exist.
    */
    struct abel_return_option ret5 = abel_map_at(ptr_test_map, "some_key");
    assert(ret5.error.error_type = KEY_NOT_FOUND);
    
    abel_free_pair(ret1.pointer);
    abel_free_pair(ret2.pointer);
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

void test_map_assign()
{
    /* 
        First, make a map on heap and get retur option ready.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();

    /*
        Now, insert some pairs
    */
    char* test_key_1 = "double";
    double test_value_1 = 3.14;
    struct abel_return_option ret1
        = abel_map_insert(ptr_test_map, test_key_1, &test_value_1);
    assert(abel_map_size(ptr_test_map) == 1);

    char* test_key_2 = "int";
    int test_value_2 = 777;
    struct abel_return_option ret2
        = abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    assert(abel_map_size(ptr_test_map) == 2);

    /*
        Okay, let's assign a different value to "int"
    */
    int test_value_3 = 42;
    struct abel_return_option ret3
        = abel_map_assign(ptr_test_map, test_key_2, &test_value_3);
    assert(ret3.pointer != NULL);
    struct abel_key_value_pair* ptr_pair_1 = ret3.pointer;
    assert(*(int*)(ptr_pair_1->ptr_data) == 42);

    /*
        Now, let's assign a value to a non-existent key.
        A new pair is created in the map, so it must be freed.
    */
    char* test_key_new = "new";
    double test_value_new = 8.69;
    struct abel_return_option ret4
        = abel_map_assign(ptr_test_map, test_key_new, &test_value_new);
    //Pair* ptr_pair_4 = ret4.pointer;
    assert(ret4.is_error == true);
    assert(ret4.error.error_type == KEY_NOT_FOUND);

    /* Clean up */
    abel_free_pair(ret1.pointer);
    abel_free_pair(ret2.pointer);
    //abel_free_pair(ret4.pointer);
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

void test_map_erase()
{
    /* 
        First, make a map on heap and get retur option ready.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();

    /*
        Now, need to insert some pairs.
    */
    char* test_key_1 = "double";
    double test_value_1 = 3.14;
    abel_map_insert(ptr_test_map, test_key_1, &test_value_1);
    assert(abel_map_size(ptr_test_map) == 1);

    char* test_key_2 = "int";
    int test_value_2 = 777;
    abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    assert(abel_map_size(ptr_test_map) == 2);

    /*
        Let me erase the pair with key "double"
    */
    struct abel_return_option ret3 = abel_map_erase(ptr_test_map, test_key_1);
    /*
        After erasing key "double", the map has only one pair
        left.
    */
    assert(abel_map_size(ptr_test_map) == 1);
    /*
        Let me try to retrieve a pair that doesn't exist.
    */
    struct abel_return_option ret4 = abel_map_find(ptr_test_map, "double");
    assert(ret4.error.error_type = KEY_NOT_FOUND);

    /*
        Let me further erase the last key in the map.
        Key "int".
    */
    struct abel_return_option ret5 = abel_map_erase(ptr_test_map, "int");
    assert(ret5.is_okay == true);
    assert(abel_map_size(ptr_test_map) == 0);
    struct abel_key_value_pair* ptr_pair = ret5.pointer;
    assert(strcmp(ptr_pair->key, "int") == 0);
    assert(*(int*)ptr_pair->ptr_data == 777);
    
    abel_free_pair(ptr_pair);
    abel_free_pair(ret3.pointer);
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

/**
 * @brief Erase in the case of key collision
 * 
 * In this test, I want to delete a pair that is registered
 * on the linked list attached to the collision vector. This
 * simulates the case that a key-value pair is registered due
 * to collision.
 * 
 * The hashing function used in Abel map converts "Hello" to
 * integer 975. So, I will manually place a pair ("World", 3.14)
 * at 975 on the pair vector such that, per inserting ("Hello", 777)
 * via public interface, the map will place it on the collision vector.
 * 
 * Then, per deleting ("Hello", 777), the linked list node attached
 * to it shall also be erased and memory freed.
 */
void test_map_erase_with_collision()
{
    struct abel_return_option ret; /* Get the return option ready */
    /* 
        First, make a map on heap and manually place a pair
        at index 975 on the pair vector.
    */
    struct abel_map* ptr_test_map = abel_make_map_ptr();
    char* test_key_1 = "World";
    double test_value_1 = 3.14;
    /*
        To do that, I have to manually make a pair on heap, as
        the pair maker is not part of the public interface.
    */
    struct abel_key_value_pair* ptr_pair_1 = NULL;
    ptr_pair_1 = malloc( sizeof(*ptr_pair_1) );
    ptr_pair_1->key = malloc(strlen(test_key_1) + 1);
    strcpy(ptr_pair_1->key, test_key_1);
    ptr_pair_1->ptr_data = &test_value_1;
    /*
        Now, place it at index 975 on pair vector
    */
    ptr_test_map->ptr_pair_vector->ptr_array[975] = ptr_pair_1;
    ptr_test_map->size += 1;

    /*
        Then, call public interface to insert another pair that
        collides with the occupant at index 975.
    */
    char* test_key_2 = "Hello";
    int test_value_2 = 777;
    ret = abel_map_insert(ptr_test_map, test_key_2, &test_value_2);
    /*
        Returned shall be this pair stored in collision vector
    */
    struct abel_key_value_pair* ptr_ret_pair = ret.pointer;
    assert(strcmp(ptr_ret_pair->key, "Hello") == 0);
    assert(*(int*)(ptr_ret_pair->ptr_data) == 777);
    assert(ptr_test_map->size == 2);

    /*
        Up to now, the map shall have size 2 and the above keys
        are available. Caution! It is not possible to enquire the
        existence of key "World" by using public interface function
        `abel_map_find`, because "World" is converted by hashing
        function into an integer that is different from 975.
    */
    assert(abel_map_size(ptr_test_map) == 2);
    assert(abel_map_find(ptr_test_map, "Hello").is_okay == true);
    
    /*
        Let's erase "Hello". I shall expect the following:
        (1) The pair that has this key shall be returned.
        (2) The linked list node that is attached to this
            key is erased and freed.
        (3) At index 975 on the collision vector, it is
            NULL again.
    */
    struct abel_return_option ret2 = abel_map_erase(ptr_test_map, "Hello");
    assert(ret2.is_okay == true);
    struct abel_key_value_pair* ptr_hello_pair = ret2.pointer;
    assert(strcmp(ptr_hello_pair->key, "Hello") == 0);
    assert(*(int*)ptr_hello_pair->ptr_data == 777);

    /* Clean up */
    abel_free_pair(ptr_pair_1);
    abel_free_pair( (struct abel_key_value_pair*)ret2.pointer );
    abel_free_vector_ptr(ptr_test_map->ptr_pair_vector);
    abel_free_vector_ptr(ptr_test_map->ptr_coll_vector);
    free(ptr_test_map);
}

int main()
{
    test_abel_map_make();
    test_abel_map_insert();
    test_key_collision_first_register();
    test_key_collision_second_register();
    test_map_find();
    test_map_at();
    test_map_assign();

/* erase */
    test_map_erase();
    test_map_erase_with_collision();
}