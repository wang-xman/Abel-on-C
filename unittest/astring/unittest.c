/* Unittest astring */
#include <assert.h>
#include "astring.h"

/**
 * @brief Test next power of two
 * 
 * This function is defined in generic.
 */
void test_next_power_of_two()
{
    unsigned int test_value = 0;
    unsigned int test_result = next_power_of_two(test_value);
    unsigned int expected = 1;
    assert(test_result == expected);

    /* input is 1, expect 1 = 2^0 */
    test_value = 1;
    expected = 1;
    assert(next_power_of_two(test_value) == expected);

    /*  input is 2, expect 2 = 2^1 */
    test_value = 2;
    expected = 2;
    assert(next_power_of_two(test_value) == expected);

    /* input is 3, expect 4 = 2^2 */
    test_value = 3;
    expected = 4;
    assert(next_power_of_two(test_value) == expected);

    /* input is 8, expect 8 = 2^3 */
    test_value = 8;
    expected = 8;
    assert(next_power_of_two(test_value) == expected);

    /* From 9 - 16, the expected is 16. */

    /* input is 9, expect 16 = 2^4 */
    test_value = 9;
    expected = 16;
    assert(next_power_of_two(test_value) == expected);
    /* input is 12, expect 16 = 2^4 */
    test_value = 12;
    assert(next_power_of_two(test_value) == expected);
    /* input is 14, expect 16 = 2^4 */
    test_value = 14;
    assert(next_power_of_two(test_value) == expected);
    /* input is 16, expect 16 = 2^4 */
    test_value = 16;
    assert(next_power_of_two(test_value) == expected);
}

void test_make_string()
{
    /* empty string */
    char* test_0 = "";
    struct abel_string text_0 = abel_make_string(test_0);
    assert(text_0.length == 0);
    assert(text_0.capacity == 4);
    free(text_0.ptr_array);

    /* 3 character */
    char* test_3 = "Kal";
    struct abel_string text_3 = abel_make_string(test_3);
    assert(text_3.length == 3);
    assert(text_3.capacity == 4);
    free(text_3.ptr_array);

    /* 4 character */
    char* test_4 = "Kale";
    struct abel_string text_4 = abel_make_string(test_4);
    assert(text_4.length == 4);
    assert(text_4.capacity == 8);
    free(text_4.ptr_array);

    /* 5 characters */
    char test_5[] = "Hello";
    struct abel_string text_5 = abel_make_string(test_5);
    assert(text_5.length == 5);
    assert(text_5.capacity == 8);
    assert(strlen(text_5.ptr_array) == 5);
    free(text_5.ptr_array);

    /* 28 characters */
    char test_28[] = "If you want to be happy, be!";
    struct abel_string text_28 = abel_make_string(test_28);
    assert(text_28.length == 28);
    assert(text_28.capacity == 32);
    assert(strlen(text_28.ptr_array) == 28);
    free(text_28.ptr_array);
}

void test_make_string_from_int()
{
    int test_0 = 0;
    struct abel_string text_0 = abel_string_from_int(test_0);
    assert(text_0.length == 1);
    assert(text_0.capacity == 4);
    assert(abel_string_eq_cstring(&text_0, "0") == true);
    free(text_0.ptr_array);

    /* 2 digit */
    int test_3 = 90;
    struct abel_string text_3 = abel_string_from_int(test_3);
    assert(text_3.length == 2);
    assert(text_3.capacity == 4);
    assert(abel_string_eq_cstring(&text_3, "90") == true);
    free(text_3.ptr_array);

    /* with plus sign : plus sign is removed */
    int test_4 = +777;
    struct abel_string text_4 = abel_string_from_int(test_4);
    assert(text_4.length == 3);    // note this length, plus sign is not here
    assert(text_4.capacity == 4);
    assert(abel_string_eq_cstring(&text_4, "777") == true);
    free(text_4.ptr_array);

    /* with minus sign */
    int test_5 = -888;    // 5 characters
    struct abel_string text_5 = abel_string_from_int(test_5);
    assert(text_5.length == 4);
    assert(text_5.capacity == 8);
    assert(abel_string_eq_cstring(&text_5, "-888") == true);
    free(text_5.ptr_array);
}

void test_make_string_ptr()
{
    /* 0 characters */
    char test_0[] = "";
    struct abel_string* ptr_text = abel_make_string_ptr(test_0);
    assert(ptr_text->length == 0);
    assert(ptr_text->capacity == 4);
    assert(strlen(ptr_text->ptr_array) == 0);
    free(ptr_text->ptr_array);
    free(ptr_text);

    /* 1 characters */
    char test_1[] = " ";
    ptr_text = abel_make_string_ptr(test_1);
    assert(ptr_text->length == 1);
    assert(ptr_text->capacity == 4);
    assert(strlen(ptr_text->ptr_array) == 1);
    free(ptr_text->ptr_array);
    free(ptr_text);

    /* 4 characters */
    char test_4[] = "1234";
    ptr_text = abel_make_string_ptr(test_4);
    assert(ptr_text->length == 4);
    assert(ptr_text->capacity == 8);
    assert(strlen(ptr_text->ptr_array) == 4);
    free(ptr_text->ptr_array);
    free(ptr_text);

    /* 16 characters */
    char test_16[] = "12b45d7j90e74y6s";
    ptr_text = abel_make_string_ptr(test_16);
    assert(ptr_text->length == 16);
    assert(ptr_text->capacity == 32);
    assert(strlen(ptr_text->ptr_array) == 16);
    free(ptr_text->ptr_array);
    free(ptr_text);
}

void test_assign_string()
{
    /* 5 characters */
    char* test_str = "Hello";
    struct abel_string* ptr_text = abel_make_string_ptr(test_str);
    assert(ptr_text->length == 5);
    assert(ptr_text->capacity == 8);

    char* test_str_2 = "Things have changed.";    // 20 chars
    abel_string_assign(ptr_text, test_str_2);
    assert(ptr_text->length == 20);
    assert(ptr_text->capacity == 32);

    assert(abel_string_eq_cstring(ptr_text, test_str_2) == true);

    abel_free_string_ptr(ptr_text);
}

void test_assign_string_char()
{
    /* 5 characters */
    char* test_str = "Hello";
    struct abel_string* ptr_text = abel_make_string_ptr(test_str);
    assert(ptr_text->length == 5);
    assert(ptr_text->capacity == 8);

    /* 20 chars */
    char test_str_2 = 'X';
    abel_string_assign_char(ptr_text, test_str_2);
    assert(ptr_text->length == 1);
    assert(ptr_text->capacity == 4);

    char converted[] = {'X', '\0'};

    assert(abel_string_eq_cstring(ptr_text, converted) == true);
    assert(abel_string_eq_char(ptr_text, test_str_2) == true);

    abel_free_string_ptr(ptr_text);
}

void test_append_string()
{
    /* 5 characters */
    char* test_str = "Hello";
    struct abel_string* ptr_text = abel_make_string_ptr(test_str);
    assert(ptr_text->length == 5);
    assert(ptr_text->capacity == 8);

    /* append 1 char, no maxcap change */
    char* test_str_2 = "W";
    abel_string_append(ptr_text, test_str_2);
    assert(ptr_text->length == 6);
    assert(strcmp(ptr_text->ptr_array, "HelloW") == 0);
    assert(ptr_text->capacity == 8);

    /* there is maxcap change */
    abel_string_append(ptr_text, "orld");
    assert(strcmp(ptr_text->ptr_array, "HelloWorld") == 0);
    assert(ptr_text->length == 10);
    assert(ptr_text->capacity == 16);

    /* append empty */
    abel_string_append(ptr_text, "");
    assert(strcmp(ptr_text->ptr_array, "HelloWorld") == 0);
    assert(ptr_text->length == 10);
    assert(ptr_text->capacity == 16);

    /* append 1 space */
    abel_string_append(ptr_text, " ");
    assert(strcmp(ptr_text->ptr_array, "HelloWorld ") == 0);
    assert(ptr_text->length == 11);
    assert(ptr_text->capacity == 16);    // next highest power of 2

    /* maxcap change */
    abel_string_append(ptr_text, "I am coming");
    assert(strcmp(ptr_text->ptr_array, "HelloWorld I am coming") == 0);
    assert(ptr_text->length == 22);
    assert(ptr_text->capacity == 32);    // next highest power of 2

    abel_free_string_ptr(ptr_text);
}

void test_append_char()
{
    /* 5 characters */
    char* test_str = "Hello";
    struct abel_string* ptr_str = abel_make_string_ptr(test_str);
    assert(ptr_str->length == 5);
    assert(ptr_str->capacity == 8);

    abel_string_append_char(ptr_str, 'X');
    assert(abel_string_eq_cstring(ptr_str, "HelloX") == true);

    abel_free_string_ptr(ptr_str);
}

void test_compare_strings()
{
    /* 5 characters */
    char* test_cstr = "Hello";
    struct abel_string* ptr_str = abel_make_string_ptr(test_cstr);
    assert(abel_string_eq_cstring(ptr_str, test_cstr) == true);

    assert(abel_cstring_eq("Hello", test_cstr) == true);

    char test_char = 'X';
    char test_str_converted[] = {test_char, '\0'};

    assert(abel_cstring_eq_char(test_str_converted, 'X') == true);
    assert(abel_cstring_eq_char(test_str_converted, test_char) == true);

    abel_free_string_ptr(ptr_str);
}

void test_get_char()
{
    /* 5 characters */
    char* test_cstr = "Hello";
    struct abel_string string = abel_make_string(test_cstr);
    assert(abel_string_at(&string, 0) == 'H');
    assert(abel_string_at(&string, 1) == 'e');
    assert(abel_string_at(&string, 4) == 'o');
    assert(abel_string_at(&string, 4) != '0');
    assert(abel_string_at(&string, 5) == '\0');

    abel_free_string(&string);
}

void test_first_char()
{
    struct abel_string string = abel_make_string("process");
    assert(abel_string_front(&string) == 'p');

    abel_free_string(&string);
}

void test_last_char()
{
    struct abel_string string = abel_make_string("proceS");
    assert(abel_string_front(&string) == 'p');
    assert(abel_string_back(&string) == 'S');

    abel_free_string(&string);
}

void test_has_char()
{
    struct abel_string string = abel_make_string("proceS");
    assert(abel_string_has(&string, 'p') == true);
    assert(abel_string_has(&string, 'r') == true);
    assert(abel_string_has(&string, 'o') == true);
    assert(abel_string_has(&string, 'c') == true);
    assert(abel_string_has(&string, 'e') == true);
    assert(abel_string_has(&string, ' ') == false);

    abel_free_string(&string); 
}

int main()
{
    test_next_power_of_two();

/* constructors */
    test_make_string();
    test_make_string_from_int();
    test_make_string_ptr();

/* string assign */
    test_assign_string();
    test_assign_string_char();

/* string append */
    test_append_string();
    test_append_char();

/* string compare */
    test_compare_strings();

/* get char */
    test_get_char();
    test_first_char();
    test_last_char();
    test_has_char();
}