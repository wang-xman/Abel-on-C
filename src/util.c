/* Source util */
#include "util.h"

void remove_space(char src_array[])
{
    char* d = src_array;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ( (*src_array++ = *d++) );
}

Bool has_letter(char* src_str)
{
    Bool ret = false;
    do {
        if (isalpha(*src_str) != 0) {
            ret = true;
            break;
        }
        src_str++;
    } while (*src_str != '\0');
    return ret;
}

Bool begins_with_letter(char* src_str)
{
    if(strlen(src_str) == 0) {
        return false;
    } else {
        if (isalpha(*src_str) != 0) {
            return true;
        } else {
            return false;
        }
    }
}

Bool begins_with_sign(char* src_str)
{
    if (strlen(src_str) == 0) {
        return false;
    } else{
        if ( (src_str[0] == '-') || (src_str[0] == '+') ) {
            return true;
        } else {
            return false;
        }
    }
}

Bool is_algebraic_operator(char* src_str)
{
    if (strlen(src_str) == 2) {    // exponentiation "**"
        if (strcmp(src_str, DOUBLE_MULTIPLY) == 0) {
            return true;
        } else {
            return false;
        }
    } else if (strlen(src_str) == 1) {
        if ( src_str[0] == '+' || src_str[0] == '-' || src_str[0] == '*'
                || src_str[0] == '/' || src_str[0] == '^') {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}