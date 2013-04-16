/*
 * libgovernor - Open-BLDC configuration and debug protocol library
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <check.h>

#include <string.h>

#include "lg/ring.h"

#include "check_suites.h"

struct ring test_ring;
uint8_t data[10];

void init_ring_tc(void)
{
	ring_init(&test_ring, data, 10);
}

void clean_ring_tc(void)
{
}


START_TEST(test_ring_create)
{
	fail_unless(9 == RING_SIZE(&test_ring));
	fail_unless(data == RING_DATA(&test_ring));
}
END_TEST

START_TEST(test_ring_write_read_one)
{
	int32_t ret;
	uint8_t ch;

	ret = ring_write_ch(&test_ring, (uint8_t)'A');

	fail_unless('A' == ret);

	ret = ring_read_ch(&test_ring, &ch);

	fail_unless('A' == ret);
	fail_unless('A' == ch);
}
END_TEST

START_TEST(test_ring_write_read_max)
{
	int32_t ret;
	uint8_t i, ch;

	for(i=0; i<9; i++){
		ret = (uint8_t)ring_write_ch(&test_ring, i);

		fail_unless(i == ret);
	}

	fail_unless(-1 == ring_write_ch(&test_ring, 10));

	for(i=0; i<9; i++){
		ret = (uint8_t)ring_read_ch(&test_ring, &ch);

		fail_unless(i == ret);
		fail_unless(i == ch);
	}

	ch = 10;
	fail_unless(-1 == ring_read_ch(&test_ring, &ch));
	fail_unless(10 == ch);
}
END_TEST

START_TEST(test_ring_write_read_array)
{
	uint8_t array[10];
	uint8_t array1[] = "ABCD";
	uint8_t array2[] = "EFGHIJ";

	fail_unless(4 == ring_write(&test_ring, array1, 4));
	fail_unless(-5 == ring_write(&test_ring, array2, 6));

	memset(array, 0, 10);
	fail_unless(9 == ring_read(&test_ring, array, 10));
	fail_unless(0 == memcmp(array, "ABCDEFGHI", 9));
	fail_unless(0 != memcmp(array, "ABCDEFGHIJ", 10));

	memset(array, 0, 10);
	fail_unless(4 == ring_write(&test_ring, (uint8_t *)"ABCD", 4));
	fail_unless(4 == ring_read(&test_ring, array, 10));
	fail_unless(0 == memcmp(array, "ABCD", 4));

	memset(array, 0, 10);
	fail_unless(6 == ring_write(&test_ring, (uint8_t *)"ABCDEF", 6));
	fail_unless(-4 == ring_read(&test_ring, array, 4));
	fail_unless(0 == memcmp(array, "ABCD", 4));
}
END_TEST

Suite *make_lg_ring_suite()
{
	Suite *s;
	TCase *tc_ring_read_write;

	s= suite_create("Ring buffer");

	tc_ring_read_write = tcase_create("Buffer read/write");
	suite_add_tcase(s, tc_ring_read_write);
	tcase_add_checked_fixture(tc_ring_read_write, init_ring_tc, clean_ring_tc);
	tcase_add_test(tc_ring_read_write, test_ring_create);
	tcase_add_test(tc_ring_read_write, test_ring_write_read_one);
	tcase_add_test(tc_ring_read_write, test_ring_write_read_max);
	tcase_add_test(tc_ring_read_write, test_ring_write_read_array);

	return s;
}
