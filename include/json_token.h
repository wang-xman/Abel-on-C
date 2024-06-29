/**
 * Header json_token.h
 * Created 09-11-2022
 * Updated 03-12-2022
 * 
 * JSON token related types, strings, and util functions.
 **/
#ifndef ABEL_ON_C_JSON_TOKEN_H
#define ABEL_ON_C_JSON_TOKEN_H

#include "symbol.h"

/* JSON token type and associated strings */
enum json_token_type {
    KEY,
    ITER_KEY,
    TERMINAL,
    DICT_OPENING,
    DICT_CLOSING,
    LIST_OPENING,
    LIST_CLOSING,
    UNKNOWN_TOKEN
};
extern const char* JsonTokenTypeString[];
const char* get_json_token_type_str(enum json_token_type token_type);

/* JSON container type and associated strings */
enum json_container_type {
    NONE_CONTAINER,
    LIST,
    DICT,
    UNKNONW_CONTAINER
};
extern const char* JsonContainerTypeString[];
const char* get_json_container_type_str(enum json_container_type container_type);

/* Literal collection scheme types and strings */
enum literal_scheme {
    NONE_SCHEME,
    DELIMITED,
    LIBERAL
};
extern const char* LiteralSchemeString[];
const char* get_literal_scheme_str(enum literal_scheme scheme);

/* JSON terminal types and associated strings */
enum json_terminal_type {
    NONE_TERM,
    BOOL_TERM,
    NULL_TERM,
    STRING_TERM,
    INTEGER_TERM,
    DOUBLE_TERM,
    COMPLEX_TERM,
    BINARY_TERM,
    BITSTRING_TERM
};
extern const char* JsonTerminalTypeString[];
const char* get_json_terminal_type_str(enum json_terminal_type term_type);

/**
 * @brief JSON token struct
 * 
 * JSON token instance and pointers are primarily used
 * internally by the parser and loader.
 */
struct json_token {
    struct abel_string* literal;        // holds resource
    enum json_token_type type;
    struct abel_string* parent_key;     // holds resource
    int level;
    int line;
    enum json_container_type container_type;
    int iter_index;
    enum json_terminal_type terminal_type; 
    enum literal_scheme literal_scheme;
    struct abel_string* referenced_type; // holds resource
};
typedef struct json_token* json_token_ptr;

/**
 * @brief Tokenizer functions
 * 
 * For iter_key, key, and terminal tokens, dedicated
 * tokenizer are available.
 * 
 * @note Arguments `literal` and `pk` will be copied
 *       onto heap, as token holds resources. Otherwise,
 *       multipled ownership will cause trouble. The
 *       side effect of such copying is that two memory
 *       locations must be freed at the end.
 */
struct json_token tokenize_iter_key(
    struct abel_string* literal,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type,
    int iter_index);

struct json_token tokenize_key(
    struct abel_string* literal,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type,
    enum literal_scheme scheme);

struct json_token tokenize_terminal(
    struct abel_string* literal,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type,
    enum json_terminal_type terminal_type,
    enum literal_scheme scheme);

/* only for none of the above */
struct json_token tokenize(
    struct abel_string* literal,
    enum json_token_type type,
    struct abel_string* pk,
    int level,
    int line,
    enum json_container_type container_type);

/**
 * @brief Maker a token instance on heap
 * 
 * Copies all fields of the reference token to heap.
 */
json_token_ptr abel_make_token_ptr(const struct json_token* ref_token);

/**
 * @brief JSON token freer
 */
void abel_free_json_token(struct json_token* ref_token);

/**
 * @brief JSON token pointer freer
 */
void abel_free_json_token_ptr(json_token_ptr ptr_token);

/**
 * Token related utility functions
 **/
Bool is_opening_symbol(char* src_str);

Bool is_closing_symbol(char* src_str);

/**
 * @brief Container type by opening symbol
 * 
 * Returns container type, either Dict or List, by opening
 * symbol only. JSON format accepts only two types of
 * containers, list or dict.
 * 
 * @note Only by opening symbol, or returns UNKNOWN_CONTAINER.
 */
enum json_container_type get_container_type_by_symbol(char* opening_sym);

/**
 * @brief Container opening (closing) by symbol
 * 
 * Returns container opening (clsoing) token type string
 * by opening or closing symbol.
 * 
 * @note This function is not for all tokens.
 */
enum json_token_type get_token_type_by_symbol(char* symbol_str);

#endif