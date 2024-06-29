/**
 * Header converter.h
 * Created 16-12-2022
 * Updated 16-12-2022
 * 
 * Functions that convert string to its designated
 * data type.
 */
#ifndef ABEL_ON_C_CONVERTER_H
#define ABEL_ON_C_CONVERTER_H

#include "util.h"

/* Null */
Bool is_null(char* src_str);
Null as_null(const char* src_str);

/* Bool*/
Bool is_bool(char* src_str);
Bool as_bool(char* src_str);

//// Integer
//bool is_integer(const std::string&);
//bool is_unsigned_integer(const std::string&);
//bool is_positive_integer(const std::string&);
//bool is_negative_integer(const std::string&);
//Integer as_integer(const std::string&);
//// Double
//DataTypeIdentifier identify_double(const std::string&);

Bool is_double(char* src_str);
//bool is_unsigned_double(const std::string&);
//bool is_positive_double(const std::string&);
//bool is_negative_double(const std::string&);
double as_double(char* src_str);

#endif