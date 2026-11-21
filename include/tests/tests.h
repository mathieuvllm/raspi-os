#ifndef TESTS_H
#define TESTS_H

typedef enum {
	TEST_FAIL = 0,
	TEST_OK = 1,
} test_ret_t;

#define ASSERT(b) if (!(b)) return TEST_FAIL

void run_tests();

#endif
