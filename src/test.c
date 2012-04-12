#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "board.h"

START_TEST(test_bitboard_size)
{
	fail_if(sizeof(bitboard) != 8, "Bitboard is not 64 bits, found %i bits instead", sizeof(bitboard)*8);
}
END_TEST

Suite* board_suite(void)
{
	Suite* s = suite_create("Board");

	TCase* tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_bitboard_size);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
{
	int number_failed;

	Suite* s = board_suite();
	SRunner* sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);

	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;	
}
