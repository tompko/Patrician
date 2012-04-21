START_TEST(test_perft)
{
    Game game;
    int i, j;

    initPerftTests();

    for (i = 0; i < NUM_PERFT_TESTS; ++i)
    {
        printf("FEN: %s\n", perftTests[i].FEN);
        set_game_from_FEN(&game, perftTests[i].FEN);
        for (j = 0; j < 6; ++j)
        {
            if (perftTests[i].perfts[j] >= 0)
            {
                int result = perft(&game, j+1);
                printf("perft %i: %i (%i) %s\n", j+1, result, perftTests[i].perfts[j],
                    result == perftTests[i].perfts[j] ? "PASS" : "FAIL");
                if (result != perftTests[i].perfts[j])
                {
                    char failMessage[512];
                    char* message = failMessage;
                    int numChars = sprintf(message, "\nFail in perft test\n");
                    message += numChars;
                    numChars = sprintf(message, "FEN: %s\n", perftTests[i].FEN);
                    message += numChars;
                    numChars = sprintf(message, "Level: %i\n", j+1);
                    message += numChars;
                    numChars = sprintf(message, "Expected: %i\n", perftTests[i].perfts[j]);
                    message += numChars;
                    numChars = sprintf(message, "Found: %i\n", result);
                    message += numChars;
                    fail(failMessage);
                }
            }
        }
    }

}
END_TEST