/*
 * Source symbol.c
 * Created 08-11-2022
 * Updated 08-11-2022
 */
#ifndef ABEL_ON_C_SYMBOL_H
#define ABEL_ON_C_SYMBOL_H

#include "symbol.h"

/* Standalone symbols */

const char SPACE[] = " ";             /* Single space */
const char SHARP[] = "#";             /* Single-line comment */
const char COLON[] = ":";             /* Key-value separator */
const char COMMA[] = ",";             /* Same level object separator */
const char ENDL[] = "\n";             /* End of line (not in active use) */ 
const char BACK_SLASH[] = "\\";       /* Back slash, need escape */
const char FORWARD_SLASH[] = "/";     /* Forward slash */
const char SINGLE_QUOTE[] = "\'";     /* Single quote */
const char DOUBLE_QUOTE[] = "\"";     /* Double quote */
const char BACK_TICK[] = "\u0060";    /* Back tick ` */
const char GRAVE_ACCENT[] = "\u0060"; /* Grave accent ` */ 

/* Algebraic symbols */

const char MINUS[] = "-";            /* Minus sign */
const char PLUS[] = "+";             /* Plus sign */
const char DECIMAL_POINT[] = ".";    /* Decimal point */
const char HAT[] = "^";              /* Exponentiation */
const char DOUBLE_MULTIPLY[] = "**"; /* Exponentiation */
const char ADD[] = "+";              /* Add + */
const char SUBTRACT[] = "-";         /* Subtract - */
const char MULTIPLY[] = "*";         /* Multiply * */
const char DIVIDE[] = "/";           /* Divide / */ 

/* Paried symbols */
const char L_BRACE[] = "{";       /* Braces (curly brackets) */
const char R_BRACE[] = "}";
const char L_BRACKET[] = "[";     /* Brackets (square brackets) */
const char R_BRACKET[] = "]";
const char L_PARENTHESIS[] = "("; /* Parentheses (round brackets) */
const char R_PARENTHESIS[] = ")";
const char L_CHEVRON[] = "<";     /* Chevrons (angle brackets) */
const char R_CHEVRON[] = ">";

/* Container symbols */
const char* CONTAINER_TYPES[] = { "Dict", "List", "Tuple" };
const char* ITERABLE_CONTAINERS[] = { "List", "Tuple" };
const char* CONTAINER_OPENING_SYMBOLS[] = { "{", "[", "(" };
const char* CONTAINER_CLOSING_SYMBOLS[] = { "}", "]", ")" };
const char* CONTAINER_OPENING_TOKEN_TYPES[] = {
    "Dict_Opening", "List_Opening", "Tuple_Opening"
};
const char* CONTAINER_CLOSING_TOKEN_TYPES[] = {
    "Dict_Closing", "List_Closing", "Tuple_Closing"
};

#endif