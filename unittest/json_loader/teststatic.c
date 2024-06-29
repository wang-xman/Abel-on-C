/**
 * Unittest JSON loader
 * Test static functions
 */
#include <assert.h>
#include "../../src/json_loader.c"

void test_get_closing_symbol_by_opening()
{
    const char* test_opening = L_BRACE;
    char* test_closing = get_closing_symbol_by_opening(test_opening);
    assert(strcmp(test_closing, R_BRACE) == 0);

    test_closing = get_closing_symbol_by_opening(L_BRACKET);
    assert(strcmp(test_closing, R_BRACKET) == 0);
    assert(strcmp(test_closing, L_BRACKET) != 0);
    assert(strcmp(test_closing, L_BRACE) != 0);

    test_closing = get_closing_symbol_by_opening(L_PARENTHESIS);
    assert(strcmp(test_closing, R_PARENTHESIS) == 0);
    assert(strcmp(test_closing, R_BRACKET) != 0);
}

/**
 * Test file test_matching.txt contains
 * 
 * { "num": [10, "Halo"] }
 * 
 * has the following token vector
 * 0 th token literal is 0, scheme is 0, level is 0, parent key is ROOT_KEY_ 
   1 th token literal is {, scheme is 0, level is 0, parent key is 0 
   2 th token literal is num, scheme is 1, level is 1, parent key is 0 
   3 th token literal is [, scheme is 0, level is 1, parent key is num 
   4 th token literal is 0, scheme is 0, level is 2, parent key is num 
   5 th token literal is 10, scheme is 2, level is 2, parent key is 0, termimal type is DOUBLE_TERM 
   6 th token literal is 1, scheme is 0, level is 2, parent key is num 
   7 th token literal is Halo, scheme is 1, level is 2, parent key is 1, termimal type is STRING_TERM 
   8 th token literal is ], scheme is 0, level is 1, parent key is num 
   9 th token literal is }, scheme is 0, level is 0, parent key is 0
 */

/**
 * Test JSON is the following
 * { 
 *    "num": [10, "Halo"]
 * }
 * Matching tokens: (#1, #9) the {}, and (#3, #8) the []
 */
void test_is_matched_closing()
{
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    abel_parse_file(&test_parser, "test_matching.txt");

    //json_token_ptr ptr_token;
    //size_t token_vector_size = abel_vector_size(&test_parser.token_vector);

    // Matching tokens: (#1, #9) the {}
    json_token_ptr dict_opening
            = abel_vector_at(&test_parser.token_vector, 1).pointer;
    json_token_ptr dict_closing
            = abel_vector_at(&test_parser.token_vector, 9).pointer;
    // Matching tokens: (#3, #8) the []
    json_token_ptr list_opening
            = abel_vector_at(&test_parser.token_vector, 3).pointer;
    json_token_ptr list_closing
            = abel_vector_at(&test_parser.token_vector, 8).pointer;
    assert( is_matched_closing(dict_closing, dict_opening) );
    assert( is_matched_closing(list_closing, list_opening) );
    // non matching
    assert( !is_matched_closing(dict_closing, list_opening) );
    assert( !is_matched_closing(list_closing, dict_opening) );

    abel_free_json_parser(&test_parser);
}

void test_get_token_ptr(void)
{
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    abel_parse_file(&test_parser, "test_matching.txt");

    json_token_ptr ptr_token;
    //size_t token_vector_size = abel_vector_size(&test_parser.token_vector);
    // 5 th token literal is 10, scheme is 2, level is 2, parent key is 0, termimal type is DOUBLE_TERM 
    ptr_token = get_token_ptr(&test_parser.token_vector, 5);
    assert( strcmp(ptr_token->literal->ptr_array, "10") == 0 );
    assert(ptr_token->literal_scheme == 2);
    assert(ptr_token->level == 2);
    assert(strcmp(ptr_token->parent_key->ptr_array, "0") == 0);
    assert(ptr_token->terminal_type == DOUBLE_TERM);
    // 7 th token literal is Halo, scheme is 1, level is 2, parent key is 1, termimal type is STRING_TERM 
    ptr_token = get_token_ptr(&test_parser.token_vector, 7);
    assert( strcmp(ptr_token->literal->ptr_array, "Halo") ==0 );
    assert(ptr_token->literal_scheme == 1);
    assert(ptr_token->level == 2);
    assert(strcmp(ptr_token->parent_key->ptr_array, "1") == 0);
    assert(ptr_token->terminal_type != DOUBLE_TERM);
    assert(ptr_token->terminal_type == STRING_TERM);
    abel_free_json_parser(&test_parser);
}

void test_get_literal(void)
{
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    abel_parse_file(&test_parser, "test_matching.txt");

    //json_token_ptr ptr_token;
    char* literal;
    //size_t token_vector_size = abel_vector_size(&test_parser.token_vector);
    // 5 th token literal is 10, scheme is 2, level is 2, parent key is 0, termimal type is DOUBLE_TERM 
    literal = get_literal(&test_parser.token_vector, 5);
    assert( strcmp(literal, "10") == 0 );
    // 7 th token literal is Halo, scheme is 1, level is 2, parent key is 1, termimal type is STRING_TERM 
    literal = get_literal(&test_parser.token_vector, 7);
    assert( strcmp(literal, "Halo") ==0 );
    abel_free_json_parser(&test_parser);
}

int main(void)
{
    test_get_closing_symbol_by_opening();
    test_is_matched_closing();
    test_get_token_ptr();
    test_get_literal();
    return 0;
}