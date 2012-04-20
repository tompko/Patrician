START_TEST(test_bitboard_size)
{
    fail_if(sizeof(bitboard) != 8, "Bitboard is not 64 bits, found %i bits instead", sizeof(bitboard)*8);
}
END_TEST