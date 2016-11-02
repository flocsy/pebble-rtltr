//#include <pebble.h>
#include "unit.h"
#include "rtltr.h"
#include "rtl.h"

char *str;

void before_each(void) {
}

void after_each(void) {
    if (str != NULL) {
        free(str);
        str = NULL;
    }
}

static char* rtltr_reverse_string_once_test(void) {
    char *orig = "שבת שלום";
    char *expected = "םולש תבש";
    str = malloc(strlen(orig) + 1);
    strcpy(str, orig);
//    rtltr_reverse_string(str);

//    rtltr_register_string(str);
//    rtltr_reverse_registered_strings();
    printf("orig: %p %s\n", orig, orig);
    printf("expected: %p %s\n", expected, expected);
    printf("str before: %p %s, %lu\n", str, str, strlen(str));
    rtl_apply((char*)str, (char*)(str+strlen(str)));
    printf("str after: %p %s\n", str, str);

    mu_assert(!strcmp(expected, str), "reverting once should result in a reversed string");
    return 0;
}

static char* rtltr_reverse_string_twice_test(void) {
    char *orig = "שבת שלום";
    str = malloc(strlen(orig) + 1);
    strcpy(str, orig);
//    rtltr_reverse_string(str);
//    rtltr_reverse_string(str);

//    rtltr_register_string(str);
//    rtltr_reverse_registered_strings();
//    rtltr_reverse_registered_strings();
    rtl_apply((char*)str, (char*)(str+strlen(str)));
    printf("%s\n", orig);
    printf("%s\n", str);
    rtl_apply((char*)str, (char*)(str+strlen(str)));
    printf("%s\n", str);
    mu_assert(!strcmp(orig, str), "reverting twice didn't return the original string");
    return 0;
}

char* all_tests(void) {
  mu_run_test(rtltr_reverse_string_once_test);
  mu_run_test(rtltr_reverse_string_twice_test);
  return 0;
}
