<div align="center">
  
# UnitCim
  
 </div>
 
A basic, single header unit testing library for C.

## Example

```c

#include "unitcim.h"

TEST(test_string_cmp)
    const char* str1 = "Hello, World!";
    const char* str2 = "Hello, W0rld!";

    ASSERT_STR_EQ(str1, str2);
ENDTEST

TEST(int_cmp)
    int a = 100;
    int b = 100;

    ASSERT_EQ(a, b);
    ASSERT_GEQ(a, b);
    ASSERT_LE(a, b);
ENDTEST

TEST(double_cmp)
    double a = 4389.345;
    double b = 4389.345;

    ASSERT_EQ(a, b);
ENDTEST

TEST(memory_cmp)
    char mem_blk_1[100];
    char mem_blk_2[100];

    memset(&mem_blk_1, 1, 100);
    memset(&mem_blk_2, 2, 100);

    ASSERT_MEM_EQ(&mem_blk_1, &mem_blk_2, 100);
ENDTEST


int main(int argc, char** argv) {
    /* Allocate test suite */
    Suite* suite = new_suite("suite 1");

    /* Add test cases */
    register_test(suite, "test_string_cmp", test_string_cmp);
    register_test(suite, "test_int_cmp", int_cmp);
    register_test(suite, "test_double_cmp", double_cmp);
    register_test(suite, "test_memory_cmp", memory_cmp);

    /* Perform registered tests */
    run_suite(suite);

    /* Print stats/information about last run of tests */
    print_suite_statistics(suite);

    /* Deallocate suite */
    destroy_suite(suite);

    return (0);
}
```
### Output
![alt text](/output1.png?raw=true)

## Assertion Macros

| Macro | Description |
| ----- | ------------- |
| ASSERT_EQ(x, y) | Assert equivalence of x and y |
| ASSERT_NEQ(x, y) | Assert that x and y are not equivalent |
| ASSERT_GR(x, y) | Assert that x is greater than y |
| ASSERT_LE(x, y) | Assert that x is less than y |
| ASSERT_GEQ(x, y) | Assert that x is greater than or equal to y |
| ASSERT_LEQ(x, y) | Assert that x is less than or equal to y |
| ASSERT_STR_EQ(x, y) | Assert that the strings x and y are equivalent |
| ASSERT_STRN_EQ(x, y, n) | Assert that the first n bytes of strings x and y are equivalent |
| ASSERT_STR_NEQ(x, y) | Assert that the strings x and y are not equivalent |
| ASSERT_STRN_NEQ(x, y, n) | Assert that the first n bytes of strings x and y are not equivalent |
| ASSERT_MEM_EQ(x, y, n) | Assert that the first n bytes of memory blocks x and y are equivalent |

## Installation
Download unitcim.h and include it in your program 


