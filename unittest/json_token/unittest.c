#include <assert.h>
#include "json_token.h"

void test_get_token_type_str()
{
    enum json_token_type test_type = KEY;
    const char* type_str = get_json_token_type_str(test_type);
    assert(strcmp(type_str, "KEY") == 0);
}

void test_tokenize_iter_key()
{
    struct json_token test_token;
    struct abel_string literal = abel_make_string("0");
    struct abel_string pk = abel_make_string("mod");
    test_token = tokenize_iter_key(&literal, &pk, 0, 3, LIST, 0);
    assert(abel_string_eq_cstring(test_token.literal, "0") == true);
    assert(abel_string_eq_cstring(test_token.parent_key, "mod") == true);
    assert(test_token.type == ITER_KEY);
    assert(test_token.container_type == LIST);

    abel_free_json_token(&test_token);
    abel_free_string(&literal);
    abel_free_string(&pk);
}

void test_tokenize_key()
{
    struct json_token test_token;
    struct abel_string literal = abel_make_string("Root");
    struct abel_string pk = abel_make_string("mod");

    test_token = tokenize_key(&literal, &pk, 0, 3, LIST, LIBERAL);
    assert(abel_string_eq_cstring(test_token.literal, "Root") == true);
    assert(abel_string_eq_cstring(test_token.parent_key, "mod") == true);
    assert(test_token.type == KEY);
    assert(test_token.container_type == LIST);
    assert(test_token.literal_scheme == LIBERAL);

    abel_free_json_token(&test_token);
    abel_free_string(&literal);
    abel_free_string(&pk);
}

void test_tokenize_terminal()
{
    struct json_token test_token;
    struct abel_string literal = abel_make_string("Root");
    struct abel_string pk = abel_make_string("mod");
    test_token = tokenize_terminal(&literal, &pk, 0, 3, LIST, STRING_TERM, LIBERAL);
    assert(abel_string_eq_cstring(test_token.literal, "Root") == true);
    assert(abel_string_eq_cstring(test_token.parent_key, "mod") == true);
    assert(test_token.type == TERMINAL);
    assert(test_token.container_type == LIST);
    assert(test_token.literal_scheme == LIBERAL);

    abel_free_json_token(&test_token);
    abel_free_string(&literal);
    abel_free_string(&pk);
}

void test_token_ptr()
{
    struct json_token test_token;
    struct abel_string literal = abel_make_string("Root");
    struct abel_string pk = abel_make_string("mod");
    test_token = tokenize_terminal(&literal, &pk, 0, 3, LIST, STRING_TERM, LIBERAL);
    assert(abel_string_eq_cstring(test_token.literal, "Root") == true);
    assert(abel_string_eq_cstring(test_token.parent_key, "mod") == true);
    assert(test_token.type == TERMINAL);
    assert(test_token.container_type == LIST);
    assert(test_token.literal_scheme == LIBERAL);

    json_token_ptr ptr_token = abel_make_token_ptr(&test_token);
    assert(abel_string_eq_cstring(ptr_token->literal, "Root") == true);
    assert(abel_string_eq_cstring(ptr_token->parent_key, "mod") == true);
    assert(ptr_token->type == TERMINAL);
    assert(ptr_token->container_type == LIST);
    assert(ptr_token->literal_scheme == LIBERAL);

    /* Clean up */
    abel_free_string(&literal);
    abel_free_string(&pk);
    abel_free_json_token_ptr(ptr_token);
}

void test_get_token_type_by_symbol()
{
    char* test_str = "]";
    assert(get_token_type_by_symbol(test_str) == LIST_CLOSING);
    test_str = "{";
    assert(get_token_type_by_symbol(test_str) == DICT_OPENING);
    test_str = "}";
    assert(get_token_type_by_symbol(test_str) == DICT_CLOSING);
    assert(get_token_type_by_symbol(test_str) != LIST_CLOSING);
    test_str = "[";
    assert(get_token_type_by_symbol(test_str) == LIST_OPENING);
    test_str = "/";
    assert(get_token_type_by_symbol(test_str) == UNKNOWN_TOKEN);
    assert(get_token_type_by_symbol(test_str) != DICT_CLOSING);
    assert(get_token_type_by_symbol(test_str) != LIST_OPENING);
}

int main()
{
    test_get_token_type_str();
    test_tokenize_iter_key();
    test_tokenize_key();
    test_tokenize_terminal();
    test_token_ptr();
    test_get_token_type_by_symbol();
}