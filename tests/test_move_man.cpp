#include <algorithm>
#include <vector>

#include "catch.hpp"

#include "FEN.h"
#include "MoveGenerator.h"

/*
http://pdn.fmjd.org/index.html
http://kvetka.org/UCI_format.shtml

full mailbox
                     00  01  02  03

               04      05  06  07  08
                     09  10  11  12
               13      14  15  16  17
                     18  19  20  21      22
                       23  24  25  26
                     27  28  29  30
               31      32  33  34  35
                     36  37  38  39

                   40  41  42  43  44

fen                  mailbox                real board
  01  02  03  04       05  06  07  08         b8  d8  f8  h8    1-4
05  06  07  08       09  10  11  12         a7  c7  e7  g7      5-8
  09  10  11  12       14  15  16  17         b6  d6  f6  h6    9-12
13  14  15  16       18  19  20  21         a5  c5  e5  g5      13-16
  17  18  19  20       23  24  25  26         b4  d4  f4  g4    17-20
21  22  23  24       27  28  29  30         a3  c3  e3  g3      21-24
  25  26  27  28       32  33  34  35         b2  d2  f2  h2    25-28
29  30  31  32       36  37  38  39         a1  c1  e1  g1      29-32

*/

static void printMailbox(const std::vector<char> &mailbox) {
  auto box = mailbox;
  box.erase(
      std::remove_if(box.begin(), box.end(),
                     [&](char s) { return s == 'x'; }),
      box.end());

  int j = 1;
  bool k = true;
  std::cout << " ";
  for (size_t i = 0; i < box.size(); ++i) {
    if (j++ == 4) {
      j = 1;
      if (k) {
        std::cout << box[i] << "\n";
        k = false;
      } else {
        std::cout << box[i] << "\n ";
        k = true;
      }
    } else {
      std::cout << box[i] << " ";
    }
  }

  std::cout << "\n";
}

TEST_CASE("FEN W:W29:B25 => h1xf3", "just take") {
  std::vector<Move> etalon_moves{
      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 28, false}
  };

  FEN fen("[FEN \"W:W29:B25\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("FEN W:W29:B25,18 => h1xd5<g2,e4>", "just take") {
  std::vector<Move> etalon_moves{
      {{32, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 20, false}
  };

  FEN fen("[FEN \"W:W29:B25,18\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("FEN B:W14,15,22,23:B18", "just take") {
  std::vector<Move> etalon_moves{
      {{28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 24, 32, false},

      {{20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 24, 16, false},

      {{19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 24, 14, false},

      {{29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 24, 34, false}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"B:W14,15,22,23:B18\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("FEN B:B13,17,18,19,22,24,25,26,27,32:B1,6,8,9,10,12,15,16,20", "just take") {
  std::vector<Move> etalon_moves{
      {{25, 33, 32, 23, 24, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 21, 29, true},

      {{25, 24, 23, 32, 33, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 21, 29, true}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"B:W13,17,18,19,22,24,25,26,27,32:B1,6,8,9,10,12,15,16,20\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("[FEN \"W:W14,15,22:B2,10\"]", "just strike") {
  std::vector<Move> etalon_moves{
      {{15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 20, 10, false},

      {{15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 19, 11, false}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"W:W14,15,22:B2,10\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("[FEN \"B:W19,23,26,18,24:B6,7,8,11,K20\"]", "turkish strike") {
  std::vector<Move> etalon_moves{
      {{30, 33, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteChecker, kWhiteChecker, kOff, kOff,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 26, 20, false}
  };

  FEN fen("[FEN \"B:W19,23,26,18,24:B6,7,8,11,K20\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("[FEN \"W:W31:B26,27,18,19\"]", "turkish strike") {
  std::vector<Move> etalon_moves{
      {{33, 24, 25, 34, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kOff,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 38, 38, false},

      {{34, 25, 24, 33, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kOff,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 38, 38, false}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"W:W31:B26,27,18,19\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("FEN W:WK29:B25", "king strike") {
  std::vector<Move> etalon_moves{
      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 28, false},

      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 24, false},

      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 20, false},

      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 16, false},

      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 12, false},

      {{32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 36, 8, false}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"W:WK29:B25\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("[FEN B:W6,K7,K14,15:BK2]", "strike in a cirkle by black king") {
  std::vector<Move> etalon_moves{
      {{11, 20, 19, 10, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteDamka, kWhiteChecker, kWhiteDamka, kWhiteChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 6, 6,
       false},

      {{10, 19, 20, 11, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteDamka, kWhiteChecker, kWhiteDamka, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 6, 6,
       false,},
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"B:W6,K7,K14,15:BK2\"]");
  MoveGenerator generator(fen);

  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("[FEN B:W6,K7,K14,15:B2]", "strike in a cirkle by black checker") {
  std::vector<Move> etalon_moves{
      {{11, 20, 19, 10, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteDamka, kWhiteChecker, kWhiteDamka, kWhiteChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 6, 6,
       false},

      {{10, 19, 20, 11, 0, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteDamka, kWhiteChecker, kWhiteDamka, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 6, 6,
       false},
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"B:W6,K7,K14,15:B2\"]");
  MoveGenerator generator(fen);

  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}

TEST_CASE("[FEN \"W::W28:B7,8,16,24,23,17,25\"]", "strike in a cirkle by white checker") {
  std::vector<Move> etalon_moves{
      // h2xf4
      {{30, 21, 12, 11, 23, 32, 29, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kOff,
        kOff, kOff, kOff, kOff}, 35, 25, true},

      // h2xf2
      {{30, 21, 12, 11, 29, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff,
        kOff}, 35, 34, true},

      // h2xg1
      {{30, 21, 12, 11, 29, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff,
        kOff}, 35, 39, true},

      // h2xd2
      {{30, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kBlackChecker, kBlackChecker, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 35, 33, false}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  FEN fen("[FEN \"W:W28:B7,8,16,24,23,17,25\"]");
  MoveGenerator generator(fen);
  auto gen_moves = generator.getMoves();
  std::sort(std::begin(gen_moves), std::end(gen_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);
}
