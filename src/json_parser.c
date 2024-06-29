/**
 * Source json_parser.c
 * Created 12-11-2022
 * Updated 27-12-2022
 * 
 * Static functions are required to operate on the internal
 * vectors of the parser.
 **/
#include "json_parser.h"

/** 
 * Static functions for token vector
 * 
 * @brief Token vector stores pointer to tokens stored
 *        on heap. Actual operations in the following
 *        functions are delegated to the ones for vector.
 * 
 * token_vector_size : Returns the size of token vector.
 *     Size is the total number of tokens stored in the
 *     token vector.
 * 
 * token_vector_push_back : Pushes back a new token into
 *     token vector.
 * 
 * token_vector_at : Returns a pointer to the token at
 *     given index.
 * 
 * token_vector_last : Returns the last token (pointer to
 *     token) in the vector.
 * 
 * free_token_vector : Releases all resource held by the
 *     token vector.
 **/

/**
 * @brief Token vector - size.
 * 
 * Size of a token vector is the total number of token
 * (pointers) stored in the vector.
 * 
 * @param ptr_parser Pointer to the parser.
 * @return Total number of tokens (pointers) stored in
 *     vector.
 */
static size_t token_vector_size(const struct json_parser* ptr_parser)
{
    return abel_vector_size( (struct abel_vector*)&ptr_parser->token_vector );
}

/**
 * @brief Token vector - pushes back new token.
 * 
 * A token instance is copied onto to heap and the pointer
 * to that on-heap instance is stored in token vector.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param ref_token Pointer to the token to be copied and
 *                  stored in token vector.
 * @return struct abel_return_option instance.
 *         - If success, is_okay is true and pointer is the
 *           to the newly added token pointer.
 *         - If failure, is_error is true and error contains
 *           the error instance.
 */
static struct abel_return_option token_vector_push_back(
        struct json_parser* ptr_parser, struct json_token* ref_token)
{
    struct abel_return_option ret;
    json_token_ptr ptr_token = abel_make_token_ptr(ref_token);
    ret = abel_vector_append(&ptr_parser->token_vector, ptr_token);
    if (ret.is_okay == true) {
        ret.pointer = ptr_token;
    }
    return ret;
}

/**
 * @brief Token vector - returns a JSON token pointer.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param idx Index of the token in the vector.
 * @return A JSON token pointer is returned. If the index
 *         is out of range, a NULL pointer is returned. 
 */
static json_token_ptr token_vector_at(struct json_parser* ptr_parser, size_t idx)
{
    struct abel_return_option ret
            = abel_vector_at(&ptr_parser->token_vector, idx);
    json_token_ptr ptr_token;
    if (ret.is_okay == true) {
        ptr_token = abel_vector_at(&ptr_parser->token_vector, idx).pointer;
    } else {
        ptr_token = NULL;
    }
    return ptr_token;
}

/**
 * @brief Token vector - returns the last token
 * 
 * @param ptr_parser Pointer to the parser.
 * @return Pointer to the last token in the vector.
 * @note If the last token doesn't exist, in for example
 *       an empty vector, a NULL pointer is returned.
 */
static json_token_ptr token_vector_last(struct json_parser* ptr_parser)
{
    struct abel_return_option ret = abel_vector_back(&ptr_parser->token_vector);
    json_token_ptr ptr_token;
    if (ret.is_okay == true) {
        ptr_token = abel_vector_back(&ptr_parser->token_vector).pointer;
    } else {
        ptr_token = NULL;
    }
    return ptr_token;
}

/**
 * @brief Token vector - freer.
 *
 * Frees all resources held by the token vector.
 * 
 * @param ptr_parser Pointer to the parser.
 * @note Since all tokens are freed by whilst freeing this
 *       vector, one must NOT free them again whilst freeing
 *       keys-per-level vector. Otherwise, error in freeing
 *       memories that are not to be freed.
 */
static void free_token_vector(struct json_parser* ptr_parser)
{
    size_t token_vector_len = token_vector_size(ptr_parser);
    json_token_ptr ptr_token = NULL;
    for (int i = 0; i < token_vector_len; i++) {
        ptr_token = token_vector_at(ptr_parser, i);
        if (ptr_token != NULL) abel_free_json_token_ptr(ptr_token);
    }
    abel_free_vector(&ptr_parser->token_vector);
}

/**
 * Static functions for current literal string
 * 
 * @brief Current literal is a `struct abel_string` instance. Actual
 *        operations are delegated to the functions for
 *        struct abel_string.
 * 
 * literal_assign : Replaces the content of the current
 *     literal by the new C string.
 * 
 * literal_append : Appends a C string to the existing
 *     literal.
 * 
 * literal_reset : Resets current literal to an empty.
 * 
 * literal_is_empty : Checks if the current literal is
 *     an empty string. Returns true/false if it is/not.
 * 
 * free_literal : Frees the resource held by current
 *     literal string.
 **/

/**
 * @brief Literal - assigns a new content.
 * 
 * Assigns the given C string to the current literal.
 * The current content in the literal is replaced by
 * the new.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param str C string to be assigned to the current
 *            literal.
 * 
 * @note It calls `abel_string_assign` to perform the
 *       operation, which deletes the frees the current
 *       content.
 */
static void literal_assign(struct json_parser* ptr_parser, const char* str)
{
    abel_string_assign(&ptr_parser->current_literal, (char*)str);
}

/**
 * @brief Literal - appends to current literal.
 * 
 * Appends the given C string to the current literal, which
 * extends the current literal.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param str C string to be assigned to the current
 *            literal.
 * @note `abel_string_append` in `astring.c`.
 * @todo Does it throw error
 */
static void literal_append(struct json_parser* ptr_parser, char* str)
{
    abel_string_append(&ptr_parser->current_literal, str);
}

/**
 * @brief Literal - resets current literal.
 * 
 * Resets the current literal to empty. It calls
 * `abel_string_assign` to assign an empty string
 * and deletes (frees) the current content.
 * 
 * @param ptr_parser Pointer to the parser.
 */
static void literal_reset(struct json_parser* ptr_parser)
{
    abel_string_assign(&ptr_parser->current_literal, "");
}

/**
 * @brief Literal - is empty.
 * 
 * Returns true (false) is the current literal is (not)
 * an empty string.
 * 
 * @param ptr_parser Pointer to the parser.
 */
static Bool literal_is_empty(const struct json_parser* ptr_parser)
{
    return abel_string_eq_cstring(&ptr_parser->current_literal, "");
}

/**
 * @brief Literal - freer.
 * 
 * Frees the resource held by the current literal struct abel_string
 * instance.
 * 
 * @param ptr_parser Pointer to the parser.
 */
static void free_literal(struct json_parser* ptr_parser)
{
    abel_free_string(&ptr_parser->current_literal);
}

/**
 * @brief Latest symbol - freer.
 * 
 * Free `parser.latest_symbol` field which is a struct abel_string
 * instance. Note that last symbol is a struct abel_string instance.
 * 
 * @param ptr_parser Pointer to the parser.
 */
static void free_latest_symbol(struct json_parser* ptr_parser)
{
    abel_free_string(&ptr_parser->latest_symbol);
}

/**
 * Static functions for current-container type vector
 * 
 * @brief Use `cct_vector_` prefix in all static functions.
 *        Note that this vector stores pointers to the
 *        container type stored on heap.
 * 
 * cct_vector_init : Initialises vector to [NONE_CONTAINER].
 * 
 * cct_vector_size : Returns the size of the vector.
 * 
 * cct_vector_push_back : Pushes back a new container type
 *     into the vector.
 * 
 * cct_vector_emplace : Places a container type at the given
 *     index for the given level. The previous value is freed.
 * 
 * cct_vector_at : Returns the container type at the given
 *     index.
 * 
 * set_root_container_type : Manually sets the container
 *     type at root level, i.e. level 0.
 * 
 * get_current_container_type : Returns the container type
 *     at the current level.
 * 
 * get_parent_container_type : Returns the container type
 *     at the parent level (to the current one).
 * 
 * is_current_container_iterable : Checks if the container
 *     at current level is a list.
 * 
 * free_cct_vector : Frees all resourced held by this vector.
 **/

/**
 * @brief Current container type vector - init.
 * 
 * Initialise current container vector (cct) to
 * [NONE_CONTAINER]
 * 
 * @param ptr_parser Pointer to the parser.
 */
static void cct_vector_init(struct json_parser* ptr_parser)
{
    enum json_container_type* ptr_ct = malloc( sizeof(*ptr_ct) );
    *ptr_ct = NONE_CONTAINER;
    abel_vector_append(&ptr_parser->current_container_type, ptr_ct);
}

/**
 * @brief Current container type vector - size.
 *
 * @param ptr_parser Pointer to the parser.
 * @return Total number of elements in the vector. 
 */
static size_t cct_vector_size(struct json_parser* ptr_parser)
{
    return abel_vector_size(&ptr_parser->current_container_type);
}

/**
 * @brief Current container type vector - pushes back.
 * 
 * Pushes back a new container type into the vector.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param type JSON container type.
 */
static void cct_vector_push_back(struct json_parser* ptr_parser,
                                 const enum json_container_type type)
{
    enum json_container_type* ptr_cct = malloc( sizeof(*ptr_cct) );
    *ptr_cct = type;
    abel_vector_push_back(&ptr_parser->current_container_type, ptr_cct);
}

/**
 * @brief Current container type vector - emplace.
 * 
 * Emplace a container type at given level. The previous
 * content is freed.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param type JSON container type.
 */
static void cct_vector_emplace(struct json_parser* ptr_parser, size_t level, 
                               const enum json_container_type type)
{
    struct abel_return_option ret;
    enum json_container_type* ptr_cct = malloc( sizeof(*ptr_cct) );
    *ptr_cct = type;
    ret = abel_vector_emplace(&ptr_parser->current_container_type, level, ptr_cct);
    /* free the previous value */
    if (ret.is_okay && ret.pointer != NULL) {
        free(ret.pointer);
    }
}

/**
 * @brief Current container type vector - at.
 * 
 * Returns the container type at given level.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param level Level of which the container type is requested.
 */
static enum json_container_type cct_vector_at(struct json_parser* ptr_parser,
                                              size_t level)
{
    enum json_container_type type
            = *(enum json_container_type*)abel_vector_at(
                    &ptr_parser->current_container_type, level).pointer;
    return type;
}

/**
 * @brief Current container type - set root container type.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param type JSON container type for the root container.
 * @return An struct abel_return_option instance.
 *         - If no error, flag is_okay is true and pointer
 *           is NULL.
 *         - If error, flag is_okay is false and error stores
 *           the specific error instance.
 */
static struct abel_return_option set_root_container_type(
        struct json_parser* ptr_parser, const enum json_container_type type)
{
    struct abel_return_option ret;
    char errmsg[128] = "\0";
    struct abel_error err;
    if (type != NONE_CONTAINER) {
        enum json_container_type root = cct_vector_at(ptr_parser, 0);
        if (root == LIST || root == DICT) {
            strcat(errmsg, "Manual override of root contianer type is forbidden");
            err = error_parser_error(errmsg, ptr_parser->current_line);
            ret = abel_option_error(err);
        } else {
            cct_vector_emplace(ptr_parser, 0, type);
            ret = abel_option_okay(NULL);
        }
    } else {
        strcat(errmsg, "Unknown type for root container.");
        err = error_parser_error(errmsg, ptr_parser->current_line);
        ret = abel_option_error(err);
    }
    return ret;
}

/**
 * @brief Current container type vector - current level.
 * 
 * @param ptr_parser Pointer to the parser.
 * @return Container type at current level.
 */
static enum json_container_type get_current_container_type(
        struct json_parser* ptr_parser)
{
    return cct_vector_at(ptr_parser, ptr_parser->current_level);
}

/**
 * @brief Current container type vector - parent level.
 * 
 * @param ptr_parser Pointer to the parser.
 * @return Container type at parent level.
 */
static enum json_container_type get_parent_container_type(
        struct json_parser* ptr_parser)
{
    return cct_vector_at(ptr_parser, ptr_parser->current_level - 1);
}

/**
 * @brief Current container type vector - iterable container.
 * 
 * @param ptr_parser Pointer to the parser.
 * @return Returns true (false) if container at current level is
 *         (not) a LIST.
 */
static Bool is_current_container_iterable(struct json_parser* ptr_parser)
{
    return get_current_container_type(ptr_parser) == LIST;
}

/**
 * @brief Current container type vector - freer.
 * 
 * Frees the resource held by this vector.
 * 
 * @param ptr_parser Pointer to the parser.
 */
static void free_cct_vector(struct json_parser* ptr_parser)
{
    size_t cct_len = cct_vector_size(ptr_parser);
    enum json_container_type* ptr = NULL;
    for (int i = 0; i < cct_len; i++) {
        ptr = abel_vector_at(&ptr_parser->current_container_type, i).pointer;
        free(ptr);
    }
    abel_free_vector(&ptr_parser->current_container_type);
}

/**
 * Static functions for current iter-index vector
 * 
 * @brief Current iter-index is a vector that stores size_t
 *        integers. In the present design, it is a vector
 *        that stores pointers to integers on heap.
 * 
 * cii_vector_init : Current iter index vector is initialised
 *                   to [0].
 * 
 * cii_vector_size : Returns the size of the vector.
 * 
 * cii_vector_at : Returns the iter index (not pointer) at
 *     the given index (level).
 * 
 * cii_vector_append : Append a new iter-index, and hence at
 *     new level, to the vector.
 * 
 * cii_vector_emplace : Emplaces an iter index at the given
 *     level. The previous pointer is freed.
 * 
 * free_cii_vector : Frees the resource held by the vector.
 **/

/**
 * @brief Current iter-index vector - init.
 * 
 * Initialise this cii vector to [0].
 * 
 * @param ptr_parser Pointer to the parser.
 */
static void cii_vector_init(struct json_parser* ptr_parser)
{
    size_t* ptr_cii = malloc( sizeof(*ptr_cii) );
    *ptr_cii = 0;    // set the first element to 0
    abel_vector_append(&ptr_parser->current_iter_index, ptr_cii);
}

/**
 * @brief Current iter-index vector - size.
 * 
 * @param ptr_parser Pointer to the parser.
 * @return Total number of elements in the vector.
 */
static size_t cii_vector_size(struct json_parser* ptr_parser)
{
    return abel_vector_size(&ptr_parser->current_iter_index);
}

/**
 * @brief Current iter-index vector - at.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param level Level at which the iter index is requested.
 * @return Iter index at the given level.
 */
static size_t cii_vector_at(struct json_parser* ptr_parser, const size_t level)
{
    size_t iter_index
            = *(size_t*)abel_vector_at(&ptr_parser->current_iter_index,
                                       level).pointer;
    return iter_index;
}

/**
 * @brief Current iter-index vector - append.
 * 
 * @param ptr_parser Pointer to the parser.
 * @param iter_index The iter index to be appended to the
 *                   vector.
 * @note Appending an iter index to the vector means a new
 *       level has been added.
 */
static void cii_vector_append(struct json_parser* ptr_parser, size_t iter_index)
{
    size_t* ptr_cii = malloc( sizeof(*ptr_cii) );
    *ptr_cii = iter_index;
    abel_vector_append(&ptr_parser->current_iter_index, ptr_cii);
}

/**
 * @brief Current iter-index vector - emplace.
 * 
 * Emplaces an iter index at the given level.
 * 
 * @param ptr_parser Pointer to parser.
 * @param level Level at which the iter index is emplaced.
 * @param iter_index Iter index to be emplaced at (for) the
 *                   given level.
 */
static void cii_vector_emplace(struct json_parser* ptr_parser, size_t level, 
                               size_t iter_index)
{
    struct abel_return_option ret;
    size_t* ptr_cii = malloc( sizeof(*ptr_cii) );
    *ptr_cii = iter_index;
    ret = abel_vector_emplace(&ptr_parser->current_iter_index, level, ptr_cii);
    /* free the previous one */
    if (ret.is_okay && ret.pointer != NULL) {
        free(ret.pointer);
    }
}

/**
 * @brief Current iter index vector - freer.
 * 
 * Frees all resource held by the vector.
 * 
 * @param ptr_parser Pointer to parser.
 */
static void free_cii_vector(struct json_parser* ptr_parser)
{
    size_t cii_len = cii_vector_size(ptr_parser);
    int* ptr = NULL;
    for (int i = 0; i < cii_len; i++) {
        ptr = abel_vector_at(&ptr_parser->current_iter_index, i).pointer;
        free(ptr);
    }
    abel_free_vector(&ptr_parser->current_iter_index);
}

/**
 * Static functions for parent key vector
 * 
 * @brief Parent-key vector stores pointers to struct abel_string
 *        intances created on the heap. In the following,
 *        parent-key vector is abbreviated as `pk_vector`.
 * 
 * pk_vector_init : Parent key vector is initialised to
 *     [struct abel_string*("ROOT_KEY_")].
 * 
 * pk_vector_size : Returns the size of the parent-key
 *     vector.
 * 
 * pk_vector_push_back : Push a parent key into the vector
 *     and effectively opens a new level.
 * 
 * pk_vector_at : Returns a struct abel_string* at the given level.
 * 
 * pk_vector_assign : Assign a parent key string to the
 *     given level. It also frees the previous key string.
 * 
 * free_pk_vector : Freer.
 **/

/**
 * @brief Parent key vector - init.
 * 
 * Initialise pk vector to [struct abel_string*("ROOT_KEY_")].
 * 
 * @param ptr_parser Pointer to parser.
 */
static void pk_vector_init(struct json_parser* ptr_parser)
{
    char root_key[] = "ROOT_KEY_";
    struct abel_string* ptr_pk = abel_make_string_ptr(root_key);
    abel_vector_append(&ptr_parser->parent_key, ptr_pk);
}

/**
 * @brief Parent key vector - size.
 * 
 * @param ptr_parser Pointer to parser.
 * @return Total number of elements in vector.
 */
static size_t pk_vector_size(struct json_parser* ptr_parser)
{
    return abel_vector_size(&ptr_parser->parent_key);
}

/**
 * @brief Parent key vector - push back.
 * 
 * Pushes a parent key in the vector. Note that this vector
 * works with struct abel_string instances, as most fields in the parser
 * are struct abel_string objects.
 * 
 * @param ptr_parser Pointer to parser.
 * @param ref_str A pointer to struct abel_string instance.
 */
static void pk_vector_push_back(struct json_parser* ptr_parser,
                                struct abel_string* ref_str)
{
    struct abel_string* ptr_str = abel_make_string_ptr(ref_str->ptr_array);
    abel_vector_append(&ptr_parser->parent_key, ptr_str);
}

/**
 * @brief Parent key vector - at.
 * 
 * @param ptr_parser Pointer to parser.
 * @param level Level at which the parent is requested.
 * @return A parent key (string pointer) at given level.
 */
static struct abel_string* pk_vector_at(struct json_parser* ptr_parser,
                                        size_t level)
{
    struct abel_string* ptr
            = abel_vector_at(&ptr_parser->parent_key, level).pointer;
    return ptr;
}

/**
 * @brief Parent key vector - assign.
 * 
 * Assign a parent key to the given level. It also frees
 * the previous struct abel_string at the same level.
 * 
 * @param ptr_parser Pointer to parser.
 * @param level Level to which the parent is assigned.
 * @param ref_str A pointer to struct abel_string instance.
 */
static void pk_vector_assign(struct json_parser* ptr_parser, size_t level,
                             struct abel_string* ref_str)
{
    struct abel_string* ptr_pk = abel_make_string_ptr(ref_str->ptr_array);
    struct abel_return_option ret
            = abel_vector_emplace(&ptr_parser->parent_key, level, ptr_pk);
    /* free the previous key string ptr */
    if (ret.is_okay && ret.pointer != NULL) { // free the previous key string
        abel_free_string_ptr(ret.pointer);
    }
}

/**
 * @brief Parent key vector - freer.
 * 
 * Frees resouces held by parent key vector.
 * 
 * @param ptr_parser Pointer to parser.
 */
static void free_pk_vector(struct json_parser* ptr_parser)
{
    size_t pk_len = pk_vector_size(ptr_parser);
    struct abel_string* ptr_str = NULL;
    for (int i = 0; i < pk_len; i++) {
        ptr_str = pk_vector_at(ptr_parser, i);
        abel_free_string_ptr(ptr_str);
    }
    abel_free_vector(&ptr_parser->parent_key);
}

/**
 * Static functions for keys-per-level vector
 * 
 * In the following I use `kpl_vector_` prefix to mark all
 * functions for this vector.
 * 
 * @brief Keys per level is a 2D vector. In the first dimension,
 *        each element is a pointer to a (sub)vector. The element
 *        in each subvector is a pointer to a JSON key token stored
 *        in the token vector. This design synchronises the keys
 *        in token vector and this one. When freeing the kpl vector
 *        there is no need to separately free the key tokens (pointers)
 *        stored here, as they are freed whilst freeing the token
 *        vector.
 * 
 * kpl_vector_init : struct abel_vector initialised to [[]]
 * 
 * kpl_vector_size : Total number of elements in the first
 *     dimension, i.e. total number of subvectors.
 * 
 * kpl_vector_at : Returns the subvector at the given level.
 * 
 * kpl_vector_new_level : Create a new level, i.e. pushing back
 *     a new subvector, in the vector.
 * 
 * kpl_vector_last : Returns the pointer to the last subvector.
 * 
 * kpl_vector_push_back_at : Pushes back a key-token pointer
 *     into the subvector at the given level.
 * 
 * kpl_vector_push_back_last : Pushes back a key-token pointer
 *     into the last subvector.
 */

/**
 * @brief Keys-per-level vector - init.
 * 
 * Initialises the kpl vector by pushing back an empty
 * (sub)vector as its first element. Note that kpl vector
 * is a 2D vector.
 * 
 * @param ptr_parser Pointer to parser.
 */
static void kpl_vector_init(struct json_parser* ptr_parser)
{
    struct abel_vector* ptr = abel_make_vector_ptr(0);
    abel_vector_append(&ptr_parser->keys_per_level, ptr);
}

/**
 * @brief Keys-per-level vector - size.
 * 
 * The total number of subvectors in the kpl vector, i.e.
 * the size of the first dimension.
 * 
 * @param ptr_parser Pointer to parser.
 * @return Total number of subvectors.
 */
static size_t kpl_vector_size(struct json_parser* ptr_parser)
{
    return abel_vector_size(&ptr_parser->keys_per_level);
}

/**
 * @brief Keys-per-level - at.
 * 
 * Returns the subvector at given level.
 * 
 * @param ptr_parser Pointer to parser.
 * @param level Level for which the subvector is requested.
 */
static struct abel_vector* kpl_vector_at(struct json_parser* ptr_parser,
                                         size_t level)
{
    struct abel_vector* ptr
            = abel_vector_at(&ptr_parser->keys_per_level, level).pointer;
    return ptr;
}

/**
 * @brief Keys-per-level vector - create a new level
 * 
 * It pushes back a vector pointer into the current
 * kpl vector and is thus creating a new level.
 * 
 * @param ptr_parser Pointer to parser.
 * @return The pointer to the newly added vector.
 */
static struct abel_vector* kpl_vector_new_level(struct json_parser* ptr_parser)
{
    struct abel_vector* ptr = abel_make_vector_ptr(0);
    abel_vector_append(&ptr_parser->keys_per_level, ptr);
    return ptr;
}

/**
 * @brief Keys-per-level vector - last subvector
 * 
 * Returns the pointer to the last subvector.
 * 
 * @param ptr_parser Pointer to parser.
 * @return Pointer to the last subvector.
 */
static struct abel_vector* kpl_vector_last(struct json_parser* ptr_parser)
{
    struct abel_return_option ret
            = abel_vector_back(&ptr_parser->keys_per_level);
    struct abel_vector* ptr_vector = NULL;
    if (ret.is_okay == true) {
        ptr_vector = ret.pointer;
    } else {
        ptr_vector = NULL;
    }
    return ptr_vector;
}

/**
 * @brief Keys-per-level - pushes a token into given subvector
 * 
 * Pushes back a key-token pointer into the subvector at the
 * given level.
 * 
 * @param ptr_parser Pointer to parser.
 * @param level Level at which the token is pushed into.
 * @param ref_token Pointer to the token which is to be stored
 *                  in this vector.
 */
static struct abel_return_option kpl_vector_push_back_at(
    struct json_parser* ptr_parser,
    const size_t level,
    const json_token_ptr ref_token)
{
    struct abel_return_option ret;
    struct abel_vector* ptr_subvector = kpl_vector_at(ptr_parser, level);
    ret = abel_vector_append(ptr_subvector, ref_token);
    return ret;
}

/**
 * @brief Keys-per-level - pushes a token into last subvector
 * 
 * Pushes back a key-token pointer into the last subvector.
 * 
 * @param ptr_parser Pointer to parser.
 * @param ref_token Pointer to the token which is to be stored
 *                  in the last subvector.
 */
static struct abel_return_option kpl_vector_push_back_last(
        struct json_parser* ptr_parser, json_token_ptr ref_token)
{
    struct abel_return_option ret;
    struct abel_vector* ptr_subvector = kpl_vector_last(ptr_parser);
    ret = abel_vector_append(ptr_subvector, ref_token);
    return ret;
}

/**
 * @brief Keys-per-level - freer.
 * 
 * Frees the resource held by kpl vector.
 * 
 * @param ptr_parser Pointer to parser.
 * @note Do not free the token pointers stored in subvectors,
 *       as they are already freed whilst freeing the token
 *       vector.
 */
static void free_kpl_vector(struct json_parser* ptr_parser)
{
    size_t vector_size = abel_vector_size(&ptr_parser->keys_per_level);
    struct abel_vector* ptr_subvector = NULL;
    for (size_t i = 0; i < vector_size; i++) {
        ptr_subvector = abel_vector_get(&ptr_parser->keys_per_level, i).pointer;
        if (ptr_subvector != NULL) {
            abel_free_vector_ptr(ptr_subvector);
        }
    }
    abel_free_vector(&ptr_parser->keys_per_level);
}

/**
 * Static - Operational functions
 * 
 * Operational functions are called by workflow functions
 * to perform a specific operation such as make and push
 * token into token vector.
 * 
 * is_first_noncomment_character
 * illegal_first_noncomment_character
 * report_duplicate_key
 * push_key_token
 * per_iterable_container
 * per_terminal_token
 * get_terminal_type
 * push_terminal_token
 * per_container_opening_token
 * push_container_opening_token
 * push_container_closing_token
 **/

/**
 * @brief Check if current literal is first non-comment character
 */
static Bool is_first_noncomment_character(struct json_parser* ptr_parser)
{
    return (abel_string_eq_cstring(&ptr_parser->current_literal, "") == true
            && abel_vector_size(&ptr_parser->token_vector) == 0);
}

/**
 * @brief Check if first noncomment char is illegal
 * 
 * @return An struct abel_return_option instance.
 *         It reports the error of having illegal
 *         first non-comment character.
 */
static struct abel_return_option illegal_first_noncomment_character(
        struct json_parser* ptr_parser, char* first_char)
{
    struct abel_return_option ret;
    struct abel_error err;
    char errmsg[64] = "Symbol '";
    strcat(errmsg, first_char);
    strcat(errmsg, "' cannot be the first character in JSON format.");
    err = error_parser_error(errmsg, ptr_parser->current_line);
    ret = abel_option_error(err);
    return ret;
}

/**
 * @brief Report duplicate key
 * 
 * JSON doesn't allow indetical keys in the same level of
 * a dictionary. This function checks keys-per-level vector
 * to identify duplicate.
 */
static struct abel_return_option report_duplicate_key(
        struct json_parser* ptr_parser, struct json_token* token)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    struct abel_error err;
    Bool is_duplicate = false;
    char errmsg[128] = "\0";
    if (kpl_vector_size(ptr_parser) <= (size_t)ptr_parser->current_level) {
        /* Keys at this level don't exist, add a new level and collect it. */ 
        kpl_vector_new_level(ptr_parser);
        kpl_vector_push_back_last(ptr_parser, token);
    } else {
        struct abel_vector* ptr_subvector = kpl_vector_at(ptr_parser,
                                                 ptr_parser->current_level);
        size_t veclen = abel_vector_size(ptr_subvector);
        for (size_t i = 0; i < veclen; i++) {
            json_token_ptr ptr_token_in = abel_vector_at(ptr_subvector, i).pointer;
            if (abel_string_eq(ptr_token_in->parent_key, token->parent_key) == true
                    && abel_string_eq(ptr_token_in->literal, token->literal) == true)
            {
                is_duplicate = true;
                strcat(errmsg, "Key '");
                strcat(errmsg, (token->literal)->ptr_array);
                strcat(errmsg, "' is a duplicate.");
            }
        }
        if (is_duplicate == false) {
            /* If no duplicate, collect the key. */ 
            kpl_vector_push_back_at(ptr_parser, ptr_parser->current_level, token);
        } else {
            err = error_parser_error(errmsg, ptr_parser->current_line);
            ret = abel_option_error(err);
        }
    }
    return ret;
}

/**
 * @brief Push a key token
 * 
 * Should all conditions be satisfied, the current literal
 * is tokenized as a key.
 * 
 * @param ptr_parser Pointer to parser.
 * @return An struct abel_return_option instance.
 *         - If no error, flag is_okay is true and pointer
 *           is NULL.
 *         - If error, flag is_okay is false and error stores
 *           the specific error instance.
 */
static struct abel_return_option push_key_token(struct json_parser* ptr_parser)
{
    struct abel_return_option ret;
    struct abel_error err;
    char errmsg[128] = "\0";
    /* Preliminary checks */
    if (get_current_container_type(ptr_parser) != DICT) {
        /* current container must be dictionary */
        strcat(errmsg, "Key is not in a dictionary.");
    } else {
        /* previous token cannot be key */
        if (token_vector_size(ptr_parser) > 0
                && token_vector_last(ptr_parser)->type == KEY) {
            strcat(errmsg, "Key cannot follow a key immediately.");
        }
    }
    /* If no pre error, make and push key token */
    if (strcmp(errmsg, "") == 0) {
        /* Important: JSON keys must be all in delimited scheme */
        if (ptr_parser->current_literal_scheme == DELIMITED) {
            struct json_token key_token = tokenize_key(
                &ptr_parser->current_literal,
                pk_vector_at(ptr_parser, ptr_parser->current_level),
                ptr_parser->current_level,
                ptr_parser->current_line,
                get_current_container_type(ptr_parser),
                ptr_parser->current_literal_scheme);
            /* TODO This may report error which must be propagated. */    
            json_token_ptr ptr_token
                    = token_vector_push_back(ptr_parser, &key_token).pointer;
            /* Must check duplicate key.*/
            ret = report_duplicate_key(ptr_parser, ptr_token);
            /* If no duplicate key, set parent key for the next level. */
            if (ret.is_okay == true) {
                if (pk_vector_size(ptr_parser) >= ptr_parser->current_level + 2)
                {
                    pk_vector_assign(ptr_parser, ptr_parser->current_level + 1,
                                     &ptr_parser->current_literal);
                } else {
                    /* This key becomes the parent key for the next level. */
                    pk_vector_push_back(ptr_parser, &ptr_parser->current_literal);
                }
            } else {
                /* Collect error due to duplicate key check. */
                strcat(errmsg, ret.error.msg);
            }
        } else {
            strcat(errmsg, "JSON does'nt accept unquoted key '");
            strcat(errmsg, (ptr_parser->current_literal).ptr_array);
            strcat(errmsg, "'.");
        }
    }

    /* return */
    //if (strcmp(errmsg, "") != 0) {
    //    /* if error, report */
    //    err = error_parser_error(msg, ptr_parser->current_line);
    //    ret = abel_option_error(err);
    //} else {
    //    /* Set parent key for the next level. */
    //    if (pk_vector_size(ptr_parser) >= ptr_parser->current_level + 2) {
    //        pk_vector_assign(ptr_parser, ptr_parser->current_level + 1,
    //                         &ptr_parser->current_literal);
    //    } else {
    //        /* This key becomes the parent key for the next level. */
    //        pk_vector_push_back(ptr_parser, &ptr_parser->current_literal);
    //    }
    //    ret = abel_option_okay(NULL);
    //}
    //if (strcmp(errmsg, "") == 0) {
    if (strlen(errmsg) == 0) {
        ret = abel_option_okay(NULL);
    } else {
        /* if error, report */
        err = error_parser_error(errmsg, ptr_parser->current_line);
        ret = abel_option_error(err);
    }
    return ret;
}

/**
 * @brief Actions required in list container
 * 
 * In an iterable container, the iter index must be tracked
 * and updated timely. Iterkey must be prepended to each element
 * in the iterable container.
 * 
 * @return An struct abel_return_option instance.
 *         struct abel_error may occur when pushing
 *         the token into token vector.
 */
static struct abel_return_option per_iterable_container(
        struct json_parser* ptr_parser)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    if (is_current_container_iterable(ptr_parser) == true) {
        if (cii_vector_size(ptr_parser) == 0) {
            cii_vector_append(ptr_parser, 0);
        }
        struct abel_string name_string = abel_string_from_int(
            cii_vector_at(ptr_parser, ptr_parser->current_level)
        );
        struct json_token iter_key_token = tokenize_iter_key(
            &name_string,
            pk_vector_at(ptr_parser, ptr_parser->current_level),
            ptr_parser->current_level,
            ptr_parser->current_line,
            get_current_container_type(ptr_parser),
            cii_vector_at(ptr_parser, ptr_parser->current_level)
        );
        ret = token_vector_push_back(ptr_parser, &iter_key_token);
        /* update parent key */
        if (pk_vector_size(ptr_parser) >= ptr_parser->current_level + 2) {
            pk_vector_assign(ptr_parser, ptr_parser->current_level + 1,
                             &name_string);
        } else {
            pk_vector_push_back(ptr_parser, &name_string);
        }
        /* Must free the temporary name string */
        abel_free_string(&name_string);
    }
    return ret;
}

/**
 * @brief Actions required per pushing terminal token
 * 
 * A terminal token must be preceeded by a key token or an
 * iter-key token.
 * 
 * @param ptr_parser Pointe to the parser.
 * @return An struct abel_return_option instance.
 * @note Called by `push_terminal_token`
 */
static struct abel_return_option per_terminal_token(
        struct json_parser* ptr_parser)
{
    struct abel_return_option ret;
    struct abel_error err;
    char errmsg[128] = "\0";
    json_token_ptr ptr_last_token = token_vector_last(ptr_parser);
    enum json_token_type last_token_type = ptr_last_token->type;
    if ( !(last_token_type == KEY || last_token_type == ITER_KEY) ) {
        strcat(errmsg, "Terminal isn't preceeded by key or iter key.");
        err = error_parser_error(errmsg, ptr_parser->current_line);
        ret = abel_option_error(err);
    } else {
        /* update previous key's referenced type */
        abel_string_assign(ptr_last_token->referenced_type, "Terminal");   
        ret = abel_option_okay(NULL);
    }
    return ret;
}

/**
 * @brief Terminal type return option.
 * 
 * Term(inal) type return option is similar to struct abel_return_option
 * defined in option header. It is however used only
 * by the get_terminal_type function as it may return
 * either a terminal type or an error.
 */
typedef struct terminal_type_return_option {
    Bool is_okay;
    Bool is_error;
    union {
        enum json_terminal_type term_type;
        struct abel_error error;
    };
} TermTypeOption;

/**
 * @brief Determines JSON terminal data type.
 * 
 * Data type is determined by the literal and its collection
 * scheme. A copy of the literal is used to determine the type
 * as some converter function, such as double, may first need
 * to remove spaces.
 * 
 * @param ptr_parser Pointer to parser.
 * @param literal Literal the datatype of which is to be determined.
 * @param scheme Literal-collection scheme associated with the
 *               literal.
 * @return A TermTypeOption instance.
 * @note Called by `push_terminal_token`.
 * @todo Consider to replace C string by Abel string.
 */
static TermTypeOption get_terminal_type(struct json_parser* ptr_parser,
        struct abel_string* literal, enum literal_scheme scheme)
{
    /* return option */
    TermTypeOption option;
    char errmsg[128] = "\0";
    /* return */
    enum json_terminal_type terminal_type = STRING_TERM;
    /* make a copy of the literal */
    struct abel_string literal_copied = abel_make_string(literal->ptr_array);
    if (scheme == LIBERAL) {
        if (is_null(literal_copied.ptr_array) ==true) {
            terminal_type = NULL_TERM;
        } else if (is_bool(literal_copied.ptr_array) == true) {
            terminal_type = BOOL_TERM;
        } else if (is_double(literal_copied.ptr_array) == true) {
            terminal_type = DOUBLE_TERM;
        } else {
            strcat(errmsg, "Type of unquoted string '");
            strcat(errmsg, literal->ptr_array);
            strcat(errmsg, "' cannot be recognised.");
        }
    } else if (scheme == NONE_SCHEME) {
        strcat(errmsg, "Collection scheme of '");
        strcat(errmsg, literal->ptr_array);
        strcat(errmsg, "' is not set.");
    } else {
        /* delimited string is always considered as string */
        terminal_type = STRING_TERM;
    }
    abel_free_string(&literal_copied);
    if (strlen(errmsg) == 0) {
        option.is_okay = true;
        option.is_error = false;
        option.term_type = terminal_type;
    } else {
        option.is_okay = false;
        option.is_error = true;
        option.error = error_new(errmsg, PARSER_ERROR, ptr_parser->current_line);
    }
    return option;
}

/**
 * @brief Push a terminal token
 * 
 * Pushes a terminal token into the token vector.
 * struct abel_error may occur and it will be returned.
 * 
 * @param ptr_parser Pointer to parser
 * @note Called by at_comma, at_container_closing
 */
static struct abel_return_option push_terminal_token(
        struct json_parser* ptr_parser)
{
    struct abel_return_option ret;
    ret = per_iterable_container(ptr_parser);
    if (ret.is_okay == true) {
        TermTypeOption term_type_option = get_terminal_type(ptr_parser,
                &ptr_parser->current_literal, ptr_parser->current_literal_scheme);
        if (term_type_option.is_okay == true) {
            struct json_token terminal_token = tokenize_terminal(
                &ptr_parser->current_literal,
                pk_vector_at(ptr_parser, ptr_parser->current_level + 1),
                ptr_parser->current_level,
                ptr_parser->current_line,
                get_current_container_type(ptr_parser),
                //terminal_type,
                term_type_option.term_type,
                ptr_parser->current_literal_scheme);
            ret = per_terminal_token(ptr_parser);
            token_vector_push_back(ptr_parser, &terminal_token);
        } else {
            ret = abel_option_error(term_type_option.error);
        }
    }
    return ret;
}

/**
 * @brief Actions for pushing container opening token
 * 
 * In a dict container, any opening token must be preceeded
 * by a key token. However, if the opening token is the first
 * of the entire text, this is not required as an implicit
 * key is assgined to it.
 * 
 * @todo TODO Merge the following logics, it is a bit messy!
 */
static struct abel_return_option per_container_opening_token(
        struct json_parser* ptr_parser, char* opening_symbol)
{
    struct abel_return_option ret;
    struct abel_error err;
    char* msg = "";
    if (token_vector_size(ptr_parser) > 0) {
        json_token_ptr ptr_last_token = token_vector_last(ptr_parser);
        if (get_current_container_type(ptr_parser) == DICT) {
            if (ptr_last_token->type != KEY) {
                msg = "In dictionary, any object must be preceeded by a key.";
                err = error_parser_error(msg, ptr_parser->current_line);
            } else {
            /* Update reference type according to the container to be created.*/
                if (strcmp(opening_symbol, L_BRACE) == 0) {
                    abel_string_assign(ptr_last_token->referenced_type, "Dict");
                } else if (strcmp(opening_symbol, L_BRACKET) == 0) {
                    abel_string_assign(ptr_last_token->referenced_type, "List");
                }
            }
        }
        if (is_current_container_iterable(ptr_parser) == true) {
            if (ptr_last_token->type != ITER_KEY) {
                msg = "In iterable container, any object must be preceeded "
                      "by an iter key.";
                err = error_parser_error(msg, ptr_parser->current_line);
            } else {
                // TODO Make a function to update last token.
                if (strcmp(opening_symbol, L_BRACE) == 0) {
                    abel_string_assign(ptr_last_token->referenced_type, "Dict");
                } else if (strcmp(opening_symbol, L_BRACKET) == 0) {
                    abel_string_assign(ptr_last_token->referenced_type, "List");
                }
            }
        }
    } else {
        /*TODO What??? */
    }
    if (strcmp(msg, "") == 0) {
        ret = abel_option_okay(NULL);
    } else {
        ret = abel_option_error(err);
    }
    return ret;
}

/**
 * @brief Make and push containing opening
 * 
 * @return An struct abel_return_option instance.
 *         struct abel_error that occurs in the operational
 *         functions will be returned.
 */
static struct abel_return_option push_container_opening_token(
        struct json_parser* ptr_parser, char* opening_symbol)
{
    struct abel_return_option ret = per_iterable_container(ptr_parser);
    if (ret.is_okay == true) {
        ret = per_container_opening_token(ptr_parser, opening_symbol);
        if (ret.is_okay == true) {
            struct abel_string opening_symbol_str = abel_make_string(opening_symbol);
            struct json_token opening_token = tokenize(
                //opening_symbol,
                &opening_symbol_str,
                get_token_type_by_symbol(opening_symbol),
                //parent_key[current_level + 1], // Note the level
                pk_vector_at(ptr_parser, ptr_parser->current_level + 1),
                ptr_parser->current_level,
                ptr_parser->current_line,
                get_current_container_type(ptr_parser) );
            /* TODO this may cause error too! */
            token_vector_push_back(ptr_parser, &opening_token);
            abel_free_string(&opening_symbol_str);
        }
    }
    return ret;
}

/**
 * @brief Make and push container closing
 *
 * @return An struct abel_return_option instance.
 *         struct abel_error that occurs in the operational
 *         functions will be returned.
 */
static struct abel_return_option push_container_closing_token(
        struct json_parser* ptr_parser, char* closing_symbol)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    /* Before pushing closing token, must adjust the iter-key at this level. */
    if (get_current_container_type(ptr_parser) == LIST) {
        cii_vector_emplace(ptr_parser, ptr_parser->current_level, 0);
    }
    struct abel_string closing_symbol_str = abel_make_string(closing_symbol);
    struct json_token closing_token = tokenize(
        &closing_symbol_str,
        get_token_type_by_symbol(closing_symbol),
        pk_vector_at(ptr_parser, ptr_parser->current_level),
        ptr_parser->current_level - 1,
        ptr_parser->current_line,
        // Caution. Must use the container of the outer (higher) level, since
        // level switching is performed after pushing the closing token.
        cct_vector_at(ptr_parser, ptr_parser->current_level - 1));
    ret = token_vector_push_back(ptr_parser, &closing_token);
    abel_free_string(&closing_symbol_str);
    return ret;
}

/**
 * Static workflow control functions
 * 
 * Parser parses the file character by character, at each
 * character, workflow controller calls the operational
 * function to perform required check and actions.
 * 
 * at_back_slash
 * at_double_quotation
 * at_space
 * at_colon
 * at_comma
 * at_container_opening
 * at_container_closing
 * at_other_symbol
 * 
 * free_parser
 * parse_line
 **/

/**
 * @brief Static - At back slash
 * 
 * Back slash can be used to initiate an escaping sequence.
 * If current parser is in an escaping sequence, the back
 * slash is considered as a character. Otherwise, it signals
 * the beginning of an escaping sequence.
 * 
 * @return An struct abel_return_option instance
 * @todo Which error shall I return? Function literal_append
 *       may cause error.
 */
static struct abel_return_option at_back_slash(struct json_parser* ptr_parser)
{
    struct abel_return_option retopt = abel_option_okay(NULL);
    if (ptr_parser->is_delimited_string_open == true) {
        if (ptr_parser->is_escaping == true) {
            literal_append(ptr_parser, (char*)BACK_SLASH);
            /* switch off escaping flag after appending the char. */
            ptr_parser->is_escaping = false;
        } else { 
            ptr_parser->is_escaping = true;
        }
    } else {
        literal_append(ptr_parser, (char*)BACK_SLASH);
    }
    ptr_parser->current_column += 1;
    return retopt;
}

/**
 * @brief Static - At double quotation mark (")
 * 
 * Double quotation marks the beginning or the end of
 * a delimited string. It can of course be part of a
 * string when in an escape sequence.
 * 
 * @return An struct abel_return_option intance.
 * @todo Where does error come from?
 */
static struct abel_return_option at_double_quotation(struct json_parser* ptr_parser)
{
    struct abel_return_option retopt = abel_option_okay(NULL);
    if (literal_is_empty(ptr_parser) == true) { /* current literal is empty */
        if (ptr_parser->is_delimited_string_open == false) {
            /* not in a delimited string */
            ptr_parser->is_delimited_string_open = true;
            ptr_parser->current_literal_scheme = DELIMITED;
        } else {
            /* already in a delimited string */ 
            ptr_parser->is_delimited_string_open = false;
            abel_string_assign(&ptr_parser->latest_syntactic_operator,
                               (char*)DOUBLE_QUOTE);
        }
    } else { /* current literal is non-empty */
        if (ptr_parser->is_delimited_string_open == true) { // a delimited string has opened...
            if (ptr_parser->is_escaping == true) { // previous char is escaping
                literal_append(ptr_parser, (char*)DOUBLE_QUOTE);
                ptr_parser->is_escaping = false; // turn off flag
            } else { // then double quote is a string-closing operator
                ptr_parser->is_delimited_string_open = false;
                abel_string_assign(&ptr_parser->latest_syntactic_operator,
                                   (char*)DOUBLE_QUOTE);
            }
        } else {    // delimited string is not open
            if (ptr_parser->current_literal_scheme == LIBERAL) {
                literal_append(ptr_parser, (char*)DOUBLE_QUOTE);
            } else {
                // If not LIBERAL scheme, the previous quoted string has been
                // closed; this quotation mark starts a new delimited string.
                // This algorithm allows a delimited string to be broken into
                // multiple lines.
                ptr_parser->is_delimited_string_open = true;
            }
        }
    }
    ptr_parser->current_column += 1;
    return retopt;
}

/**
 * @brief Static - At space
 * 
 * Single space is either skipped, if it is outside of
 * a delimited string, or collected as a char if it is.
 * 
 * @return An struct abel_return_option instance.
 * @todo What error to report?
 */
static struct abel_return_option at_space(struct json_parser* ptr_parser)
{
    struct abel_return_option retopt = abel_option_okay(NULL);
    if (ptr_parser->is_delimited_string_open == true) {
        literal_append(ptr_parser, (char*)SPACE);
    }
    ptr_parser->current_column += 1;
    return retopt;
}

/**
 * @brief Static workflow controller - At colon (:)
 * 
 * @todo the logic of error handling is very messy.
 */
static struct abel_return_option at_colon(struct json_parser* ptr_parser)
{
    struct abel_return_option retopt;
    struct abel_error err;
    char errmsg[128] = "\0";
    if (ptr_parser->is_delimited_string_open == true) {    /* part of literal */
        literal_append(ptr_parser, (char*)COLON);
        retopt = abel_option_okay(NULL);
    }     else {
        /* Pre-tokenization syntax checks */
        if ( is_first_noncomment_character(ptr_parser) ) {
            retopt = illegal_first_noncomment_character(ptr_parser, (char*)COLON);
        } else {
            if ( is_current_container_iterable(ptr_parser) ) {
                strcat(errmsg, "Colon operator is meaningless.");
            } else if (literal_is_empty(ptr_parser) == true) {
                strcat(errmsg, "Colon operator must appear after "
                               "a non-empty key token.");
            } else if ( abel_string_eq_cstring(&ptr_parser->latest_syntactic_operator,
                                        (char*)COLON) )
            {
                strcat(errmsg, "Colon operator cannot appear immediately "
                               "after a colon operator.");
            }
            /* make return option */
            if (strcmp(errmsg, "") != 0) {
                err = error_parser_error(errmsg, ptr_parser->current_line);
                retopt = abel_option_error(err);
            }
        }

        //if (strcmp(errmsg, "") != 0) {
        //    err = error_parser_error(errmsg, ptr_parser->current_line);
        //    retopt = abel_option_error(err);
        //}

        /* FIXME How about this one ?*/
        if (literal_is_empty(ptr_parser) == false) {
            if (cct_vector_at(ptr_parser, 0) == NONE_CONTAINER) {
                set_root_container_type(ptr_parser, DICT); 
            }
            retopt = push_key_token(ptr_parser);
            literal_reset(ptr_parser);
            abel_string_assign(&ptr_parser->latest_syntactic_operator, (char*)COLON);
        }
    }
    ptr_parser->current_column += 1;
    return retopt;
}

/**
 * @brief Static - At comma (,)
 * 
 * @todo struct abel_error handling logic is messy.
 */
static struct abel_return_option at_comma(struct json_parser* ptr_parser)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    struct abel_error err;
    char errmsg[128] = "\0";
    if (ptr_parser->is_delimited_string_open == true) {
        literal_append(ptr_parser, (char*)COMMA);
        ret = abel_option_okay(NULL);
    } else {
        if (is_first_noncomment_character(ptr_parser) == true) {
            ret = illegal_first_noncomment_character(ptr_parser, (char*)COMMA);
        }
        if (literal_is_empty(ptr_parser) == true) {
            if ( !(abel_string_eq_cstring(&ptr_parser->latest_syntactic_operator, (char*)DOUBLE_QUOTE)
                    || abel_string_eq_cstring(&ptr_parser->latest_syntactic_operator, (char*)R_BRACE)
                    || abel_string_eq_cstring(&ptr_parser->latest_syntactic_operator, (char*)R_BRACKET)) )
            {    
                strcat(errmsg, "Comma appears only after a terminal, "
                               "a string, or a container closing operator.");
                err = error_parser_error(errmsg, ptr_parser->current_line);
                ret = abel_option_error(err);
            }
        }

        if (literal_is_empty(ptr_parser) == false 
            || (literal_is_empty(ptr_parser) == false 
                && abel_string_eq_cstring(&ptr_parser->latest_syntactic_operator, "\"")) )
        {
            if (cct_vector_at(ptr_parser, 0) == NONE_CONTAINER) {
                set_root_container_type(ptr_parser, LIST);
            }
            ret = push_terminal_token(ptr_parser);
            literal_reset(ptr_parser);
            /* increase iter index at current level by 1 */
            size_t iter_index
                    = cii_vector_at(ptr_parser, ptr_parser->current_level);
            cii_vector_emplace(ptr_parser, ptr_parser->current_level, iter_index + 1);
        }
        /* Must perform Post-pushing check */
        enum json_token_type last_token_type
                = token_vector_last(ptr_parser)->type;
        if ( !(last_token_type == TERMINAL || last_token_type == DICT_CLOSING
                || last_token_type == LIST_CLOSING) ) {
            strcat(errmsg, "Comma is meaningless.");
            err = error_parser_error(errmsg, ptr_parser->current_line);
            ret = abel_option_error(err);
        }
        abel_string_append(&ptr_parser->latest_syntactic_operator, (char*)COMMA);
    }
    ptr_parser->current_column += 1;
    return ret;
}

/**
 * @brief Static - At container opening symbol.
 * 
 * Container opening always enters a deeper-than-the-current
 * level.
 */
static struct abel_return_option at_container_opening(
        struct json_parser* ptr_parser, char* opening_symbol)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    enum json_container_type container_type
        = get_container_type_by_symbol(opening_symbol);

    if (ptr_parser->is_delimited_string_open == true) {
        literal_append(ptr_parser, opening_symbol);
    } else {
        if (is_first_noncomment_character(ptr_parser) == true) {
            if (get_current_container_type(ptr_parser) == NONE_CONTAINER) { 
                set_root_container_type(ptr_parser, LIST);
            }
            /* only one parent key so far */
            if (pk_vector_size(ptr_parser) == 1) {
                pk_vector_push_back( ptr_parser,
                        pk_vector_at(ptr_parser, ptr_parser->current_level) );
            }
        }
        // TODO Implement bracket match
        //bracket_match.match(opening_symbol, current_line, current_column);

        /* Must update container type for the next level */
        if (cct_vector_size(ptr_parser) >= ptr_parser->current_level + 2) {
            cct_vector_emplace(ptr_parser, ptr_parser->current_level + 1,
                               container_type);
        } else {
            cct_vector_push_back(ptr_parser, container_type);
        }
        ret = push_container_opening_token(ptr_parser, opening_symbol);
        /* TODO If last step has error, the following shall be paused !!*/
        ptr_parser->current_level += 1;    // enter deeper level
        /* initialise iter index to 0, disregarding the container type */
        cii_vector_append(ptr_parser, 0);
        /* update deepest level */
        if (ptr_parser->current_level > ptr_parser->deepest_level) {
            ptr_parser->deepest_level = ptr_parser->current_level;
        }
        /* update latest syntactic operator */
        abel_string_assign(&ptr_parser->latest_syntactic_operator, opening_symbol);
    }
    ptr_parser->current_column += 1;
    return ret;
}

/**
 * @brief Static - At container closing symbol
 * 
 * Closing a container always goes to a higher level.
 * 
 */
static struct abel_return_option at_container_closing(
        struct json_parser* ptr_parser, char* closing_symbol)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    if (ptr_parser->is_delimited_string_open == true) {
        literal_append(ptr_parser, closing_symbol);
    } else {
        if ( is_first_noncomment_character(ptr_parser) ) {
            illegal_first_noncomment_character(ptr_parser, closing_symbol);
        }
        // TODO Must implement the following check.
        //bracket_match.match(closing_symbol, current_line, current_column);

        /* Current literal is not empty */
        if (literal_is_empty(ptr_parser) == false) {
            ret = push_terminal_token(ptr_parser);
            literal_reset(ptr_parser);
        }
        ret = push_container_closing_token(ptr_parser, closing_symbol);
        // FIXME The following iter-key adjustment doesn't seem to work.
        if (get_parent_container_type(ptr_parser) == LIST) {
            cii_vector_emplace(ptr_parser, ptr_parser->current_level, 0);
            size_t parent_level_iter_index
                    = cii_vector_at(ptr_parser, ptr_parser->current_level - 1);
            cii_vector_emplace(ptr_parser, ptr_parser->current_level - 1,
                               parent_level_iter_index + 1);
        }
        ptr_parser->current_level -= 1;
        abel_string_assign(&ptr_parser->latest_syntactic_operator, closing_symbol);

    }
    ptr_parser->current_column += 1;
    return ret;
}

/**
 * @brief Static - At other symbol
 * 
 * Most characters fall into this category.
 */
static struct abel_return_option at_other_symbol(struct json_parser* ptr_parser,
                                                 char* current_char)
{
    struct abel_return_option ret = abel_option_okay(NULL);
    struct abel_error err;
    char errmsg[128] = "\0";
    /* Current literal is empty, new literal collection starts */ 
    if (literal_is_empty(ptr_parser) == true) {
        /* If inside delimited string is on, this is just after
           double quotation mark, simply append the character */
        if (ptr_parser->is_delimited_string_open == true) {
            literal_append(ptr_parser, current_char);
        } else {
            ptr_parser->current_literal_scheme = LIBERAL;
            literal_append(ptr_parser, current_char);
        }
    } else {    /* Current literal is non-empty */
        if (ptr_parser->is_delimited_string_open == false
                && ptr_parser->current_literal_scheme == DELIMITED)
        {
            /* A delimited string is before this character, for example
               "first"name, i.e. appending a liberal string to a delimited
               one is not allowed. */
            strcat(errmsg, "Appending a liberal string to a "
                           "delimited one is not allowed.");
            err = error_parser_error(errmsg, ptr_parser->current_line);
            ret = abel_option_error(err);
        } else {
            /* TODO there is a bug! */
            literal_append(ptr_parser, current_char);
        }
    }
    ptr_parser->current_column += 1;
    return ret;
}

static void free_parser(struct json_parser* ptr_parser)
{
    free_token_vector(ptr_parser);
    free_literal(ptr_parser);
    free_latest_symbol(ptr_parser);
    free_cct_vector(ptr_parser);
    /* free a vector of strings */
    free_cii_vector(ptr_parser);
    free_pk_vector(ptr_parser);
    free_kpl_vector(ptr_parser);
    abel_free_string(&ptr_parser->latest_syntactic_operator);
}

/**
 * @brief Static - Line parser.
 * 
 * Line parser processes and parses a line char by char.
 * 
 * @todo Propagate and handle error.
 */
static struct abel_return_option parse_line(struct json_parser* ptr_parser, char* line)
{
    struct abel_return_option retopt = abel_option_okay(NULL);
    size_t raw_length = strlen(line);
    while (ptr_parser->current_column < raw_length) {
        /* convert char into C string */
        char current_char[] = {line[ptr_parser->current_column], '\0'};
        /* branching */
        if (abel_cstring_eq(current_char, (char*)BACK_SLASH) == true) {
            retopt = at_back_slash(ptr_parser);
        } else if (abel_cstring_eq(current_char, (char*)DOUBLE_QUOTE) == true) {
            retopt = at_double_quotation(ptr_parser);
        } else if (abel_cstring_eq(current_char, (char*)SPACE) == true) {
            retopt = at_space(ptr_parser);
        } else if (abel_cstring_eq(current_char, (char*)SHARP) == true) {/* # comment */
            if (ptr_parser->is_delimited_string_open) {
                literal_append(ptr_parser, current_char);
                ptr_parser->current_column += 1;
                continue;
            } else {
                break;
            }
        } else if (abel_cstring_eq(current_char, (char*)COLON) == true) {
            retopt = at_colon(ptr_parser);
        } else if (abel_cstring_eq(current_char, (char*)COMMA) == true) {
            retopt = at_comma(ptr_parser);
        } else if (is_opening_symbol(current_char) == true) {    /* opening */
            retopt = at_container_opening(ptr_parser, current_char);
        } else if (is_closing_symbol(current_char) == true) {    /* closing */
            retopt = at_container_closing(ptr_parser, current_char);
        } else {    /* none of the above */
            retopt = at_other_symbol(ptr_parser, current_char);
            //continue;
        }
        /* error handling */
        if (retopt.is_okay == true) {
            continue;
        } else {
            break;
        }
    }
    return retopt;
}

/**
 * @brief Exit parser gracefully
 * 
 * Exiting parser due to error must be come after all resources
 * are freed to prevent memory leak. This is graceful shut down.
 */
static void safe_exit(struct json_parser* ptr_parser)
{
    free_parser(ptr_parser);

}
/**
 * Public interface
 * 
 * abel_make_json_parser : Initialises a parser
 * 
 * abel_free_json_parser : Frees resources held by the
 *     JSON parser.
 * 
 * abel_parse_file : Parses a JSON file
 **/

/**
 * @brief Initialises a JSON parser
 * 
 * @param ptr_parser Pointer to a JSON parser.
 */
void abel_make_json_parser(struct json_parser* ptr_parser)
{
    ptr_parser->token_vector = abel_make_vector(0);
    ptr_parser->current_line = 0;
    ptr_parser->current_column = 0;
    ptr_parser->current_literal = abel_make_string("");
    ptr_parser->latest_symbol = abel_make_string("");
    ptr_parser->current_level = 0;
    ptr_parser->deepest_level = 0;
    /* current container type (cct) vector */
    ptr_parser->current_container_type = abel_make_vector(0);
    cct_vector_init(ptr_parser);
    /* current iter index (cii) vector */
    ptr_parser->current_iter_index = abel_make_vector(0);
    cii_vector_init(ptr_parser);
    /* parent key (pk) vector */
    ptr_parser->parent_key = abel_make_vector(0);
    pk_vector_init(ptr_parser);
    /* keys per level (kpl) vector is 2D */
    ptr_parser->keys_per_level = abel_make_vector(0);
    kpl_vector_init(ptr_parser);
    /* latest syntactic operator */
    ptr_parser->latest_syntactic_operator = abel_make_string("");
    /* escaping and delimiting flags */
    ptr_parser->is_escaping = false;
    ptr_parser->is_delimited_string_open = false;
    /* literal scheme */
    ptr_parser->current_literal_scheme = NONE_SCHEME;
    /* initiliase error register */
    //ptr_parser->error_register[0] = error_none();
}

/**
 * @brief File parser
 * 
 * File parser opens and parses a file. It reads the file
 * line by line and processes each line by calling the line
 * parser (static function `parse_line`).
 * 
 * @todo If file opening fails, 
 */
void abel_parse_file(struct json_parser* ptr_parser, char* file_name)
{
    FILE* file = fopen(file_name, "r");    // should check the result
    char line[255];
    struct abel_return_option retopt_line_parser = abel_option_okay(NULL);

    while ( fgets(line, sizeof(line), file) ) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        size_t raw_length = strlen(line);    // may contain the newline char
        if (line[raw_length - 1] == '\n') {
            line[raw_length - 1] = '\0';
        }
        ptr_parser->current_line += 1;
        ptr_parser->current_column = 0;
        if (strlen(line) == 0) continue;
        retopt_line_parser = parse_line(ptr_parser, line);
        /* TODO check error and stop parsing */
        if (retopt_line_parser.is_okay == true) {
            continue;
        } else {
            safe_exit(ptr_parser);
        }
    }
    fclose(file);
}

void abel_free_json_parser(struct json_parser* ptr_parser)
{
    free_parser(ptr_parser);
}