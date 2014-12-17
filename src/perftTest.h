#ifndef PERFTTEST_H
#define PERFTTEST_H

typedef struct
{
	char * FEN;
	int perfts[6];
} PerftTest;

#define NUM_PERFT_TESTS (128)

PerftTest perftTests[NUM_PERFT_TESTS];

void initPerftTests(void)
{
	perftTests[0].FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	perftTests[0].perfts[0] = 20;
	perftTests[0].perfts[1] = 400;
	perftTests[0].perfts[2] = 8902;
	perftTests[0].perfts[3] = 197281;
	perftTests[0].perfts[4] = 4865609;
	perftTests[0].perfts[5] = 119060324;

	perftTests[1].FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	perftTests[1].perfts[0] = 48;
	perftTests[1].perfts[1] = 2039;
	perftTests[1].perfts[2] = 97862;
	perftTests[1].perfts[3] = 4085603;
	perftTests[1].perfts[4] = 193690690;
	perftTests[1].perfts[5] = -1;

	perftTests[2].FEN = "4k3/8/8/8/8/8/8/4K2R w K - 0 1";
	perftTests[2].perfts[0] = 15;
	perftTests[2].perfts[1] = 66;
	perftTests[2].perfts[2] = 1197;
	perftTests[2].perfts[3] = 7059;
	perftTests[2].perfts[4] = 133987;
	perftTests[2].perfts[5] = 764643;

	perftTests[3].FEN = "4k3/8/8/8/8/8/8/R3K3 w Q - 0 1";
	perftTests[3].perfts[0] = 16;
	perftTests[3].perfts[1] = 71;
	perftTests[3].perfts[2] = 1287;
	perftTests[3].perfts[3] = 7626;
	perftTests[3].perfts[4] = 145232;
	perftTests[3].perfts[5] = 846648;

	perftTests[4].FEN = "4k2r/8/8/8/8/8/8/4K3 w k - 0 1";
	perftTests[4].perfts[0] = 5;
	perftTests[4].perfts[1] = 75;
	perftTests[4].perfts[2] = 459;
	perftTests[4].perfts[3] = 8290;
	perftTests[4].perfts[4] = 47635;
	perftTests[4].perfts[5] = 899442;

	perftTests[5].FEN = "r3k3/8/8/8/8/8/8/4K3 w q - 0 1";
	perftTests[5].perfts[0] = 5;
	perftTests[5].perfts[1] = 80;
	perftTests[5].perfts[2] = 493;
	perftTests[5].perfts[3] = 8897;
	perftTests[5].perfts[4] = 52710;
	perftTests[5].perfts[5] = 1001523;

	perftTests[6].FEN = "4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1";
	perftTests[6].perfts[0] = 26;
	perftTests[6].perfts[1] = 112;
	perftTests[6].perfts[2] = 3189;
	perftTests[6].perfts[3] = 17945;
	perftTests[6].perfts[4] = 532933;
	perftTests[6].perfts[5] = 2788982;

	perftTests[7].FEN = "r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1";
	perftTests[7].perfts[0] = 5;
	perftTests[7].perfts[1] = 130;
	perftTests[7].perfts[2] = 782;
	perftTests[7].perfts[3] = 22180;
	perftTests[7].perfts[4] = 118882;
	perftTests[7].perfts[5] = 3517770;

	perftTests[8].FEN = "8/8/8/8/8/8/6k1/4K2R w K - 0 1";
	perftTests[8].perfts[0] = 12;
	perftTests[8].perfts[1] = 38;
	perftTests[8].perfts[2] = 564;
	perftTests[8].perfts[3] = 2219;
	perftTests[8].perfts[4] = 37735;
	perftTests[8].perfts[5] = 185867;

	perftTests[9].FEN = "8/8/8/8/8/8/1k6/R3K3 w Q - 0 1";
	perftTests[9].perfts[0] = 15;
	perftTests[9].perfts[1] = 65;
	perftTests[9].perfts[2] = 1018;
	perftTests[9].perfts[3] = 4573;
	perftTests[9].perfts[4] = 80619;
	perftTests[9].perfts[5] = 413018;

	perftTests[10].FEN = "4k2r/6K1/8/8/8/8/8/8 w k - 0 1";
	perftTests[10].perfts[0] = 3;
	perftTests[10].perfts[1] = 32;
	perftTests[10].perfts[2] = 134;
	perftTests[10].perfts[3] = 2073;
	perftTests[10].perfts[4] = 10485;
	perftTests[10].perfts[5] = 179869;

	perftTests[11].FEN = "r3k3/1K6/8/8/8/8/8/8 w q - 0 1";
	perftTests[11].perfts[0] = 4;
	perftTests[11].perfts[1] = 49;
	perftTests[11].perfts[2] = 243;
	perftTests[11].perfts[3] = 3991;
	perftTests[11].perfts[4] = 20780;
	perftTests[11].perfts[5] = 367724;

	perftTests[12].FEN = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
	perftTests[12].perfts[0] = 26;
	perftTests[12].perfts[1] = 568;
	perftTests[12].perfts[2] = 13744;
	perftTests[12].perfts[3] = 314346;
	perftTests[12].perfts[4] = 7594526;
	perftTests[12].perfts[5] = 179862938;

	perftTests[13].FEN = "r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1";
	perftTests[13].perfts[0] = 25;
	perftTests[13].perfts[1] = 567;
	perftTests[13].perfts[2] = 14095;
	perftTests[13].perfts[3] = 328965;
	perftTests[13].perfts[4] = 8153719;
	perftTests[13].perfts[5] = 195629489;

	perftTests[14].FEN = "r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1";
	perftTests[14].perfts[0] = 25;
	perftTests[14].perfts[1] = 548;
	perftTests[14].perfts[2] = 13502;
	perftTests[14].perfts[3] = 312835;
	perftTests[14].perfts[4] = 7736373;
	perftTests[14].perfts[5] = 184411439;

	perftTests[15].FEN = "r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1";
	perftTests[15].perfts[0] = 25;
	perftTests[15].perfts[1] = 547;
	perftTests[15].perfts[2] = 13579;
	perftTests[15].perfts[3] = 316214;
	perftTests[15].perfts[4] = 7878456;
	perftTests[15].perfts[5] = 189224276;

	perftTests[16].FEN = "1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1";
	perftTests[16].perfts[0] = 26;
	perftTests[16].perfts[1] = 583;
	perftTests[16].perfts[2] = 14252;
	perftTests[16].perfts[3] = 334705;
	perftTests[16].perfts[4] = 8198901;
	perftTests[16].perfts[5] = 198328929;

	perftTests[17].FEN = "2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1";
	perftTests[17].perfts[0] = 25;
	perftTests[17].perfts[1] = 560;
	perftTests[17].perfts[2] = 13592;
	perftTests[17].perfts[3] = 317324;
	perftTests[17].perfts[4] = 7710115;
	perftTests[17].perfts[5] = 185959088;

	perftTests[18].FEN = "r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1";
	perftTests[18].perfts[0] = 25;
	perftTests[18].perfts[1] = 560;
	perftTests[18].perfts[2] = 13607;
	perftTests[18].perfts[3] = 320792;
	perftTests[18].perfts[4] = 7848606;
	perftTests[18].perfts[5] = 190755813;

	perftTests[19].FEN = "4k3/8/8/8/8/8/8/4K2R b K - 0 1";
	perftTests[19].perfts[0] = 5;
	perftTests[19].perfts[1] = 75;
	perftTests[19].perfts[2] = 459;
	perftTests[19].perfts[3] = 8290;
	perftTests[19].perfts[4] = 47635;
	perftTests[19].perfts[5] = 899442;

	perftTests[20].FEN = "4k3/8/8/8/8/8/8/R3K3 b Q - 0 1";
	perftTests[20].perfts[0] = 5;
	perftTests[20].perfts[1] = 80;
	perftTests[20].perfts[2] = 493;
	perftTests[20].perfts[3] = 8897;
	perftTests[20].perfts[4] = 52710;
	perftTests[20].perfts[5] = 1001523;

	perftTests[21].FEN = "4k2r/8/8/8/8/8/8/4K3 b k - 0 1";
	perftTests[21].perfts[0] = 15;
	perftTests[21].perfts[1] = 66;
	perftTests[21].perfts[2] = 1197;
	perftTests[21].perfts[3] = 7059;
	perftTests[21].perfts[4] = 133987;
	perftTests[21].perfts[5] = 764643;

	perftTests[22].FEN = "r3k3/8/8/8/8/8/8/4K3 b q - 0 1";
	perftTests[22].perfts[0] = 16;
	perftTests[22].perfts[1] = 71;
	perftTests[22].perfts[2] = 1287;
	perftTests[22].perfts[3] = 7626;
	perftTests[22].perfts[4] = 145232;
	perftTests[22].perfts[5] = 846648;

	perftTests[23].FEN = "4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1";
	perftTests[23].perfts[0] = 5;
	perftTests[23].perfts[1] = 130;
	perftTests[23].perfts[2] = 782;
	perftTests[23].perfts[3] = 22180;
	perftTests[23].perfts[4] = 118882;
	perftTests[23].perfts[5] = 3517770;

	perftTests[24].FEN = "r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1";
	perftTests[24].perfts[0] = 26;
	perftTests[24].perfts[1] = 112;
	perftTests[24].perfts[2] = 3189;
	perftTests[24].perfts[3] = 17945;
	perftTests[24].perfts[4] = 532933;
	perftTests[24].perfts[5] = 2788982;

	perftTests[25].FEN = "8/8/8/8/8/8/6k1/4K2R b K - 0 1";
	perftTests[25].perfts[0] = 3;
	perftTests[25].perfts[1] = 32;
	perftTests[25].perfts[2] = 134;
	perftTests[25].perfts[3] = 2073;
	perftTests[25].perfts[4] = 10485;
	perftTests[25].perfts[5] = 179869;

	perftTests[26].FEN = "8/8/8/8/8/8/1k6/R3K3 b Q - 0 1";
	perftTests[26].perfts[0] = 4;
	perftTests[26].perfts[1] = 49;
	perftTests[26].perfts[2] = 243;
	perftTests[26].perfts[3] = 3991;
	perftTests[26].perfts[4] = 20780;
	perftTests[26].perfts[5] = 367724;

	perftTests[27].FEN = "4k2r/6K1/8/8/8/8/8/8 b k - 0 1";
	perftTests[27].perfts[0] = 12;
	perftTests[27].perfts[1] = 38;
	perftTests[27].perfts[2] = 564;
	perftTests[27].perfts[3] = 2219;
	perftTests[27].perfts[4] = 37735;
	perftTests[27].perfts[5] = 185867;

	perftTests[28].FEN = "r3k3/1K6/8/8/8/8/8/8 b q - 0 1";
	perftTests[28].perfts[0] = 15;
	perftTests[28].perfts[1] = 65;
	perftTests[28].perfts[2] = 1018;
	perftTests[28].perfts[3] = 4573;
	perftTests[28].perfts[4] = 80619;
	perftTests[28].perfts[5] = 413018;

	perftTests[29].FEN = "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1";
	perftTests[29].perfts[0] = 26;
	perftTests[29].perfts[1] = 568;
	perftTests[29].perfts[2] = 13744;
	perftTests[29].perfts[3] = 314346;
	perftTests[29].perfts[4] = 7594526;
	perftTests[29].perfts[5] = 179862938;

	perftTests[30].FEN = "r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1";
	perftTests[30].perfts[0] = 26;
	perftTests[30].perfts[1] = 583;
	perftTests[30].perfts[2] = 14252;
	perftTests[30].perfts[3] = 334705;
	perftTests[30].perfts[4] = 8198901;
	perftTests[30].perfts[5] = 198328929;

	perftTests[31].FEN = "r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1";
	perftTests[31].perfts[0] = 25;
	perftTests[31].perfts[1] = 560;
	perftTests[31].perfts[2] = 13592;
	perftTests[31].perfts[3] = 317324;
	perftTests[31].perfts[4] = 7710115;
	perftTests[31].perfts[5] = 185959088;

	perftTests[32].FEN = "r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1";
	perftTests[32].perfts[0] = 25;
	perftTests[32].perfts[1] = 560;
	perftTests[32].perfts[2] = 13607;
	perftTests[32].perfts[3] = 320792;
	perftTests[32].perfts[4] = 7848606;
	perftTests[32].perfts[5] = 190755813;

	perftTests[33].FEN = "1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1";
	perftTests[33].perfts[0] = 25;
	perftTests[33].perfts[1] = 567;
	perftTests[33].perfts[2] = 14095;
	perftTests[33].perfts[3] = 328965;
	perftTests[33].perfts[4] = 8153719;
	perftTests[33].perfts[5] = 195629489;

	perftTests[34].FEN = "2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1";
	perftTests[34].perfts[0] = 25;
	perftTests[34].perfts[1] = 548;
	perftTests[34].perfts[2] = 13502;
	perftTests[34].perfts[3] = 312835;
	perftTests[34].perfts[4] = 7736373;
	perftTests[34].perfts[5] = 184411439;

	perftTests[35].FEN = "r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1";
	perftTests[35].perfts[0] = 25;
	perftTests[35].perfts[1] = 547;
	perftTests[35].perfts[2] = 13579;
	perftTests[35].perfts[3] = 316214;
	perftTests[35].perfts[4] = 7878456;
	perftTests[35].perfts[5] = 189224276;

	perftTests[36].FEN = "8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1";
	perftTests[36].perfts[0] = 14;
	perftTests[36].perfts[1] = 195;
	perftTests[36].perfts[2] = 2760;
	perftTests[36].perfts[3] = 38675;
	perftTests[36].perfts[4] = 570726;
	perftTests[36].perfts[5] = 8107539;

	perftTests[37].FEN = "8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1";
	perftTests[37].perfts[0] = 11;
	perftTests[37].perfts[1] = 156;
	perftTests[37].perfts[2] = 1636;
	perftTests[37].perfts[3] = 20534;
	perftTests[37].perfts[4] = 223507;
	perftTests[37].perfts[5] = 2594412;

	perftTests[38].FEN = "8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1";
	perftTests[38].perfts[0] = 19;
	perftTests[38].perfts[1] = 289;
	perftTests[38].perfts[2] = 4442;
	perftTests[38].perfts[3] = 73584;
	perftTests[38].perfts[4] = 1198299;
	perftTests[38].perfts[5] = 19870403;

	perftTests[39].FEN = "K7/8/2n5/1n6/8/8/8/k6N w - - 0 1";
	perftTests[39].perfts[0] = 3;
	perftTests[39].perfts[1] = 51;
	perftTests[39].perfts[2] = 345;
	perftTests[39].perfts[3] = 5301;
	perftTests[39].perfts[4] = 38348;
	perftTests[39].perfts[5] = 588695;

	perftTests[40].FEN = "k7/8/2N5/1N6/8/8/8/K6n w - - 0 1";
	perftTests[40].perfts[0] = 17;
	perftTests[40].perfts[1] = 54;
	perftTests[40].perfts[2] = 835;
	perftTests[40].perfts[3] = 5910;
	perftTests[40].perfts[4] = 92250;
	perftTests[40].perfts[5] = 688780;

	perftTests[41].FEN = "8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1";
	perftTests[41].perfts[0] = 15;
	perftTests[41].perfts[1] = 193;
	perftTests[41].perfts[2] = 2816;
	perftTests[41].perfts[3] = 40039;
	perftTests[41].perfts[4] = 582642;
	perftTests[41].perfts[5] = 8503277;

	perftTests[42].FEN = "8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1";
	perftTests[42].perfts[0] = 16;
	perftTests[42].perfts[1] = 180;
	perftTests[42].perfts[2] = 2290;
	perftTests[42].perfts[3] = 24640;
	perftTests[42].perfts[4] = 288141;
	perftTests[42].perfts[5] = 3147566;

	perftTests[43].FEN = "8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1";
	perftTests[43].perfts[0] = 4;
	perftTests[43].perfts[1] = 68;
	perftTests[43].perfts[2] = 1118;
	perftTests[43].perfts[3] = 16199;
	perftTests[43].perfts[4] = 281190;
	perftTests[43].perfts[5] = 4405103;

	perftTests[44].FEN = "K7/8/2n5/1n6/8/8/8/k6N b - - 0 1";
	perftTests[44].perfts[0] = 17;
	perftTests[44].perfts[1] = 54;
	perftTests[44].perfts[2] = 835;
	perftTests[44].perfts[3] = 5910;
	perftTests[44].perfts[4] = 92250;
	perftTests[44].perfts[5] = 688780;

	perftTests[45].FEN = "k7/8/2N5/1N6/8/8/8/K6n b - - 0 1";
	perftTests[45].perfts[0] = 3;
	perftTests[45].perfts[1] = 51;
	perftTests[45].perfts[2] = 345;
	perftTests[45].perfts[3] = 5301;
	perftTests[45].perfts[4] = 38348;
	perftTests[45].perfts[5] = 588695;

	perftTests[46].FEN = "B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1";
	perftTests[46].perfts[0] = 17;
	perftTests[46].perfts[1] = 278;
	perftTests[46].perfts[2] = 4607;
	perftTests[46].perfts[3] = 76778;
	perftTests[46].perfts[4] = 1320507;
	perftTests[46].perfts[5] = 22823890;

	perftTests[47].FEN = "8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1";
	perftTests[47].perfts[0] = 21;
	perftTests[47].perfts[1] = 316;
	perftTests[47].perfts[2] = 5744;
	perftTests[47].perfts[3] = 93338;
	perftTests[47].perfts[4] = 1713368;
	perftTests[47].perfts[5] = 28861171;

	perftTests[48].FEN = "k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1";
	perftTests[48].perfts[0] = 21;
	perftTests[48].perfts[1] = 144;
	perftTests[48].perfts[2] = 3242;
	perftTests[48].perfts[3] = 32955;
	perftTests[48].perfts[4] = 787524;
	perftTests[48].perfts[5] = 7881673;

	perftTests[49].FEN = "K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1";
	perftTests[49].perfts[0] = 7;
	perftTests[49].perfts[1] = 143;
	perftTests[49].perfts[2] = 1416;
	perftTests[49].perfts[3] = 31787;
	perftTests[49].perfts[4] = 310862;
	perftTests[49].perfts[5] = 7382896;

	perftTests[50].FEN = "B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1";
	perftTests[50].perfts[0] = 6;
	perftTests[50].perfts[1] = 106;
	perftTests[50].perfts[2] = 1829;
	perftTests[50].perfts[3] = 31151;
	perftTests[50].perfts[4] = 530585;
	perftTests[50].perfts[5] = 9250746;

	perftTests[51].FEN = "8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1";
	perftTests[51].perfts[0] = 17;
	perftTests[51].perfts[1] = 309;
	perftTests[51].perfts[2] = 5133;
	perftTests[51].perfts[3] = 93603;
	perftTests[51].perfts[4] = 1591064;
	perftTests[51].perfts[5] = 29027891;

	perftTests[52].FEN = "k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1";
	perftTests[52].perfts[0] = 7;
	perftTests[52].perfts[1] = 143;
	perftTests[52].perfts[2] = 1416;
	perftTests[52].perfts[3] = 31787;
	perftTests[52].perfts[4] = 310862;
	perftTests[52].perfts[5] = 7382896;

	perftTests[53].FEN = "K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1";
	perftTests[53].perfts[0] = 21;
	perftTests[53].perfts[1] = 144;
	perftTests[53].perfts[2] = 3242;
	perftTests[53].perfts[3] = 32955;
	perftTests[53].perfts[4] = 787524;
	perftTests[53].perfts[5] = 7881673;

	perftTests[54].FEN = "7k/RR6/8/8/8/8/rr6/7K w - - 0 1";
	perftTests[54].perfts[0] = 19;
	perftTests[54].perfts[1] = 275;
	perftTests[54].perfts[2] = 5300;
	perftTests[54].perfts[3] = 104342;
	perftTests[54].perfts[4] = 2161211;
	perftTests[54].perfts[5] = 44956585;

	perftTests[55].FEN = "R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1";
	perftTests[55].perfts[0] = 36;
	perftTests[55].perfts[1] = 1027;
	perftTests[55].perfts[2] = 29215;
	perftTests[55].perfts[3] = 771461;
	perftTests[55].perfts[4] = 20506480;
	perftTests[55].perfts[5] = 525169084;

	perftTests[56].FEN = "7k/RR6/8/8/8/8/rr6/7K b - - 0 1";
	perftTests[56].perfts[0] = 19;
	perftTests[56].perfts[1] = 275;
	perftTests[56].perfts[2] = 5300;
	perftTests[56].perfts[3] = 104342;
	perftTests[56].perfts[4] = 2161211;
	perftTests[56].perfts[5] = 44956585;

	perftTests[57].FEN = "R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1";
	perftTests[57].perfts[0] = 36;
	perftTests[57].perfts[1] = 1027;
	perftTests[57].perfts[2] = 29227;
	perftTests[57].perfts[3] = 771368;
	perftTests[57].perfts[4] = 20521342;
	perftTests[57].perfts[5] = 524966748;

	perftTests[58].FEN = "6kq/8/8/8/8/8/8/7K w - - 0 1";
	perftTests[58].perfts[0] = 2;
	perftTests[58].perfts[1] = 36;
	perftTests[58].perfts[2] = 143;
	perftTests[58].perfts[3] = 3637;
	perftTests[58].perfts[4] = 14893;
	perftTests[58].perfts[5] = 391507;

	perftTests[59].FEN = "6KQ/8/8/8/8/8/8/7k b - - 0 1";
	perftTests[59].perfts[0] = 2;
	perftTests[59].perfts[1] = 36;
	perftTests[59].perfts[2] = 143;
	perftTests[59].perfts[3] = 3637;
	perftTests[59].perfts[4] = 14893;
	perftTests[59].perfts[5] = 391507;

	perftTests[60].FEN = "K7/8/8/3Q4/4q3/8/8/7k w - - 0 1";
	perftTests[60].perfts[0] = 6;
	perftTests[60].perfts[1] = 35;
	perftTests[60].perfts[2] = 495;
	perftTests[60].perfts[3] = 8349;
	perftTests[60].perfts[4] = 166741;
	perftTests[60].perfts[5] = 3370175;

	perftTests[61].FEN = "6qk/8/8/8/8/8/8/7K b - - 0 1";
	perftTests[61].perfts[0] = 22;
	perftTests[61].perfts[1] = 43;
	perftTests[61].perfts[2] = 1015;
	perftTests[61].perfts[3] = 4167;
	perftTests[61].perfts[4] = 105749;
	perftTests[61].perfts[5] = 419369;

	perftTests[62].FEN = "6KQ/8/8/8/8/8/8/7k b - - 0 1";
	perftTests[62].perfts[0] = 2;
	perftTests[62].perfts[1] = 36;
	perftTests[62].perfts[2] = 143;
	perftTests[62].perfts[3] = 3637;
	perftTests[62].perfts[4] = 14893;
	perftTests[62].perfts[5] = 391507;

	perftTests[63].FEN = "K7/8/8/3Q4/4q3/8/8/7k b - - 0 1";
	perftTests[63].perfts[0] = 6;
	perftTests[63].perfts[1] = 35;
	perftTests[63].perfts[2] = 495;
	perftTests[63].perfts[3] = 8349;
	perftTests[63].perfts[4] = 166741;
	perftTests[63].perfts[5] = 3370175;

	perftTests[64].FEN = "8/8/8/8/8/K7/P7/k7 w - - 0 1";
	perftTests[64].perfts[0] = 3;
	perftTests[64].perfts[1] = 7;
	perftTests[64].perfts[2] = 43;
	perftTests[64].perfts[3] = 199;
	perftTests[64].perfts[4] = 1347;
	perftTests[64].perfts[5] = 6249;

	perftTests[65].FEN = "8/8/8/8/8/7K/7P/7k w - - 0 1";
	perftTests[65].perfts[0] = 3;
	perftTests[65].perfts[1] = 7;
	perftTests[65].perfts[2] = 43;
	perftTests[65].perfts[3] = 199;
	perftTests[65].perfts[4] = 1347;
	perftTests[65].perfts[5] = 6249;

	perftTests[66].FEN = "K7/p7/k7/8/8/8/8/8 w - - 0 1";
	perftTests[66].perfts[0] = 1;
	perftTests[66].perfts[1] = 3;
	perftTests[66].perfts[2] = 12;
	perftTests[66].perfts[3] = 80;
	perftTests[66].perfts[4] = 342;
	perftTests[66].perfts[5] = 2343;

	perftTests[67].FEN = "7K/7p/7k/8/8/8/8/8 w - - 0 1";
	perftTests[67].perfts[0] = 1;
	perftTests[67].perfts[1] = 3;
	perftTests[67].perfts[2] = 12;
	perftTests[67].perfts[3] = 80;
	perftTests[67].perfts[4] = 342;
	perftTests[67].perfts[5] = 2343;

	perftTests[68].FEN = "8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1";
	perftTests[68].perfts[0] = 7;
	perftTests[68].perfts[1] = 35;
	perftTests[68].perfts[2] = 210;
	perftTests[68].perfts[3] = 1091;
	perftTests[68].perfts[4] = 7028;
	perftTests[68].perfts[5] = 34834;

	perftTests[69].FEN = "8/8/8/8/8/K7/P7/k7 b - - 0 1";
	perftTests[69].perfts[0] = 1;
	perftTests[69].perfts[1] = 3;
	perftTests[69].perfts[2] = 12;
	perftTests[69].perfts[3] = 80;
	perftTests[69].perfts[4] = 342;
	perftTests[69].perfts[5] = 2343;

	perftTests[70].FEN = "8/8/8/8/8/7K/7P/7k b - - 0 1";
	perftTests[70].perfts[0] = 1;
	perftTests[70].perfts[1] = 3;
	perftTests[70].perfts[2] = 12;
	perftTests[70].perfts[3] = 80;
	perftTests[70].perfts[4] = 342;
	perftTests[70].perfts[5] = 2343;

	perftTests[71].FEN = "K7/p7/k7/8/8/8/8/8 b - - 0 1";
	perftTests[71].perfts[0] = 3;
	perftTests[71].perfts[1] = 7;
	perftTests[71].perfts[2] = 43;
	perftTests[71].perfts[3] = 199;
	perftTests[71].perfts[4] = 1347;
	perftTests[71].perfts[5] = 6249;

	perftTests[72].FEN = "7K/7p/7k/8/8/8/8/8 b - - 0 1";
	perftTests[72].perfts[0] = 3;
	perftTests[72].perfts[1] = 7;
	perftTests[72].perfts[2] = 43;
	perftTests[72].perfts[3] = 199;
	perftTests[72].perfts[4] = 1347;
	perftTests[72].perfts[5] = 6249;

	perftTests[73].FEN = "8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1";
	perftTests[73].perfts[0] = 5;
	perftTests[73].perfts[1] = 35;
	perftTests[73].perfts[2] = 182;
	perftTests[73].perfts[3] = 1091;
	perftTests[73].perfts[4] = 5408;
	perftTests[73].perfts[5] = 34822;

	perftTests[74].FEN = "8/8/8/8/8/4k3/4P3/4K3 w - - 0 1";
	perftTests[74].perfts[0] = 2;
	perftTests[74].perfts[1] = 8;
	perftTests[74].perfts[2] = 44;
	perftTests[74].perfts[3] = 282;
	perftTests[74].perfts[4] = 1814;
	perftTests[74].perfts[5] = 11848;

	perftTests[75].FEN = "4k3/4p3/4K3/8/8/8/8/8 b - - 0 1";
	perftTests[75].perfts[0] = 2;
	perftTests[75].perfts[1] = 8;
	perftTests[75].perfts[2] = 44;
	perftTests[75].perfts[3] = 282;
	perftTests[75].perfts[4] = 1814;
	perftTests[75].perfts[5] = 11848;

	perftTests[76].FEN = "8/8/7k/7p/7P/7K/8/8 w - - 0 1";
	perftTests[76].perfts[0] = 3;
	perftTests[76].perfts[1] = 9;
	perftTests[76].perfts[2] = 57;
	perftTests[76].perfts[3] = 360;
	perftTests[76].perfts[4] = 1969;
	perftTests[76].perfts[5] = 10724;

	perftTests[77].FEN = "8/8/k7/p7/P7/K7/8/8 w - - 0 1";
	perftTests[77].perfts[0] = 3;
	perftTests[77].perfts[1] = 9;
	perftTests[77].perfts[2] = 57;
	perftTests[77].perfts[3] = 360;
	perftTests[77].perfts[4] = 1969;
	perftTests[77].perfts[5] = 10724;

	perftTests[78].FEN = "8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1";
	perftTests[78].perfts[0] = 5;
	perftTests[78].perfts[1] = 25;
	perftTests[78].perfts[2] = 180;
	perftTests[78].perfts[3] = 1294;
	perftTests[78].perfts[4] = 8296;
	perftTests[78].perfts[5] = 53138;

	perftTests[79].FEN = "8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1";
	perftTests[79].perfts[0] = 8;
	perftTests[79].perfts[1] = 61;
	perftTests[79].perfts[2] = 483;
	perftTests[79].perfts[3] = 3213;
	perftTests[79].perfts[4] = 23599;
	perftTests[79].perfts[5] = 157093;

	perftTests[80].FEN = "8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1";
	perftTests[80].perfts[0] = 8;
	perftTests[80].perfts[1] = 61;
	perftTests[80].perfts[2] = 411;
	perftTests[80].perfts[3] = 3213;
	perftTests[80].perfts[4] = 21637;
	perftTests[80].perfts[5] = 158065;

	perftTests[81].FEN = "k7/8/3p4/8/3P4/8/8/7K w - - 0 1";
	perftTests[81].perfts[0] = 4;
	perftTests[81].perfts[1] = 15;
	perftTests[81].perfts[2] = 90;
	perftTests[81].perfts[3] = 534;
	perftTests[81].perfts[4] = 3450;
	perftTests[81].perfts[5] = 20960;

	perftTests[82].FEN = "8/8/7k/7p/7P/7K/8/8 b - - 0 1";
	perftTests[82].perfts[0] = 3;
	perftTests[82].perfts[1] = 9;
	perftTests[82].perfts[2] = 57;
	perftTests[82].perfts[3] = 360;
	perftTests[82].perfts[4] = 1969;
	perftTests[82].perfts[5] = 10724;

	perftTests[83].FEN = "8/8/k7/p7/P7/K7/8/8 b - - 0 1";
	perftTests[83].perfts[0] = 3;
	perftTests[83].perfts[1] = 9;
	perftTests[83].perfts[2] = 57;
	perftTests[83].perfts[3] = 360;
	perftTests[83].perfts[4] = 1969;
	perftTests[83].perfts[5] = 10724;

	perftTests[84].FEN = "8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1";
	perftTests[84].perfts[0] = 5;
	perftTests[84].perfts[1] = 25;
	perftTests[84].perfts[2] = 180;
	perftTests[84].perfts[3] = 1294;
	perftTests[84].perfts[4] = 8296;
	perftTests[84].perfts[5] = 53138;

	perftTests[85].FEN = "8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1";
	perftTests[85].perfts[0] = 8;
	perftTests[85].perfts[1] = 61;
	perftTests[85].perfts[2] = 411;
	perftTests[85].perfts[3] = 3213;
	perftTests[85].perfts[4] = 21637;
	perftTests[85].perfts[5] = 158065;

	perftTests[86].FEN = "8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1";
	perftTests[86].perfts[0] = 8;
	perftTests[86].perfts[1] = 61;
	perftTests[86].perfts[2] = 483;
	perftTests[86].perfts[3] = 3213;
	perftTests[86].perfts[4] = 23599;
	perftTests[86].perfts[5] = 157093;

	perftTests[87].FEN = "k7/8/3p4/8/3P4/8/8/7K b - - 0 1";
	perftTests[87].perfts[0] = 4;
	perftTests[87].perfts[1] = 15;
	perftTests[87].perfts[2] = 89;
	perftTests[87].perfts[3] = 537;
	perftTests[87].perfts[4] = 3309;
	perftTests[87].perfts[5] = 21104;

	perftTests[88].FEN = "7k/3p4/8/8/3P4/8/8/K7 w - - 0 1";
	perftTests[88].perfts[0] = 4;
	perftTests[88].perfts[1] = 19;
	perftTests[88].perfts[2] = 117;
	perftTests[88].perfts[3] = 720;
	perftTests[88].perfts[4] = 4661;
	perftTests[88].perfts[5] = 32191;

	perftTests[89].FEN = "7k/8/8/3p4/8/8/3P4/K7 w - - 0 1";
	perftTests[89].perfts[0] = 5;
	perftTests[89].perfts[1] = 19;
	perftTests[89].perfts[2] = 116;
	perftTests[89].perfts[3] = 716;
	perftTests[89].perfts[4] = 4786;
	perftTests[89].perfts[5] = 30980;

	perftTests[90].FEN = "k7/8/8/7p/6P1/8/8/K7 w - - 0 1";
	perftTests[90].perfts[0] = 5;
	perftTests[90].perfts[1] = 22;
	perftTests[90].perfts[2] = 139;
	perftTests[90].perfts[3] = 877;
	perftTests[90].perfts[4] = 6112;
	perftTests[90].perfts[5] = 41874;

	perftTests[91].FEN = "k7/8/7p/8/8/6P1/8/K7 w - - 0 1";
	perftTests[91].perfts[0] = 4;
	perftTests[91].perfts[1] = 16;
	perftTests[91].perfts[2] = 101;
	perftTests[91].perfts[3] = 637;
	perftTests[91].perfts[4] = 4354;
	perftTests[91].perfts[5] = 29679;

	perftTests[92].FEN = "k7/8/8/6p1/7P/8/8/K7 w - - 0 1";
	perftTests[92].perfts[0] = 5;
	perftTests[92].perfts[1] = 22;
	perftTests[92].perfts[2] = 139;
	perftTests[92].perfts[3] = 877;
	perftTests[92].perfts[4] = 6112;
	perftTests[92].perfts[5] = 41874;

	perftTests[93].FEN = "k7/8/6p1/8/8/7P/8/K7 w - - 0 1";
	perftTests[93].perfts[0] = 4;
	perftTests[93].perfts[1] = 16;
	perftTests[93].perfts[2] = 101;
	perftTests[93].perfts[3] = 637;
	perftTests[93].perfts[4] = 4354;
	perftTests[93].perfts[5] = 29679;

	perftTests[94].FEN = "k7/8/8/3p4/4p3/8/8/7K w - - 0 1";
	perftTests[94].perfts[0] = 3;
	perftTests[94].perfts[1] = 15;
	perftTests[94].perfts[2] = 84;
	perftTests[94].perfts[3] = 573;
	perftTests[94].perfts[4] = 3013;
	perftTests[94].perfts[5] = 22886;

	perftTests[95].FEN = "k7/8/3p4/8/8/4P3/8/7K w - - 0 1";
	perftTests[95].perfts[0] = 4;
	perftTests[95].perfts[1] = 16;
	perftTests[95].perfts[2] = 101;
	perftTests[95].perfts[3] = 637;
	perftTests[95].perfts[4] = 4271;
	perftTests[95].perfts[5] = 28662;

	perftTests[96].FEN = "7k/3p4/8/8/3P4/8/8/K7 b - - 0 1";
	perftTests[96].perfts[0] = 5;
	perftTests[96].perfts[1] = 19;
	perftTests[96].perfts[2] = 117;
	perftTests[96].perfts[3] = 720;
	perftTests[96].perfts[4] = 5014;
	perftTests[96].perfts[5] = 32167;

	perftTests[97].FEN = "7k/8/8/3p4/8/8/3P4/K7 b - - 0 1";
	perftTests[97].perfts[0] = 4;
	perftTests[97].perfts[1] = 19;
	perftTests[97].perfts[2] = 117;
	perftTests[97].perfts[3] = 712;
	perftTests[97].perfts[4] = 4658;
	perftTests[97].perfts[5] = 30749;

	perftTests[98].FEN = "k7/8/8/7p/6P1/8/8/K7 b - - 0 1";
	perftTests[98].perfts[0] = 5;
	perftTests[98].perfts[1] = 22;
	perftTests[98].perfts[2] = 139;
	perftTests[98].perfts[3] = 877;
	perftTests[98].perfts[4] = 6112;
	perftTests[98].perfts[5] = 41874;

	perftTests[99].FEN = "k7/8/7p/8/8/6P1/8/K7 b - - 0 1";
	perftTests[99].perfts[0] = 4;
	perftTests[99].perfts[1] = 16;
	perftTests[99].perfts[2] = 101;
	perftTests[99].perfts[3] = 637;
	perftTests[99].perfts[4] = 4354;
	perftTests[99].perfts[5] = 29679;

	perftTests[100].FEN = "k7/8/8/6p1/7P/8/8/K7 b - - 0 1";
	perftTests[100].perfts[0] = 5;
	perftTests[100].perfts[1] = 22;
	perftTests[100].perfts[2] = 139;
	perftTests[100].perfts[3] = 877;
	perftTests[100].perfts[4] = 6112;
	perftTests[100].perfts[5] = 41874;

	perftTests[101].FEN = "k7/8/6p1/8/8/7P/8/K7 b - - 0 1";
	perftTests[101].perfts[0] = 4;
	perftTests[101].perfts[1] = 16;
	perftTests[101].perfts[2] = 101;
	perftTests[101].perfts[3] = 637;
	perftTests[101].perfts[4] = 4354;
	perftTests[101].perfts[5] = 29679;

	perftTests[102].FEN = "k7/8/8/3p4/4p3/8/8/7K b - - 0 1";
	perftTests[102].perfts[0] = 5;
	perftTests[102].perfts[1] = 15;
	perftTests[102].perfts[2] = 102;
	perftTests[102].perfts[3] = 569;
	perftTests[102].perfts[4] = 4337;
	perftTests[102].perfts[5] = 22579;

	perftTests[103].FEN = "k7/8/3p4/8/8/4P3/8/7K b - - 0 1";
	perftTests[103].perfts[0] = 4;
	perftTests[103].perfts[1] = 16;
	perftTests[103].perfts[2] = 101;
	perftTests[103].perfts[3] = 637;
	perftTests[103].perfts[4] = 4271;
	perftTests[103].perfts[5] = 28662;

	perftTests[104].FEN = "7k/8/8/p7/1P6/8/8/7K w - - 0 1";
	perftTests[104].perfts[0] = 5;
	perftTests[104].perfts[1] = 22;
	perftTests[104].perfts[2] = 139;
	perftTests[104].perfts[3] = 877;
	perftTests[104].perfts[4] = 6112;
	perftTests[104].perfts[5] = 41874;

	perftTests[105].FEN = "7k/8/p7/8/8/1P6/8/7K w - - 0 1";
	perftTests[105].perfts[0] = 4;
	perftTests[105].perfts[1] = 16;
	perftTests[105].perfts[2] = 101;
	perftTests[105].perfts[3] = 637;
	perftTests[105].perfts[4] = 4354;
	perftTests[105].perfts[5] = 29679;

	perftTests[106].FEN = "7k/8/8/1p6/P7/8/8/7K w - - 0 1";
	perftTests[106].perfts[0] = 5;
	perftTests[106].perfts[1] = 22;
	perftTests[106].perfts[2] = 139;
	perftTests[106].perfts[3] = 877;
	perftTests[106].perfts[4] = 6112;
	perftTests[106].perfts[5] = 41874;

	perftTests[107].FEN = "7k/8/1p6/8/8/P7/8/7K w - - 0 1";
	perftTests[107].perfts[0] = 4;
	perftTests[107].perfts[1] = 16;
	perftTests[107].perfts[2] = 101;
	perftTests[107].perfts[3] = 637;
	perftTests[107].perfts[4] = 4354;
	perftTests[107].perfts[5] = 29679;

	perftTests[108].FEN = "k7/7p/8/8/8/8/6P1/K7 w - - 0 1";
	perftTests[108].perfts[0] = 5;
	perftTests[108].perfts[1] = 25;
	perftTests[108].perfts[2] = 161;
	perftTests[108].perfts[3] = 1035;
	perftTests[108].perfts[4] = 7574;
	perftTests[108].perfts[5] = 55338;

	perftTests[109].FEN = "k7/6p1/8/8/8/8/7P/K7 w - - 0 1";
	perftTests[109].perfts[0] = 5;
	perftTests[109].perfts[1] = 25;
	perftTests[109].perfts[2] = 161;
	perftTests[109].perfts[3] = 1035;
	perftTests[109].perfts[4] = 7574;
	perftTests[109].perfts[5] = 55338;

	perftTests[110].FEN = "3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1";
	perftTests[110].perfts[0] = 7;
	perftTests[110].perfts[1] = 49;
	perftTests[110].perfts[2] = 378;
	perftTests[110].perfts[3] = 2902;
	perftTests[110].perfts[4] = 24122;
	perftTests[110].perfts[5] = 199002;

	perftTests[111].FEN = "7k/8/8/p7/1P6/8/8/7K b - - 0 1";
	perftTests[111].perfts[0] = 5;
	perftTests[111].perfts[1] = 22;
	perftTests[111].perfts[2] = 139;
	perftTests[111].perfts[3] = 877;
	perftTests[111].perfts[4] = 6112;
	perftTests[111].perfts[5] = 41874;

	perftTests[112].FEN = "7k/8/p7/8/8/1P6/8/7K b - - 0 1";
	perftTests[112].perfts[0] = 4;
	perftTests[112].perfts[1] = 16;
	perftTests[112].perfts[2] = 101;
	perftTests[112].perfts[3] = 637;
	perftTests[112].perfts[4] = 4354;
	perftTests[112].perfts[5] = 29679;

	perftTests[113].FEN = "7k/8/8/1p6/P7/8/8/7K b - - 0 1";
	perftTests[113].perfts[0] = 5;
	perftTests[113].perfts[1] = 22;
	perftTests[113].perfts[2] = 139;
	perftTests[113].perfts[3] = 877;
	perftTests[113].perfts[4] = 6112;
	perftTests[113].perfts[5] = 41874;

	perftTests[114].FEN = "7k/8/1p6/8/8/P7/8/7K b - - 0 1";
	perftTests[114].perfts[0] = 4;
	perftTests[114].perfts[1] = 16;
	perftTests[114].perfts[2] = 101;
	perftTests[114].perfts[3] = 637;
	perftTests[114].perfts[4] = 4354;
	perftTests[114].perfts[5] = 29679;

	perftTests[115].FEN = "k7/7p/8/8/8/8/6P1/K7 b - - 0 1";
	perftTests[115].perfts[0] = 5;
	perftTests[115].perfts[1] = 25;
	perftTests[115].perfts[2] = 161;
	perftTests[115].perfts[3] = 1035;
	perftTests[115].perfts[4] = 7574;
	perftTests[115].perfts[5] = 55338;

	perftTests[116].FEN = "k7/6p1/8/8/8/8/7P/K7 b - - 0 1";
	perftTests[116].perfts[0] = 5;
	perftTests[116].perfts[1] = 25;
	perftTests[116].perfts[2] = 161;
	perftTests[116].perfts[3] = 1035;
	perftTests[116].perfts[4] = 7574;
	perftTests[116].perfts[5] = 55338;

	perftTests[117].FEN = "3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1";
	perftTests[117].perfts[0] = 7;
	perftTests[117].perfts[1] = 49;
	perftTests[117].perfts[2] = 378;
	perftTests[117].perfts[3] = 2902;
	perftTests[117].perfts[4] = 24122;
	perftTests[117].perfts[5] = 199002;

	perftTests[118].FEN = "8/Pk6/8/8/8/8/6Kp/8 w - - 0 1";
	perftTests[118].perfts[0] = 11;
	perftTests[118].perfts[1] = 97;
	perftTests[118].perfts[2] = 887;
	perftTests[118].perfts[3] = 8048;
	perftTests[118].perfts[4] = 90606;
	perftTests[118].perfts[5] = 1030499;

	perftTests[119].FEN = "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1";
	perftTests[119].perfts[0] = 24;
	perftTests[119].perfts[1] = 421;
	perftTests[119].perfts[2] = 7421;
	perftTests[119].perfts[3] = 124608;
	perftTests[119].perfts[4] = 2193768;
	perftTests[119].perfts[5] = 37665329;

	perftTests[120].FEN = "8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1";
	perftTests[120].perfts[0] = 18;
	perftTests[120].perfts[1] = 270;
	perftTests[120].perfts[2] = 4699;
	perftTests[120].perfts[3] = 79355;
	perftTests[120].perfts[4] = 1533145;
	perftTests[120].perfts[5] = 28859283;

	perftTests[121].FEN = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1";
	perftTests[121].perfts[0] = 24;
	perftTests[121].perfts[1] = 496;
	perftTests[121].perfts[2] = 9483;
	perftTests[121].perfts[3] = 182838;
	perftTests[121].perfts[4] = 3605103;
	perftTests[121].perfts[5] = 71179139;

	perftTests[122].FEN = "8/Pk6/8/8/8/8/6Kp/8 b - - 0 1";
	perftTests[122].perfts[0] = 11;
	perftTests[122].perfts[1] = 97;
	perftTests[122].perfts[2] = 887;
	perftTests[122].perfts[3] = 8048;
	perftTests[122].perfts[4] = 90606;
	perftTests[122].perfts[5] = 1030499;

	perftTests[123].FEN = "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1";
	perftTests[123].perfts[0] = 24;
	perftTests[123].perfts[1] = 421;
	perftTests[123].perfts[2] = 7421;
	perftTests[123].perfts[3] = 124608;
	perftTests[123].perfts[4] = 2193768;
	perftTests[123].perfts[5] = 37665329;

	perftTests[124].FEN = "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1";
	perftTests[124].perfts[0] = 18;
	perftTests[124].perfts[1] = 270;
	perftTests[124].perfts[2] = 4699;
	perftTests[124].perfts[3] = 79355;
	perftTests[124].perfts[4] = 1533145;
	perftTests[124].perfts[5] = 28859283;

	perftTests[125].FEN = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1";
	perftTests[125].perfts[0] = 24;
	perftTests[125].perfts[1] = 496;
	perftTests[125].perfts[2] = 9483;
	perftTests[125].perfts[3] = 182838;
	perftTests[125].perfts[4] = 3605103;
	perftTests[125].perfts[5] = 71179139;
	
	perftTests[126].FEN = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
	perftTests[126].perfts[0] = 14;
	perftTests[126].perfts[1] = 191;
	perftTests[126].perfts[2] = 2812;
	perftTests[126].perfts[3] = 43238;
	perftTests[126].perfts[4] = 674624;
	perftTests[126].perfts[5] = 11030083;

	perftTests[127].FEN = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	perftTests[127].perfts[0] = 6;
	perftTests[127].perfts[1] = 264;
	perftTests[127].perfts[2] = 9467;
	perftTests[127].perfts[3] = 422333;
	perftTests[127].perfts[4] = 15833292;
	perftTests[127].perfts[5] = 706045033;
}

#endif