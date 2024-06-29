/**
 * SOURCE json_token.c
 * Created 09-11-2022
 * Updated 03-11-2022
 **/
#include "json_token.h"

/* JSON token type and associated strings */
const char* JsonTokenTypeString[] = {
    "KEY",
    "ITER_KEY",
    "TERMINAL",
    "DICT_OPENING",
    "DICT_CLOSING",
    "LIST_OPENING",
    "LIST_CLOSING",
    "UNKNOWN_TOKEN"
};

const char* get_json_token_type_str(enum json_token_type token_type)
{
    return JsonTokenTypeString[token_type];
}

/* JSON container type and associated strings */
const char* JsonContainerTypeString[] = {
    "NONE_CONTAINER",
    "LIST",
    "DICT",
    "UNKNONW_CONTAINER"
};

const char* get_json_container_type_str(enum json_container_type container_type)
{
    return JsonContainerTypeString[container_type];
}

/* Literal collection scheme types and strings */
const char* LiteralSchemeString[] = {
    "NONE_SCHEME",
    "DELIMITED",
    "LIBERAL"
};

const char* get_literal_scheme_str(enum literal_scheme scheme)
{
    return LiteralSchemeString[scheme];
}

const char* JsonTerminalTypeString[] = {
    "NONE_TERM",
    "BOOL_TERM",
    "NULL_TERM",
    "STRING_TERM",
    "INTEGER_TERM",
    "DOUBLE_TERM",
    "COMPLEX_TERM",
    "BINARY_TERM",
    "BITSTRING_TERM"
};
const char* get_json_terminal_type_str(enum json_terminal_type term_type)
{
    return JsonTerminalTypeString[term_type];
}

/* Tokenizers for various token types */

struct json_token tokenize_iter_key(
    struct abel_string* literal,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type,
    int iter_index)
{
    struct json_token token;
    token.literal = abel_make_string_ptr(literal->ptr_array);
    token.type = ITER_KEY;
    //token.parent_key = pk;
    token.parent_key = abel_make_string_ptr(pk->ptr_array);
    token.level = level;
    token.line = line;
    token.container_type = container_type;
    token.iter_index = iter_index;
    /* not used by iter key */
    token.terminal_type = NONE_TERM;
    token.literal_scheme = NONE_SCHEME;
    token.referenced_type = abel_make_string_ptr("");
    return token;
}

struct json_token tokenize_key(
    struct abel_string* literal,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type,
    enum literal_scheme scheme)
{
    struct json_token token;
    token.literal = abel_make_string_ptr(literal->ptr_array);
    token.type = KEY;
    token.parent_key = abel_make_string_ptr(pk->ptr_array);
    token.level = level;
    token.line = line;
    token.container_type = container_type;
    token.literal_scheme = scheme;
    /* not used by key */
    token.iter_index = 0;
    token.terminal_type = NONE_TERM;
    token.referenced_type = abel_make_string_ptr("");
    return token;
}

struct json_token tokenize_terminal(
    struct abel_string* literal,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type,
    enum json_terminal_type terminal_type,
    enum literal_scheme scheme)
{
    struct json_token token;
    token.literal = abel_make_string_ptr(literal->ptr_array);
    token.type = TERMINAL;
    token.parent_key = abel_make_string_ptr(pk->ptr_array);
    token.level = level;
    token.line = line;
    token.container_type = container_type;
    token.terminal_type = terminal_type;
    token.literal_scheme = scheme;
    /* not used by terminal */
    token.iter_index = 0;
    token.referenced_type = abel_make_string_ptr("");
    return token;
}

struct json_token tokenize(
    struct abel_string* literal,
    enum json_token_type type,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type)
{
    struct json_token token;
    token.literal = abel_make_string_ptr(literal->ptr_array);
    token.type = type;
    token.parent_key = abel_make_string_ptr(pk->ptr_array);
    token.level = level;
    token.line = line;
    token.container_type = container_type;
    token.referenced_type = abel_make_string_ptr("");
    /* not used in opening/closing token */
    token.iter_index = 0;
    token.terminal_type = NONE_TERM;
    token.literal_scheme = NONE_SCHEME;
    return token;
}

json_token_ptr abel_make_token_ptr(const struct json_token* ref_token)
{
    struct json_token* ptr_token = malloc( sizeof(*ptr_token) );
    /* copy pointer to the literal string */
    ptr_token->literal = ref_token->literal;
    ptr_token->type = ref_token->type;
    /* copy pointer to the parent-key string */
    ptr_token->parent_key = ref_token->parent_key;
    ptr_token->level = ref_token->level;
    ptr_token->line = ref_token->line;
    ptr_token->container_type = ref_token->container_type;
    ptr_token->iter_index = ref_token->iter_index;
    ptr_token->terminal_type = ref_token->terminal_type;
    ptr_token->literal_scheme = ref_token->literal_scheme;
    ptr_token->referenced_type = ref_token->referenced_type;
    return ptr_token;
}

void abel_free_json_token(struct json_token* ptr_token)
{
    abel_free_string_ptr(ptr_token->literal);
    abel_free_string_ptr(ptr_token->parent_key);
    abel_free_string_ptr(ptr_token->referenced_type);
}

void abel_free_json_token_ptr(json_token_ptr ptr_token)
{
    abel_free_string_ptr(ptr_token->literal);
    abel_free_string_ptr(ptr_token->parent_key);
    abel_free_string_ptr(ptr_token->referenced_type);
    free(ptr_token);
}

Bool is_opening_symbol(char* src_str)
{
    Bool ret = false;
    if (strcmp(src_str, L_BRACE) == 0 || strcmp(src_str, L_BRACKET) == 0
            || strcmp(src_str, L_PARENTHESIS) == 0)
    {
        ret = true;
    }
    return ret;
}

Bool is_closing_symbol(char* src_str)
{
    Bool ret = false;
    if (strcmp(src_str, R_BRACE) == 0 || strcmp(src_str, R_BRACKET) == 0
            || strcmp(src_str, R_PARENTHESIS) == 0)
    {
        ret = true;
    }
    return ret;
}


enum json_container_type get_container_type_by_symbol(char* opening_sym)
{
    enum json_container_type container_type;
    if (strcmp(opening_sym, L_BRACE) == 0) {
        container_type = DICT;
    } else if (strcmp(opening_sym, L_BRACKET) == 0) {
        container_type = LIST;
    } else {
        container_type = UNKNONW_CONTAINER;
    }
    return container_type;
}

enum json_token_type get_token_type_by_symbol(char* symbol_str)
{
    enum json_token_type token_type;
    if (strcmp(symbol_str, L_BRACE) == 0) {
        token_type = DICT_OPENING;
    } else if ( strcmp(symbol_str, R_BRACE) == 0) {
        token_type = DICT_CLOSING;
    } else if ( strcmp(symbol_str, L_BRACKET) == 0) {
        token_type = LIST_OPENING;
    } else if ( strcmp(symbol_str, R_BRACKET) == 0) {
        token_type = LIST_CLOSING;
    } else {
        token_type = UNKNOWN_TOKEN;
    }
    return token_type;
}