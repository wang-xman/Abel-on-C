#include <assert.h>
#include "symbol.h"

void test_selected_symbols()
{
    assert(strcmp(SPACE, " ") == 0);    /* single space */
    assert(strlen(SPACE) == 1);
    assert(strcmp(SPACE, "  ") != 0);   /* not double space */

    assert(strcmp(BACK_TICK, "`") == 0);
    assert(strlen(BACK_TICK) == 1);

    assert(strcmp(SINGLE_QUOTE, "\'") == 0);
    assert(strcmp(DOUBLE_QUOTE, "\"") == 0);
}


int main()
{
    test_selected_symbols();
    return 0;
}