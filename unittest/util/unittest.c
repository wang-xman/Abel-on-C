#include <assert.h>
#include "util.h"

void test_remove_space()
{
    /* space in the middle */
    char test_data[] = "H e l l o, W   o   rl  d";
    remove_space(test_data);
    assert(strcmp(test_data, "Hello,World") == 0);

    /* only space */
    char test_data_2[] = "   ";
    remove_space(test_data_2);
    assert(strcmp(test_data_2, "") == 0);

    /* leading space */
    char test_data_3[] = "  D a    rt  h   ,V a   d e   r";
    remove_space(test_data_3);
    assert(strcmp(test_data_3, "Darth,Vader") == 0);

    /* trailing space */
    char test_data_4[] = "  D a  rt  h   ,V a  d e r       ";
    remove_space(test_data_4);
    assert(strcmp(test_data_4, "Darth,Vader") == 0);

    /* empty */
    char test_data_5[] = "";
    remove_space(test_data_5);
    assert(strcmp(test_data_5, "") == 0);
}

void test_has_letter_on_array()
{
    char test_data_1[] = "H e l l o, W   o   rl  d";
    assert(has_letter(test_data_1) == true);

    char test_data_2[] = "&%98@21";
    assert(has_letter(test_data_2) == false);

    char test_data_3[] = "  &% 98 @21";
    assert(has_letter(test_data_3) == false);

    /* only spaces */
    char test_data_4[] = "  ";
    assert(has_letter(test_data_4) == false);
}

void test_has_letter_on_literal()
{
    /* empty literal */
    char* test_data_5 = "";
    assert(has_letter(test_data_5) == false);

    /* all space literal */
    char* test_data_6 = "     ";
    assert(has_letter(test_data_6) == false);

    /* literal has no letter */
    char* test_data_7 = "  &% 98 @2 . 1 .  ";
    assert(has_letter(test_data_7) == false);

    /* literal has on2 letter */
    char* test_data_8 = "  0x &% 98 @2 . 1 .  ";
    assert(has_letter(test_data_8) == true);
}

void test_begins_with_letter()
{
    /* empty literal */
    char* test_data_5 = "";
    assert(begins_with_letter(test_data_5) == false);

    /* all space literal */
    char* test_data_6 = "     ";
    assert(begins_with_letter(test_data_6) == false);

    /* literal has no letter */
    char* test_data_7 = "  &% 98 @2 . 1 .  ";
    assert(begins_with_letter(test_data_7) == false);

    /* array */

    char test_data_1[] = "H e l l o, W   o   rl  d";
    assert(begins_with_letter(test_data_1) == true);

    char test_data_2[] = "&%98@21";
    assert(begins_with_letter(test_data_2) == false);

    char test_data_3[] = "  &% 98 @21";
    assert(begins_with_letter(test_data_3) == false);

    char test_data_4[] = "a  ";
    assert(begins_with_letter(test_data_4) == true);
}

void test_begins_with_sign()
{
    /* array */
    char test_data_0[] = "";
    assert(begins_with_sign(test_data_0) == false);

    char test_data_1[] = "H e l l o, W   o   rl  d";
    assert(begins_with_sign(test_data_1) == false);

    char test_data_2[] = "+&%98@21";
    assert(begins_with_sign(test_data_2) == true);

    char test_data_3[] = "-  &% 98 @21";
    assert(begins_with_sign(test_data_3) == true);

    char test_data_4[] = " -a  ";
    assert(begins_with_sign(test_data_4) == false);

    /* literal */

    /* just a sign */
    char* test_data_5 = "+";
    assert(begins_with_sign(test_data_5) == true);

    /* sign followed by space */
    char* test_data_6 = "-     ";
    assert(begins_with_sign(test_data_6) == true);

    /* leading space */
    char* test_data_7 = " +-  &% 98 @2 . 1 .  ";
    assert(begins_with_sign(test_data_7) == false);
}

void test_is_algebraic_operator()
{
    char test_plus[] = "+";
    assert(is_algebraic_operator(test_plus) == true);

    char test_minus[] = "-";
    assert(is_algebraic_operator(test_minus) == true);

    char test_mul[] = "*";
    assert(is_algebraic_operator(test_mul) == true);

    char test_div[] = "/";
    assert(is_algebraic_operator(test_div) == true);

    char test_exp_1[] = "**";
    assert(is_algebraic_operator(test_exp_1) == true);

    char test_exp_2[] = "^";
    assert(is_algebraic_operator(test_exp_2) == true);

    char test_no_1[] = "*+";
    assert(is_algebraic_operator(test_no_1) == false);

    char test_no_2[] = " +";    /* leading space */
    assert(is_algebraic_operator(test_no_2) == false);
}

int main()
{
    test_remove_space();
    test_has_letter_on_array();
    test_has_letter_on_literal();
    test_begins_with_letter();
    test_begins_with_sign();
    test_is_algebraic_operator();
}
