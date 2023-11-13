#include "test.h"

void test_strings(void)
{
    char str1[] = "bonjour";
    char str2[256];

    assert(SA_CHAR_IS_LOWERCASE('c') == SA_TRUE);
    assert(SA_CHAR_IS_LOWERCASE('H') == SA_FALSE);
    assert(SA_CHAR_IS_LOWERCASE(';') == SA_FALSE);

    assert(SA_CHAR_IS_UPPERCASE('K') == SA_TRUE);
    assert(SA_CHAR_IS_UPPERCASE('i') == SA_FALSE);
    assert(SA_CHAR_IS_UPPERCASE(';') == SA_FALSE);

    SA_str_to_upper_inplace(str1);

    assert(SA_strcmp(str1, "bonjour") != 0);
    assert(SA_strcmp(str1, "BONJOUR") == 0);
    assert(SA_strcasecmp(str1, "bonjour") == 0);
    assert(SA_strcasecmp(str1, "bOnJOuR") == 0);

    assert(SA_strcmp("airplane", "zoro") < 0);
    assert(SA_strcmp("airplane", "ZoRo") > 0);

    assert(SA_strcasecmp("airplane", "zoro") < 0);
    assert(SA_strcasecmp("airplane", "ZoRo") < 0);

    assert(SA_strlen("bonjour") == 7);
    assert(SA_strlen("") == 0);

    assert(SA_toupper('a') == 'A');
    assert(SA_toupper('B') == 'B');

    assert(SA_tolower('a') == 'a');
    assert(SA_tolower('B') == 'b');

    SA_str_to_lower(str2, str1);

    assert(SA_strcmp(str2, "bonjour") == 0);

    SA_str_to_upper(str2, "bOnJour");

    assert(SA_strcmp(str2, "BONJOUR") == 0);

    SA_str_to_lower(str2, "");

    assert(SA_strcmp(str2, "") == 0);

    SA_str_to_lower(str2, "gightr");

    assert(SA_strcmp(SA_uint64_to_str(str2, 567), "567") == 0);
    assert(SA_strcmp(SA_uint64_to_str(str2, 0), "0") == 0);

    assert(SA_strcmp(SA_int64_to_str(str2, 567), "567") == 0);
    assert(SA_strcmp(SA_int64_to_str(str2, -567), "-567") == 0);
    assert(SA_strcmp(SA_int64_to_str(str2, -0), "0") == 0);
    assert(SA_strcmp(SA_int64_to_str(str2, -1), "-1") == 0);

    assert(SA_hex_to_uint64("162E") == 5678);
    assert(SA_hex_to_uint64("162e") == 5678);
    assert(SA_hex_to_uint64("-162E") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_hex_to_uint64("dfg") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_hex_to_uint64("-") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_hex_to_uint64("") == 0 && SA_get_last_error() == SA_ERROR_NAN);

    assert(SA_str_to_uint64("5678") == 5678);
    assert(SA_str_to_uint64("-5678") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_uint64("dfg") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_uint64("-") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_uint64("") == 0 && SA_get_last_error() == SA_ERROR_NAN);

    assert(SA_str_to_int64("5678") == 5678);
    assert(SA_str_to_int64("-5678") == -5678);
    assert(SA_str_to_int64("dfg") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_int64("-") == 0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_int64("") == 0 && SA_get_last_error() == SA_ERROR_NAN);

    assert(SA_str_to_double("12.34") == 12.34);
    assert(SA_str_to_double(".34") == 0.34);
    assert(SA_str_to_double("3.3.4") == 0.0 && SA_get_last_error() == SA_ERROR_NAN);
    assert(SA_str_to_double(".") == 0.0);

    SA_strncpy(str2, "machin", 32);
    assert(SA_strcmp(str2, "machin") == 0);

    SA_strncpy(str2, "trucs", 4);
    assert(SA_strcmp(str2, "tru") == 0);

    assert(SA_startswith("bonjour", "bon") == SA_TRUE);
    assert(SA_startswith("bonjour", "fon") == SA_FALSE);
    assert(SA_startswith("bonjour", "bonjour") == SA_TRUE);
    assert(SA_startswith("bonjour", "bonjours") == SA_FALSE);
    assert(SA_startswith("bonjour", "BON") == SA_FALSE);

    assert(SA_startswith_case_unsensitive("bonjour", "bon") == SA_TRUE);
    assert(SA_startswith_case_unsensitive("bonjour", "fon") == SA_FALSE);
    assert(SA_startswith_case_unsensitive("bonjour", "bOnjoUr") == SA_TRUE);
    assert(SA_startswith_case_unsensitive("bonjour", "bonjours") == SA_FALSE);
    assert(SA_startswith_case_unsensitive("bonjour", "BON") == SA_TRUE);

    assert(SA_str_search("bonjour fanfan la tulipe", "fanfan") == 8);
    assert(SA_str_search("bonjour fanfan la tulipe", "Fanfan") == -1);
    assert(SA_str_search("bonjour fanfan la tulipe", "") == 0);

    assert(SA_str_search_case_unsensitive("bonjour fanfan la tulipe", "fanfan") == 8);
    assert(SA_str_search_case_unsensitive("bonjour fanfan la tulipe", "FaNfAn") == 8);
    assert(SA_str_search_case_unsensitive("bonjour fanfan la tulipe", "camion") == -1);
}