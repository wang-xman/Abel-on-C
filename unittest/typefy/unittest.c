/* Unittest typefy */
#include <assert.h>
#include "typefy.h"

void test_bool()
{
    Bool test_value = false;
    assert(test_value == 0);
    test_value = true;
    assert(test_value == 1);
}

void test_data_type()
{
    enum data_type test_value = BOOL_TYPE;
    assert(strcmp(DATA_TYPE_STRING[test_value], "Bool") == 0);
    assert(strcmp(DATA_TYPE_STRING[NULL_TYPE], "Null") == 0);
    assert(strcmp(DATA_TYPE_STRING[INTEGER_TYPE], "Integer") == 0);
    assert(strcmp(DATA_TYPE_STRING[DOUBLE_TYPE], "Double") == 0);
    assert(strcmp(DATA_TYPE_STRING[DICT_TYPE], "Dict") == 0);
}

int main()
{
    test_bool();
    test_data_type();
}