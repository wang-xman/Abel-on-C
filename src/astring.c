/* Source astring.h */

#include "astring.h"

static char* pointer_to_internal_array(size_t size_requested)
{
    char* ptr_array;
    ptr_array = malloc( sizeof(char)*(size_requested) );
    return ptr_array;
}

/**
 * @brief Static - Return size to be requested
 * 
 * The returned size-to-be-requested is larger than the
 * combined length of string and null-terminator, and it
 * is always a multiple of 4 bytes.
 * 
 * @param src_net_length Net length of a string with the null
 *     terminator
 */
static size_t requested_size(size_t src_net_length)
{
    size_t size_requested = 4;    // minimal size/maxcap to request
    if (src_net_length > 3) {
        size_requested = next_power_of_two(src_net_length + 1);
    }
    return size_requested;
}

struct abel_string abel_make_string(char* src_cstr)
{
    struct abel_string string;
    size_t src_net_length = strlen(src_cstr);    // length without null
    size_t size_requested = requested_size(src_net_length);
    /* resource is allocated on heap */
    string.ptr_array = pointer_to_internal_array(size_requested);
    /* source is duplicated on heap */
    strncpy(string.ptr_array, src_cstr, src_net_length + 1);
    string.length = src_net_length;
    string.capacity = size_requested;
    return string;
}

struct abel_string abel_string_from_int(int src_int)
{
    struct abel_string string;
    /* net length does not include plus sign */
    int net_length = snprintf(NULL, 0, "%d", src_int);
    char* ptr_str = malloc( (net_length + 1)*sizeof(*ptr_str) );
    snprintf(ptr_str, net_length + 1, "%d", src_int);
    /* content in ptr_str is copied ... */
    string = abel_make_string(ptr_str);
    /* ... and it must be freed. */
    free(ptr_str);
    return string;
}

void abel_free_string(struct abel_string* ptr_str)
{
    free(ptr_str->ptr_array);
}

struct abel_string* abel_make_string_ptr(char* src_cstr)
{
    /* memory for string instance */
    struct abel_string* ptr = malloc( sizeof(*ptr) );
    size_t src_net_length = strlen(src_cstr);    // length without null
    size_t size_requested = requested_size(src_net_length);
    /* resource is allocated on heap */
    ptr->ptr_array = pointer_to_internal_array(size_requested);
    /* source C string is copied on heap */
    strncpy(ptr->ptr_array, src_cstr, src_net_length + 1);
    ptr->length = src_net_length;
    ptr->capacity = size_requested;
    return ptr;
}

void abel_free_string_ptr(struct abel_string* ptr_str)
{
    free(ptr_str->ptr_array);
    free(ptr_str);
}

/* Checker */

size_t abel_string_len(struct abel_string* ptr_str) {
    return ptr_str->length;
}

size_t abel_string_capacity(struct abel_string* ptr_str) {
    return ptr_str->capacity;
}

/* Assign */

void abel_string_assign(struct abel_string* ptr_str, char* src_cstr)
{
    size_t src_net_length = strlen(src_cstr);    // length without null
    size_t size_requested = requested_size(src_net_length);
    /* free the current content */
    free(ptr_str->ptr_array);
    /* allocate again */
    ptr_str->ptr_array = pointer_to_internal_array(size_requested);
    strncpy(ptr_str->ptr_array, src_cstr, src_net_length + 1);
    ptr_str->length = src_net_length;
    ptr_str->capacity = size_requested;
}

void abel_string_assign_char(struct abel_string* ptr_str, char src_char)
{
    char converted[] = {src_char, '\0'};
    abel_string_assign(ptr_str, converted);
}

/* Append */

/**
 * @todo Shall it throw error?
 */
void abel_string_append(struct abel_string* ptr_str, char* src_cstr)
{
    char* ptr = NULL;
    if (strlen(src_cstr) > 0) {
        size_t new_net_length = ptr_str->length + strlen(src_cstr);
        size_t new_requested = requested_size(new_net_length);
        if (new_requested > ptr_str->capacity) {   // need to realloc
            ptr_str->ptr_array = realloc( ptr_str->ptr_array,
                                          new_requested * sizeof(char) );
            ptr_str->capacity = new_requested;
        }
        ptr = ptr_str->ptr_array + ptr_str->length;
        while ( (*ptr++ = *src_cstr++) );
        ptr_str->length = new_net_length;
    }
}

void abel_string_append_char(struct abel_string* ptr_str, char src_char)
{
    char converted[] = {src_char, '\0'};
    abel_string_append(ptr_str, converted);
}

/* Compare */

Bool abel_string_eq(const struct abel_string* ptr_str1,
                    const struct abel_string* ptr_str2)
{
    Bool ret = false;
    if (ptr_str1->length == ptr_str2->length) {
        if (strcmp(ptr_str1->ptr_array, ptr_str2->ptr_array) == 0) {
            ret = true;
        }
    }
    return ret;
}

Bool abel_string_eq_cstring(const struct abel_string* ptr_str,
                            const char* src_cstr)
{
    Bool ret = false;
    if (ptr_str->length == strlen(src_cstr)) {
        if (strcmp(ptr_str->ptr_array, src_cstr) == 0) {
            ret = true;
        }
    }
    return ret;
}

Bool abel_string_eq_char(struct abel_string* ptr_str, char src_char)
{
    Bool ret = false;
    char converted[] = {src_char, '\0'};
    ret = abel_string_eq_cstring(ptr_str, converted);
    return ret;
}

Bool abel_cstring_eq(const char* cstr1, const char* cstr2)
{
    Bool ret = false;
    if ( strlen(cstr1) == strlen(cstr2) && strcmp(cstr1, cstr2) == 0 ) {
        ret = true;
    }
    return ret;
}

Bool abel_cstring_eq_char(const char* src_str, const char src_char)
{
    Bool ret = false;
    char converted[] = {src_char, '\0'};
    ret = abel_cstring_eq(src_str, converted);
    return ret;
}

/* char getter */

char abel_string_at(struct abel_string* ptr_str, size_t idx)
{
    if (idx < ptr_str->length) {
        return ptr_str->ptr_array[idx];
    } else {
        return '\0';
    }
}


char abel_string_front(struct abel_string* ptr_str)
{
    if ( abel_string_len(ptr_str) == 0) {
        return '\0';
    } else {
        return abel_string_at(ptr_str, 0);
    }
}

char abel_string_back(struct abel_string* ptr_str)
{
    size_t length = abel_string_len(ptr_str);
    if (length == 0) {
        return '\0';
    } else {
        return abel_string_at(ptr_str, length - 1);
    }
}


Bool abel_string_has(struct abel_string* ptr_str, char src_char)
{
    Bool flag = false;
    for (int i = 0; i < ptr_str->length; i++) {
        if (ptr_str->ptr_array[i] == src_char) {
            flag = true;
        }
    }
    return flag;
}