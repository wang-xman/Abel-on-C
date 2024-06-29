/*
 * HEADER symbol.h
 * Created 08-11-2022
 * Updated 08-11-2022
 * 
 * Symbols are defined as string literals
 */
#include "typefy.h"

/* Standalone symbols */

extern const char SPACE[];
extern const char SHARP[];
extern const char COLON[];
extern const char COMMA[];
extern const char ENDL[];  // End of line (not in active use)
extern const char BACK_SLASH[];
extern const char FORWARD_SLASH[];
extern const char SINGLE_QUOTE[];
extern const char DOUBLE_QUOTE[];
extern const char BACK_TICK[];
extern const char GRAVE_ACCENT[];

/* Algebraic symbols */

extern const char MINUS[];
extern const char PLUS[];
extern const char DECIMAL_POINT[];
extern const char HAT[];
extern const char DOUBLE_MULTIPLY[];
extern const char ADD[];
extern const char SUBTRACT[];
extern const char MULTIPLY[];
extern const char DIVIDE[];

/* Paired symbols */

extern const char L_BRACE[];
extern const char R_BRACE[];
extern const char L_BRACKET[];
extern const char R_BRACKET[];
extern const char L_PARENTHESIS[];
extern const char R_PARENTHESIS[];
extern const char L_CHEVRON[];
extern const char R_CHEVRON[];

/* Container symbols */

extern const char* CONTAINER_TYPES[];
extern const char* ITERABLE_CONTAINERS[];
/* Opening symbol for each container defined in CONTAINER_TYPES */
extern const char* CONTAINER_OPENING_SYMBOLS[];
/*  Token type string for container opening token */
extern const char* CONTAINER_OPENING_TOKEN_TYPES[];
/* Closing symbol for each container defined in CONTAINER_TYPES */
extern const char* CONTAINER_CLOSING_SYMBOLS[];
/* Token type string for container closing token */
extern const char* CONTAINER_CLOSING_TOKEN_TYPES[];