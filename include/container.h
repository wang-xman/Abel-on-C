/*
 * HEADER container.h
 * Created 05-11-2022
 * Updated 05-11-2022
 * 
 * Combines both list and dictionary.
 */
#include "list.h"
#include "dict.h"

struct abel_dict* abel_list_get_dict_ptr(struct abel_list* ptr_list,
                                         size_t index);