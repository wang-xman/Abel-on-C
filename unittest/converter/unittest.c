/**
 * @file unittest.c
 * @brief Unittest converter module
 * @created 16-12-2022
 * @updated 16-12-2022 
 * @copyright Copyright (c) 2022 
 */
#include <assert.h>
#include "converter.h"

void test_is_null()
{
    /* true */
    char* test_str_0 = "null";
    assert(is_null(test_str_0) == true);
    char test_str_01[] = "null";
    assert(is_null(test_str_01) == true);

    /* false */
    char* test_str_1 = "nu ll";
    assert(is_null(test_str_1) == false);

    char* test_str_2 = " n u ll";
    assert(is_null(test_str_2) == false);

    char* test_str_3 = "u ll";
    assert(is_null(test_str_3) == false);
}

void test_is_bool()
{
    /* true */
    char* test_str_0 = "true";
    assert(is_bool(test_str_0) == true);

    char test_str_01[] = "false";
    assert(is_bool(test_str_01) == true);

    /* no */
    char* test_str_1 = "t r u";
    assert(is_bool(test_str_1) == false);

    char test_str_11[] = "f al e";
    assert(is_bool(test_str_11) == false);
}

void test_is_double()
{
    char test_string_0[] = ".e+10"; /* no a double */
    //char sentinel;
    //double dest;
    /* failure */
    assert(is_double(test_string_0) == false);

    char test_string_1[] = ".1e+10"; /* a double */
    /* okay */
    //assert(sscanf(test_string_1, "%lf %c", &dest, &sentinel) == 1);
    assert(is_double(test_string_1) == true);

    char test_string_2[] = ".1e-1 ssss"; /* double and text */
    /* okay but returns 2 */
    //assert(sscanf(test_string_2, "%lf %c", &dest, &sentinel) == 2);
    assert(is_double(test_string_2) == false);
    //printf("%c \n", sentinel);

    char test_string_3[] = " 1.1 "; /* double */
    /* okay, returns 1 */
    //assert(sscanf(test_string_3, "%lf %c", &dest, &sentinel) == 1);
    assert(is_double(test_string_3) == true);
    //printf("%c \n", sentinel);

    char test_string_4[] = " 1.1 E+1"; /* double and text */
    /* okay, returns 2 */
    //assert(sscanf(test_string_4, "%lf %c", &dest, &sentinel) == 2);
    //remove_space(test_string_4);
    //assert(sscanf(test_string_4, "%lf %c", &dest, &sentinel) == 1);
    assert(is_double(test_string_4) == true);

    char test_string_5[] = "    1.1E+1"; /* double */
    /* okay, returns 1 */
    //assert(sscanf(test_string_5, "%lf %c", &dest, &sentinel) == 1);
    assert(is_double(test_string_5) == true);
    printf("%s \n", test_string_5);

    struct abel_string test_string_6 = abel_make_string("    8.2E-10");
    printf("%s \n", test_string_6.ptr_array);
    assert(is_double(test_string_6.ptr_array) == true);
    printf("%s \n", test_string_6.ptr_array);
    abel_free_string(&test_string_6);
}

void test_sccanf()
{
    char test_string_0[] = ".e+10"; /* no a double */
    char sentinel;
    double dest;
    /* failure */
    assert(sscanf(test_string_0, "%lf %c", &dest, &sentinel) == 0);

    char test_string_1[] = ".1e+10"; /* a double */
    /* okay */
    assert(sscanf(test_string_1, "%lf %c", &dest, &sentinel) == 1);

    char test_string_2[] = ".1e-1 ssss"; /* double and text */
    /* okay but returns 2 */
    assert(sscanf(test_string_2, "%lf %c", &dest, &sentinel) == 2);
    //printf("%c \n", sentinel);

    char test_string_3[] = " 1.1 "; /* double */
    /* okay, returns 1 */
    assert(sscanf(test_string_3, "%lf %c", &dest, &sentinel) == 1);
    //printf("%c \n", sentinel);

    char test_string_4[] = " 1.1 E+1"; /* double and text */
    /* okay, returns 2 */
    assert(sscanf(test_string_4, "%lf %c", &dest, &sentinel) == 2);
    remove_space(test_string_4);
    assert(sscanf(test_string_4, "%lf %c", &dest, &sentinel) == 1);

    char test_string_5[] = " 1.1E+1"; /* double */
    /* okay, returns 1 */
    assert(sscanf(test_string_5, "%lf %c", &dest, &sentinel) == 1);
}

int main()
{
/* null */
    test_is_null();
/* bool */    
    test_is_bool();
/* double */    
    test_is_double();
/* sscanf function */
    test_sccanf();
}