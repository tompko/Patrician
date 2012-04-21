START_TEST(test_bit_scan_forward)
{
    int i;

    for (i = 0; i < 64; ++i)
    {
        fail_unless(bit_scan_forward(1ull << i) == i);
    }

    fail_unless(bit_scan_forward(1ull << 54 | 1ull << 28) == 28);
    fail_unless(bit_scan_forward(1ull << 38 | 1ull << 54) == 38);
    fail_unless(bit_scan_forward(1ull << 9  | 1ull << 42) == 9);
    fail_unless(bit_scan_forward(1ull << 14 | 1ull << 51) == 14);
    fail_unless(bit_scan_forward(1ull << 23 | 1ull << 61) == 23);
}
END_TEST

START_TEST(test_bit_scan_reverse)
{
    int i;

    for (i = 0; i < 64; ++i)
    {
        fail_unless(bit_scan_reverse(1ull << i) == i);
    }

    fail_unless(bit_scan_reverse(1ull << 54 | 1ull << 28) == 54);
    fail_unless(bit_scan_reverse(1ull << 38 | 1ull << 54) == 54);
    fail_unless(bit_scan_reverse(1ull << 9  | 1ull << 42) == 42);
    fail_unless(bit_scan_reverse(1ull << 14 | 1ull << 51) == 51);
    fail_unless(bit_scan_reverse(1ull << 23 | 1ull << 61) == 61);

}
END_TEST
