#include <stdio.h>
#include <assert.h>
#include "SA/SA.h"

SA_bool cmp_int(void* int1, void* int2)
{
    return *((int*)int1) == *((int*)int2);
}

int main()
{
    char str1[] = "bonjour";
    char str2[256];
    int i;

    SA_init();

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

    SA_path_join(str2, 256, 3, "machin", "chose", "bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    SA_path_join(str2, 256, 3, "machin/", "chose/", "bidule");
    assert(SA_strcmp(str2, "machin/chose/bidule") == 0);

    SA_path_join(str1, 8, 2, "cc", "bonjour");
    assert(SA_strcmp(str1, "cc/bonj") == 0);

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

    SA_strncpy(str2, "vive les gnus", 256);
    for(i = 0; str2[i] != '\0'; i++)
    {
        if(SA_parser_search_occurence_in_bytes_stream(str2[i], "les"))
        {
            assert(SA_strcmp(str2+i, "s gnus") == 0);
        }
    }

    assert(SA_int_pow(5, 7) == 78125);
    assert(SA_int_pow(5, 0) == 1);
    assert(SA_int_pow(0, 7) == 0);
    assert(SA_int_pow(0, 0) == 1);

    assert(SA_RELU(5) == 5);
    assert(SA_RELU(-5) == 0);
    assert(SA_RELU(6.7) == 6.7);

    for(int i = 0; i < 100; i++)
    {
        double x = SA_logistic(SA_random_unsecure_float(-1000, 1000));
        assert(x >= 0 && x <= 1.0);
    }
    for(int i = 0; i < 100; i++)
    {
        double x = SA_logistic(SA_random_unsecure_float(-10, 10));
        assert(x > 0 && x < 1.0);
    }

    SA_simplify_path(str2, "../.././.././././truc/chose/./bidule/./../machin/ttyy/eg/f/../../");
    assert(SA_strcmp(str2, "../../../truc/chose/machin/ttyy/") == 0);

    assert(SA_str_search("bonjour fanfan la tulipe", "fanfan") == 8);


    SA_Matrix* mat1 = SA_matrix_create(3, 5);
    SA_Matrix* mat2 = NULL;
    SA_mat_float array[5] = {1, 2, 3, 4, 5};
    SA_mat_float array2[5] = {6792.6, 2, 3, 4, 5};

    SA_matrix_add_line_from_array(mat1, 0, array);
    SA_matrix_add_line_from_array(mat1, 1, array2);
    SA_matrix_add_line_from_array(mat1, 2, array);

    SA_matrix_print(mat1);

    mat2 = SA_matrix_transpose(mat1);

    SA_matrix_print(mat2);

    SA_matrix_free(&mat1);
    SA_matrix_free(&mat2);


    SA_HashMap* hashmap = SA_hashmap_create(cmp_int);
    int k1 = 56;
    int k2 = 56 << 16;
    int v1 = 578;
    int v2 = 23460;

    SA_hashmap_set_value(hashmap, &k1, sizeof(int), &v1);
    SA_hashmap_set_value(hashmap, &k2, sizeof(int), &v2);

    assert(SA_hashmap_get_value(hashmap, &k1, sizeof(int)) == &v1);
    assert(SA_hashmap_get_value(hashmap, &k2, sizeof(int)) == &v2);

    SA_hashmap_free(&hashmap);

    SA_ParserTree* tree = SA_parse_urlencoded_form("word=%C3%A9laborer&machin=truc&chose=bidule");
    assert(SA_strcmp("élaborer", SA_ptree_get_value(tree, "word")) == 0);
    assert(SA_strcmp("truc", SA_ptree_get_value(tree, "machin")) == 0);
    assert(SA_strcmp("bidule", SA_ptree_get_value(tree, "chose")) == 0);

    SA_ptree_free(&tree);


    /*
    ======================================
    ============ Test network ============
    ======================================
    */

    SA_RequestsHandler* handler = NULL;
    char buffer[1024];
    int n;

    handler = SA_req_get(handler, "https://api.coindesdevs.fr/get-exercice?difficulty=2&liked=0&disliked=45&beginners=0&topics_left=0", "");

    SA_req_display_headers(handler);

    while((n = SA_req_read_output_body(handler, buffer, 1023)))
    {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    putchar('\n');

    SA_req_close_connection(&handler);
    
    SA_destroy();
}
