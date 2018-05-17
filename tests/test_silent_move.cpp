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

[FEN "W:W18,21,22,23,24,25,26,27,29,30,31,32:B1,2,3,5,6,7,8,9,10,11,12,28"]
*/

TEST_CASE("FEN B:WK39,33,23:B34 transformation into a king", "silent move") {
  std::string startPos = "[FEN \"B:WK39,33,23:B34\"]";
  FEN fen(startPos);
  MoveGenerator generator(fen);

  std::vector<Move> etalon_moves{
      {34, 38, true, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}}
  };

  auto moves = generator.getMoves();
  REQUIRE(moves.size() == etalon_moves.size());
  REQUIRE(moves == etalon_moves);
}
