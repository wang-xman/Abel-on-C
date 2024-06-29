/**
 * Header json_parser.h
 * Created 12-11-2022
 * Updated 03-01-2022
 **/
#ifndef ABEL_ON_C_JSON_PARSER_H
#define ABEL_ON_C_JSON_PARSER_H

#include "converter.h"    // has util.h
#include "json_token.h"

/**
 * @brief Json parser struct
 * 
 * Json parser parses the input file char by char. The
 * parsing rules are implemented via operational and
 * workflow functions.
 * 
 * The most important field of a Json parser is the token
 * vector that stores pointers to tokens (created on heap).
 * 
 * Fields
 * 
 * token_vector : A struct abel_vector instance used to store tokens.
 *     Token vector doesn't store the token instances, but
 *     pointers to tokens created on heap. This field is
 *     resource holding. Inited to empty.
 * 
 * current_line : Line number of the char on the file that
 *     is being parsed. Note that line number in file start
 *     from 1, not 0. Inited to 0.
 *  
 * current_column : Column number of the char on the file
 *     that is being parsed. Note that column number in file
 *     starts from 1. Inited to 0.
 * current_literal : A struct abel_string instance. It is the literal
 *     that has been collected and it will be tokenized
 *     according to the context and symbols around it.
 *     Inited to empty string.
 * latest_symbol : A struct abel_string instance. The latest symbol
 *     that the parser has encountered. It is often used as
 *     an indicator to determine the type of the token the
 *     literal shall be tokenized into. Inited to empty string.
 * current_level : A size_t integer.
 * deepest_level : A size_t integer.
 * current_container_type : An vector of pointer of `JsonContainerType`.
 *     It stores the pointers to container type stored on
 *     heap.
 * current_iter_index : A vector of integers. It stores level
 *     wise iter(ation) index. The vector index indicates the
 *     level of the iterable scope; the integer stored at each
 *     level is continuously updated. Iter index at level 0 is
 *     always set to 0, even though the scope at level 0 is not
 *     necessarily iterable.
 * parent_key : A struct abel_vector instance. Each element stores the
 *     parent key for the current level. Inited to ["ROOT_KEY_"].
 * keys_per_level : A 2D vector to store keys at each level. Initialised to [[]].
 */
struct json_parser {
    struct abel_vector token_vector;    // init to []
    size_t current_line;    // init to 0
    size_t current_column;    // init to 0
    struct abel_string current_literal;    // init to ""
    struct abel_string latest_symbol;    // init to ""
    size_t current_level;    // init to 0
    size_t deepest_level;    // init to 0
    struct abel_vector current_container_type;    // init to NONE
    struct abel_vector current_iter_index;    // init to 0
    struct abel_vector parent_key;    // init to "ROOT_KEY_"
    struct abel_vector keys_per_level;    // init to [[]]
    struct abel_string latest_syntactic_operator;
    Bool is_escaping;    // init false
    Bool is_delimited_string_open;    // init to false
    enum literal_scheme current_literal_scheme;    // must be inited
};

/**
 * @brief Initalise a parser
 * 
 * It takes a pointer to JSON parser and initialises it.
 * 
 * @todo Handle errors.
 */
void abel_make_json_parser(struct json_parser* ptr_parser);

/**
 * @brief Parse a JSON file
 * 
 * @param ptr_parser Pointer to JSON parser.
 * @param file_name JSON file to be parsed.
 * 
 * @todo Function `parse_line` may return errors which must
 *       be handled.
 */
void abel_parse_file(struct json_parser* ptr_parser, char* file_name);

/**
 * @brief Free JSON parser
 * 
 * JSON parser has several fields that are resource-holding.
 * And, these fields must be freed. This freer is facilitated
 * by several static function defined in the source file.
 * 
 */
void abel_free_json_parser(struct json_parser* ptr_parser);

#endif