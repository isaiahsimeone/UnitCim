#ifndef UNITCIM_H
#define UNITCIM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Toggle output colour */
#define SHOULD_USE_COLOURS       1
/* Maximum number of test cases (increase to allocate more tests & mem) */
#define MAX_TESTS                16384

#if SHOULD_USE_COLOURS
    #define COLOUR_GREEN         "\033[0;32m"
    #define COLOUR_GREEN_BOLD    "\033[1;32m"
    #define COLOUR_RED           "\033[0;31m"
    #define COLOUR_RED_BOLD      "\033[1;31m"
    #define COLOUR_YELLOW        "\033[0;33m"
    #define COLOUR_RESET         "\033[0m"
#else
    #define COLOUR_GREEN         "\033[0m"
    #define COLOUR_GREEN_BOLD    "\033[0m"
    #define COLOUR_RED           "\033[0m"
    #define COLOUR_RED_BOLD      "\033[0m"
    #define COLOUR_YELLOW        "\033[0m"
    #define COLOUR_RESET         "\033[0m"
#endif

/*
 * Assertion 'functions'
 */
#define ASSERT_EQ(x,y)          if (x != y) return (Return){ASSERT_EQ_FAILED, "ASSERT_EQ("#x", "#y")"};
#define ASSERT_NEQ(x,y)         if (x == y) return (Return){ASSERT_NEQ_FAILED, "ASSERT_NEQ("#x", "#y")"};
#define ASSERT_GR(x,y)          if (x <= y) return (Return){ASSERT_GR_FAILED, "ASSERT_GR("#x", "#y")"};
#define ASSERT_LE(x,y)          if (x >= y) return (Return){ASSERT_LE_FAILED, "ASSERT_LE("#x", "#y")"};
#define ASSERT_GEQ(x,y)         if (x < y)  return (Return){ASSERT_GEQ_FAILED, "ASSERT_GEQ("#x", "#y")"};
#define ASSERT_LEQ(x,y)         if (x > y)  return (Return){ASSERT_LEQ_FAILED, "ASSERT_LEQ("#x", "#y")"};
#define ASSERT_STR_EQ(x,y)      if (strlen(x) != strlen(y) || memcmp(x, y, strlen(x)) != 0) return (Return){ASSERT_STR_EQ_FAILED, "ASSERT_STR_EQ("#x", "#y")"};
#define ASSERT_STRN_EQ(x,y,n)   if (memcmp(x,y,n) != 0) return (Return){ASSERT_STRN_EQ_FAILED, "ASSERT_STRN_EQ("#x", "#y", "#n")"};
#define ASSERT_STR_NEQ(x,y)     if (memcmp(x,y,strlen(x)) == 0) (Return){return ASSERT_STR_NEQ_FAILED, "ASSERT_STR_NEQ("#x","#y")"};
#define ASSERT_STRN_NEQ(x,y,n)  if (memcmp(x,y,n) == 0) return (Return){ASSERT_STRN_NEQ_FAILED, "ASSERT_STRN_NEQ("#x", "#y", "#n")"};
#define ASSERT_MEM_EQ(x,y,n)    if (memcmp(x,y,n) != 0) return (Return){ASSERT_MEM_EQ_FAILED, "ASSERT_MEM_EQ("#x", "#y", "#n")"};

#define RETURN_OK               (Return){TEST_OK, "ok"}
#define TEST(x)                 Return x() {
#define ENDTEST                 return RETURN_OK;}
#define PROTOTYPE(x)            Return x(void);

typedef enum TestStatus_ {
    TEST_OK = 0,
    GENERAL_TEST_FAIL = 1,
    ASSERT_EQ_FAILED = 2,
    ASSERT_NEQ_FAILED = 3,
    ASSERT_GR_FAILED = 4,
    ASSERT_LE_FAILED = 5,
    ASSERT_GEQ_FAILED = 6,
    ASSERT_LEQ_FAILED = 7,
    ASSERT_STR_EQ_FAILED = 8,
    ASSERT_STRN_EQ_FAILED = 9,
    ASSERT_STR_NEQ_FAILED = 10,
    ASSERT_STRN_NEQ_FAILED = 11,
    ASSERT_MEM_EQ_FAILED = 12
} TestStatus;

static const char* FAILURE_REASON[] = {
        "OK",
        "General test failure",
        "Assert Equal",
        "Assert Not Equal",
        "Assert Greater Than",
        "Assert Less Than",
        "Assert Greater Than or Equal to",
        "Assert Less Than or Equal to",
        "Assert Strings Equal",
        "Assert Strings Equal (n)",
        "Assert Strings not Equal",
        "Assert Strings not Equal (n)",
        "Assert Memory Equal"
};

typedef struct {
    TestStatus stat;
    const char* exp;
} Return;

typedef struct {
    const char* test_name;
    int test_id;
    Return (*func)(void);
    Return last_run_status;
} TestCase;

typedef struct {
    const char* suite_name;
    TestCase* cases;
    int num_cases;
    int pass_count;
    int fail_count;
    int has_run;
} Suite;

/* Prototypes */
Suite*  new_suite(const char*);
void    destroy_suite(Suite*);
void    register_test(Suite*, const char*, Return (*)(void));
void    print_suite_statistics(Suite*);
void    run_suite(Suite*);

/*
 * Allocates & initialises a testing suite with the
 * specified name. No tests added by this function. Memory
 * for MAX_TESTS is allocated, (see MAX_TESTS macro for more
 * test cases.)
 *
 * suite_name: The readable name assigned to this test suite.
 *
 *    returns: A newly allocated, empty test suite.
 */
Suite* new_suite(const char* suite_name) {
    Suite* suite = malloc(sizeof(Suite));
    memset(suite, 0, sizeof(Suite));

    TestCase* cases = malloc(sizeof(TestCase) * MAX_TESTS);
    memset(cases, 0, sizeof(TestCase) * MAX_TESTS);

    suite->suite_name = suite_name;

    suite->cases = cases;
    suite->num_cases = 0;
    suite->fail_count = 0;
    suite->pass_count = 0;
    suite->has_run = 0;

    return suite;
}

/*
 * Deallocates the memory allocated to the specified suite.
 *
 * suite: The suite to deallocate.
 */
void destroy_suite(Suite* suite) {
    free(suite->cases);
    free(suite);
}

/*
 * Add a test case to the specified test suite.
 *
 *        suite: The test suite to add the test case to.
 *    test_name: The name of the test being added
 * function_ptr: A pointer to the test function (where
 *               testing logic is located.)
 */
void register_test(Suite* suite, const char* test_name, Return (*function_ptr)(void)) {
    int test_idx = suite->num_cases;

    suite->cases[test_idx].test_name = test_name;
    suite->cases[test_idx].test_id = test_idx;
    suite->cases[test_idx].func = function_ptr;
    suite->cases[test_idx].last_run_status = RETURN_OK;

    suite->num_cases += 1;
}

/*
 * Perform all tests that have been registered to the specified suite
 * (see register_test()). Each test is executed in a FIFO fashion.
 * i.e. the first registered test will be executed first.
 *
 * suite: A pointer to the suite containing tests to run.
 */
void run_suite(Suite* suite) {
    suite->has_run = 1;
    Return res = RETURN_OK;
    for (int i = 0; i < suite->num_cases; i++) {
        // record exit result
        res = suite->cases[i].func();
        suite->cases[i].last_run_status = res;
        if (res.stat != TEST_OK)
            suite->fail_count += 1;
        else
            suite->pass_count += 1;
    }
}

/*
 * Print statistics & information about test cases from the most recent
 * execution of tests. Includes pass rate, fail rate, which tests failed
 * and the assertion that resulted in their failure.
 *
 * suite: the test suite to print testing stats/info about.
 */
void print_suite_statistics(Suite* suite) {

    int test_count = suite->num_cases;

    printf("========= %s =========\n", suite->suite_name);

    if (!suite->has_run) {
        printf(COLOUR_YELLOW "Suite tests have not been run (see run_suite())\n");
        return ;
    }

    if (suite->pass_count != 0)
        printf(COLOUR_GREEN "Passed: %d/%d (%.1f%)\n", suite->pass_count, test_count,
            100 * (float)suite->pass_count/(float)test_count);

    // All tests passed.
    if (suite->pass_count == test_count) {
        printf(COLOUR_GREEN_BOLD "\nAll tests OK\n");
        return ;
    }

    printf(COLOUR_RED "Failed: %d/%d (%.1f%)\n", suite->fail_count, test_count,
            100 * (float)suite->fail_count/(float)test_count);

    if (suite->fail_count == test_count) {
        printf(COLOUR_RED_BOLD "\nAll tests failed\n");
    }

    printf("\nThe following test cases failed:\n");

    // Print failed tests and failure reason
    for (int i = 0; i < test_count; i++) {
        if (suite->cases[i].last_run_status.stat != TEST_OK) {
            printf(COLOUR_RED_BOLD "  %s\n", suite->cases[i].test_name);

            printf("     %s" COLOUR_RED " failed\n",
                   FAILURE_REASON[suite->cases[i].last_run_status.stat]);

            printf("     %s\n\n", suite->cases[i].last_run_status.exp);
        }
    }
}

#endif /* UNITCIM_H */