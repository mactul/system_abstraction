#include "test.h"

void test_path(void)
{
    char str1[8];
    char str2[256];

    SA_path_join(str2, 256, 3, "machin", "chose", "bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    SA_path_join(str2, 256, 3, "machin/", "chose/", "bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    SA_path_join(str2, 256, 3, "machin/", "/chose/", "/bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    SA_path_join(str1, 8, 2, "cc", "bonjour");
    assert(SA_strcmp(str1, "cc/bonj") == 0);

    SA_path_join(str1, 8, 2, "cc", "/bonjour");
    assert(SA_strcmp(str1, "cc/bonj") == 0);

    SA_simplify_path(str2, "../.././.././././truc/chose/./bidule/./../machin/ttyy/eg/f/../../");
    assert(SA_strcmp(str2, "../../../truc/chose/machin/ttyy/") == 0);
}