/**
 * Unittest JSON parser
 * @created 25-11-2022
 * @updated 25-11-2022
 */
#include <assert.h>
#include "json_parser.h"

void test_json_parser_maker()
{
    struct json_parser test_parser; 
    abel_make_json_parser(&test_parser);

    assert(abel_vector_size(&test_parser.token_vector) == 0);
    assert(abel_vector_size(&test_parser.parent_key) == 1);

    abel_free_json_parser(&test_parser);
}

/**
 * @brief Test parse_file function
 * 
 * @note Default directory is the one where this unittest
 *       file locates.
 */
void test_parse_file()
{
    struct json_parser test_parser;
    abel_make_json_parser(&test_parser);
    assert(abel_vector_size(&test_parser.token_vector) == 0);
    assert(abel_vector_size(&test_parser.parent_key) == 1);

    abel_parse_file(&test_parser, "test.txt");
    
    size_t token_vector_size = abel_vector_size(&test_parser.token_vector);

    json_token_ptr ptr_token;

    for (size_t i = 0; i< token_vector_size; i++) {
        ptr_token = abel_vector_at(&test_parser.token_vector, i).pointer;
        if (ptr_token->terminal_type != NONE_TERM) {
            printf("%ld th token literal is %s, scheme is %d, "
               "level is %d, parent key is %s, termimal type is %s \n",
                i, ptr_token->literal->ptr_array, ptr_token->literal_scheme,
                   ptr_token->level, ptr_token->parent_key->ptr_array,
                   JsonTerminalTypeString[ptr_token->terminal_type]);
        } else {
            printf("%ld th token literal is %s, scheme is %d, "
               "level is %d, parent key is %s \n",
                i, ptr_token->literal->ptr_array, ptr_token->literal_scheme,
                   ptr_token->level, ptr_token->parent_key->ptr_array);
        }
    }
    int root_cnt_type = *(enum json_container_type*)(abel_vector_at(&(test_parser.current_container_type), 0).pointer);
    printf("Root container type is %d\n", root_cnt_type);
    printf("*** 0: NONE, 1: LIST, 2: DICT, 3: UNKNOWN ***\n");
    // freer
    abel_free_json_parser(&test_parser);
}

int main(void)
{
/* parser maker */
    test_json_parser_maker();

/* parse file */
    test_parse_file();
}