START_TEST(test_set_from_FEN)
{
    Board* board = (Board*)malloc(sizeof(Board));

    int result = set_from_FEN(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    fail_if((board->pieces[WHITE_ROOK] & (1ULL << A1)) == 0);
    fail_if((board->pieces[WHITE_KNIGHT] & (1ULL << B1)) == 0);
    fail_if((board->pieces[WHITE_BISHOP] & (1ULL << C1)) == 0);
    fail_if((board->pieces[WHITE_QUEEN] & (1ULL << D1)) == 0);
    fail_if((board->pieces[WHITE_KING] & (1ULL << E1)) == 0);
    fail_if((board->pieces[WHITE_BISHOP] & (1ULL << F1)) == 0);
    fail_if((board->pieces[WHITE_KNIGHT] & (1ULL << G1)) == 0);
    fail_if((board->pieces[WHITE_ROOK] & (1ULL << H1)) == 0);

    fail_if((board->pieces[WHITE_PAWN] & (1ULL << A2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << B2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << C2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << D2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << E2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << F2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << G2)) == 0);
    fail_if((board->pieces[WHITE_PAWN] & (1ULL << H2)) == 0);

    fail_if((board->pieces[BLACK_ROOK] & (1ULL << A8)) == 0);
    fail_if((board->pieces[BLACK_KNIGHT] & (1ULL << B8)) == 0);
    fail_if((board->pieces[BLACK_BISHOP] & (1ULL << C8)) == 0);
    fail_if((board->pieces[BLACK_QUEEN] & (1ULL << D8)) == 0);
    fail_if((board->pieces[BLACK_KING] & (1ULL << E8)) == 0);
    fail_if((board->pieces[BLACK_BISHOP] & (1ULL << F8)) == 0);
    fail_if((board->pieces[BLACK_KNIGHT] & (1ULL << G8)) == 0);
    fail_if((board->pieces[BLACK_ROOK] & (1ULL << H8)) == 0);

    fail_if((board->pieces[BLACK_PAWN] & (1ULL << A7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << B7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << C7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << D7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << E7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << F7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << G7)) == 0);
    fail_if((board->pieces[BLACK_PAWN] & (1ULL << H7)) == 0);

    fail_if(board->sideToMove == 0);
    fail_if((board->castling & (1u << WHITE_KINGSIDE)) == 0);
    fail_if((board->castling & (1u << WHITE_QUEENSIDE)) == 0);
    fail_if((board->castling & (1u << BLACK_KINGSIDE)) == 0);
    fail_if((board->castling & (1u << BLACK_QUEENSIDE)) == 0);
    fail_unless(board->enPassant == 0);
    fail_unless(board->halfmove == 0);
    fail_unless(board->move == 1);
    fail_if(result == 0);

    free(board);
}
END_TEST
