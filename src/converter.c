/**
 * Source converter.c
 * Created 16-12-2022
 * Updated 27-12-2022
 * 
 * Functions that convert string to its designated
 * data type.
 */
#include "converter.h"

Bool is_null(char* src_str)
{
    Bool ret = false;
    if (strcmp(src_str, "null") == 0) ret = true;
    return ret;
}

// TODO Not tested
Null as_null(const char* src_str)
{
    Null ret = null;
    //if ( !is_null(src_str) ) {
    //    throw error::DataTypeConversionError(
    //            "Failed to identify and convert a Null type.");
    //}
    return ret;
}

Bool is_bool(char* src_str)
{
    Bool ret = false;
    if (strcmp(src_str, "true") == 0 || strcmp(src_str, "false") == 0) {
        ret = true;
    }
    return ret;
}

// TODO not tested
Bool as_bool(char* src_str)
{
    Bool ret;
    if ( is_bool(src_str) ) {
        if (strcmp(src_str, "true") == 0) {
            ret = true;
        } else {
            ret = false;
        }
    }
    //else {
    //    throw error::DataTypeConversionError(
    //            "Failed to identify and convert a Bool type.");
    //}
    return ret;
}

Bool is_double(char* src_str)
{
    Bool ret = false;
    size_t sscanf_ret = 0;
    char first_non_numeric;
    double converted;
    remove_space(src_str);
    sscanf_ret = sscanf(src_str, "%lf %c", &converted, &first_non_numeric);
    if (sscanf_ret == 1) ret = true;
    return ret;
}

// TODO Not tested
double as_double(char* src_str)
{
    double ret;
    if ( is_double(src_str) ) {
        //ret = stod(src_str);
        ret = atof(src_str);
    }
    //else {
    //    throw error::DataTypeConversionError(
    //            "Failed to identify and convert a Double type.");
    //}
    return ret;
}