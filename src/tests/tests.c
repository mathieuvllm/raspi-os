#include <tests/tests.h>
#include <common/stdio.h>
#include <tests/mem_tests.h>

void run_tests() {
	if (run_mem_tests() == TEST_FAIL) {
		puts("TESTS FAILED\r\n");
		return;
	}

	puts("TESTS PASSED\r\n");
}
