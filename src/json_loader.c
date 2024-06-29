/* Source json_loader */
#include "json_loader.h"

/* Return  */
static char* get_closing_symbol_by_opening(const char* opening_sym)
{
    char* closing_symbol;
    if (strcmp(opening_sym, L_BRACE) == 0) {
        closing_symbol = (char*)R_BRACE;
    } else if (strcmp(opening_sym, L_BRACKET) == 0) {
        closing_symbol = (char*)R_BRACKET;
    } else if (strcmp(opening_sym, L_PARENTHESIS) == 0) {
        closing_symbol = (char*)R_PARENTHESIS;
    } else if (strcmp(opening_sym, L_CHEVRON) == 0) {
        closing_symbol = (char*)R_CHEVRON;
    } else {
        // error
    }
    return closing_symbol;
}

static Bool is_matched_closing(struct json_token* ptr_closing_token,
                               struct json_token* ptr_opening_token)
{
    Bool matching_symbol = (strcmp(
            ptr_closing_token->literal->ptr_array,
            get_closing_symbol_by_opening(ptr_opening_token->literal->ptr_array)) == 0);
    Bool matching_level = (ptr_closing_token->level == ptr_opening_token->level);
    Bool matching_pk = (strcmp(
            ptr_closing_token->parent_key->ptr_array,
            ptr_opening_token->parent_key->ptr_array) == 0);
    return (matching_symbol && matching_level && matching_pk);
}

struct json_loader able_make_json_loader()
{
    struct json_loader loader;
    loader.current_index = 0;
    return loader;
}

/**
 * @brief Get a token pointer from token vector
 */
static struct json_token* get_token_ptr(
        struct abel_vector* ptr_token_vector, size_t idx)
{
    return (struct json_token*)abel_vector_at(ptr_token_vector, idx).pointer;
}

/**
 * @brief Get token pointer at current index
 * 
 * Current index is set by JSON loader
 * 
 * @todo not unit tested, not used
 */
static struct json_token* get_current_token_ptr(
        struct json_loader* ptr_loader, struct abel_vector* ptr_token_vector)
{
    return (struct json_token*)abel_vector_at(ptr_token_vector,
            ptr_loader->current_index).pointer;
}

/**
 * @brief Get the literal of the token
 * 
 * Returns the literal of the token a the given index
 */
static char* get_literal(struct abel_vector* ptr_token_vector, size_t idx)
{
    return get_token_ptr(ptr_token_vector, idx)->literal->ptr_array;
}

struct abel_dict* make_dict(struct json_loader* ptr_loader,
        int index_opening_token, struct abel_vector* ptr_token_vector)
{
    struct abel_dict* dict_sptr = abel_make_dict_ptr();
    // Prepare current index for iteration. Iteration start from next index.
    ptr_loader->current_index = index_opening_token + 1;
    while ( ptr_loader->current_index < (int) ptr_token_vector->size
            && !is_matched_closing(
                get_token_ptr(ptr_token_vector, ptr_loader->current_index),
                get_token_ptr(ptr_token_vector, index_opening_token)) ) {
        if (get_token_ptr(ptr_token_vector, ptr_loader->current_index)->type == KEY) {
            at_key(ptr_loader, dict_sptr, ptr_token_vector);
            continue;
        }
    }
    // Since when current index is pointing at a dict closing symbole, while
    // loop terminates, must update current index for the next root-level run.
    ptr_loader->current_index += 1;
    return dict_sptr;
}

struct abel_list* make_list(struct json_loader* ptr_loader,
        int index_opening_token, struct abel_vector* ptr_token_vector)
{
    struct abel_list* list_sptr = abel_make_list_ptr(0);
    ptr_loader->current_index = index_opening_token + 1;
    while ( ptr_loader->current_index < (int)ptr_token_vector->size
            && !is_matched_closing(
                get_token_ptr(ptr_token_vector, ptr_loader->current_index),
                get_token_ptr(ptr_token_vector, index_opening_token)) ) {
        if (get_token_ptr(ptr_token_vector, ptr_loader->current_index)->type == ITER_KEY) {
            at_iter_key(ptr_loader, list_sptr, ptr_token_vector);
            continue;
        }
    }
    ptr_loader->current_index += 1;
    return list_sptr;
}

/*
    Template method that sets an accepted JSON terminal type
    into the given container passsed in as the first argument
*/
static void set_terminal_in_list(struct abel_list* ptr_list,
        size_t iter_key, struct json_token* ptr_token)
{
    char* value = ptr_token->literal->ptr_array;
    if (ptr_token->terminal_type == NULL_TERM) {
        abel_list_append( ptr_list, as_null(value) );
    } else if (ptr_token->terminal_type == BOOL_TERM) {
        abel_list_append( ptr_list, as_bool(value) );
    } else if (ptr_token->terminal_type == DOUBLE_TERM) {
        abel_list_append( ptr_list, as_double(value) );
    } else {    // otherwise, set as string
        abel_list_append(ptr_list, value);
    }
}

static void set_terminal_in_dict(struct abel_dict* ptr_dict, char* key,
                                 struct json_token* ptr_token)
{
    char* value = ptr_token->literal->ptr_array;
    if (ptr_token->terminal_type == NULL_TERM) {
        abel_dict_insert_null( ptr_dict, key, as_null(value) );
    } else if (ptr_token->terminal_type == BOOL_TERM) {
        abel_dict_insert_bool( ptr_dict, key, as_bool(value) );
    } else if (ptr_token->terminal_type == DOUBLE_TERM) {
        abel_dict_insert_double( ptr_dict, key, as_double(value) );
    } else {    // otherwise, set as string
        abel_dict_insert_string(ptr_dict, key, value);
    }
}

/*
    Inside a dictionary scope, only key-value pairs.
    This means the only possibilities are
        case 1, key : terminal
        case 2, key : {}
        case 3, key : []
        case 4, key : () not for JSON
    determine key and value
*/
void fill_dict(struct json_loader* ptr_loader, struct abel_dict* ptr_dict,
               char* key, struct abel_vector* ptr_token_vector)
{
    int next_index = ptr_loader->current_index + 1;
    enum json_token_type next_token_type
            = get_token_ptr(ptr_token_vector, next_index)->type;
    if (next_token_type == TERMINAL) { // Case 1, next token is a terminal
        // set terminal value
        set_terminal_in_dict( ptr_dict, key,
                              get_token_ptr(ptr_token_vector, next_index) );
        // shift current index to point at next iter key token
        ptr_loader->current_index += 2;
    } else if (next_token_type == DICT_OPENING) { // next token is dict opening.
        // build a dict recursively
        struct abel_dict* ptr_subdict
                = make_dict(ptr_loader, next_index, ptr_token_vector);
        abel_dict_insert( ptr_dict, key,
                          abel_make_object_ptr_from_dict_ptr(ptr_subdict) );
    } else if (next_token_type == LIST_OPENING) { // next token is list opening.
        // build a list
        struct abel_list* ptr_sublist
                = make_list(ptr_loader, next_index, ptr_token_vector);
        abel_dict_insert( ptr_dict, key,
                          abel_make_object_ptr_from_list_ptr(ptr_sublist) );
    } else { /* TODO */ }
}

/**
 * @brief Fill the targe list with given token
 * 
 * Called by `at_iter_key`. The current index in token
 * vector is pointing at an iter key. There are only 3
 * possibilities:
 *     - iter_key : Terminal token
 *     - iter_key : { (dict opening token)
 *     - iter_key : [ (list opening token)
 * 
 * The following list contains all 3 possibilities
 *     [ "Halo", {"a":1}, [7, 9] ]
 * where the first iter key is followed by a terminal,
 * the 2nd by a dict opening, the 3rd by list opening.
 * 
 * At an iter key, what action to take is determined by
 * the NEXT token.
 */
void fill_list(struct json_loader* ptr_loader, struct abel_list* ptr_target_list,
               int key, struct abel_vector* ptr_token_vector)
{
    int next_index = ptr_loader->current_index + 1;
    // get (pointer to) token at next index
    struct json_token* ptr_token
            = (struct json_token*)get_token_ptr(ptr_token_vector, next_index);
    enum json_token_type next_token_type
            = get_token_ptr(ptr_token_vector, next_index)->type;
    if (next_token_type == TERMINAL) { // next token is a terminal    
        set_terminal_in_list(ptr_target_list, key, ptr_token);
        // shift current index to point at next iter key token
        ptr_loader->current_index += 2;
    } else if (next_token_type == DICT_OPENING) { // next token is dict opening.
        // build a dict recursively
        struct abel_dict* ptr_subdict
                = make_dict(ptr_loader, next_index, ptr_token_vector);
        abel_list_append(ptr_target_list, ptr_subdict);
    } else if (next_token_type == LIST_OPENING) { // next token is list opening.
        // build a sublist
        struct abel_list* ptr_sublist
                = make_list(ptr_loader, next_index, ptr_token_vector);
        abel_list_append(ptr_target_list, ptr_sublist);
    } else { /* TODO */ }
}

/* Operations at a key token */
void at_key(struct json_loader* ptr_loader, struct abel_dict* dict_sptr_ref,
            struct abel_vector* token_vector)
{
    int current_index = ptr_loader->current_index;
    char* key = get_literal(token_vector, current_index);
    fill_dict(ptr_loader, dict_sptr_ref, key, token_vector);
}

/**
 * @brief Operations at an inter-key token
 * 
 * This method is called by `make_root_list` and `make_list`
 * to populate the target list with the tokens from the
 * token vector.
 * 
 * @param ptr_loader Pointer to the current JSON loader.
 * @param ptr_target_list Pointer to the target list to
 *        be filled up.
 */
void at_iter_key(struct json_loader* ptr_loader,
        struct abel_list* ptr_target_list, struct abel_vector* ptr_token_vector)
{
    int current_index = ptr_loader->current_index;
    char* literal = get_literal(ptr_token_vector, current_index);
    int iter_key = atoi(literal);  // convert to int
    // Populate target list with the token
    fill_list(ptr_loader, ptr_target_list, iter_key, ptr_token_vector);
}

/**
 * @brief Construct the root dictionary
 * 
 * Token vector traversing starts from this method.
 * 
 * For this method, current index must always point at an
 * key token.
 * @todo Just replace pointer to token vector by the pointer
 *       to the JSON parser, to unify the APIs.
 **/
struct abel_dict* make_root_dict(struct json_loader* ptr_loader,
        struct abel_vector* ptr_token_vector)
{
    struct abel_dict* ptr_root_dict = abel_make_dict_ptr();
    // traversing the token vector
    while(ptr_loader->current_index < (int)ptr_token_vector->size) {
        // For this method, current index shall always point at a key token.
        struct json_token* ptr_token
                = get_token_ptr(ptr_token_vector, ptr_loader->current_index);
        if (ptr_token->type == KEY) {
            at_key(ptr_loader, ptr_root_dict, ptr_token_vector);
            continue;
        }
    }
    return ptr_root_dict;
}

/** 
 * @brief Construct the root list
 * 
 * Traversing of token vector starts here.
 * 
 * For this method, current index must always point at
 * an iter-key token.
 * 
 * @param ptr_loader Pointer to the JSON loader.
 * @param ptr_token_vector Pointer to the token vector of
 *        the JSON parser.
 * 
 * @todo Just replace pointer to token vector by the pointer
 *       to the JSON parser, to unify the APIs.
 **/
struct abel_list* make_root_list(struct json_loader* ptr_loader,
                                 struct abel_vector* ptr_token_vector)
{
    struct abel_list* ptr_root_list = abel_make_list_ptr(0);
    while(ptr_loader->current_index < (int)ptr_token_vector->size) {
        struct json_token* ptr_token
                = get_token_ptr(ptr_token_vector, ptr_loader->current_index);
        if (ptr_token->type == ITER_KEY) {
            at_iter_key(ptr_loader, ptr_root_list, ptr_token_vector);
            continue;
        }
    }
    return ptr_root_list;
}

/**
 * User provides a global dictionary in order to
 * accommodate key "ROOT_KEY_" such that
 *     {
 *         "ROOT_KEY_": [
 *             CONTENT_IN_FILE
 *         ]
 *     }
 */
void load_from_parser(struct json_loader* ptr_loader,
        struct json_parser* ptr_parser, struct abel_dict* ptr_global_dict)
{
    ptr_loader->root_container_type
            = *(enum json_container_type*)(abel_vector_at(&(ptr_parser->current_container_type), 0).pointer);
    if (ptr_loader->root_container_type == DICT) {
        abel_dict_insert_dict_ptr(ptr_global_dict, "ROOT_KEY_",
            make_root_dict(ptr_loader, &(ptr_parser->token_vector)) );
    } else if (ptr_loader->root_container_type == LIST) {
        abel_dict_insert(ptr_global_dict, "ROOT_KEY_",
            abel_make_object_ptr(make_root_list(ptr_loader, &(ptr_parser->token_vector))) );
    } else {
        // error
    }
}