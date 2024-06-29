/*
 * SOURCE container.h
 * Created 05-11-2022
 * Updated 05-11-2022
 */
#include "container.h"

struct abel_dict* abel_list_get_dict_ptr(struct abel_list* ptr_list, size_t index)
{
    return abel_object_get_dict_ptr(abel_list_get_object_pointer(ptr_list, index));
}