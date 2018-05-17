#include <string>
#include <vector>

#include "FEN.h"
#include "types.h"

#include "catch.hpp"

/*
    g8  e8  c8  a8  1-4
  h7  f7  d7  b7    5-8
    g6  e6  c6  a6  9-12
  h5  f5  d5  b5    13-16
    g4  e4  c4  a4  17-20
  h3  f3  d3  b3    21-24
    g2  e2  c2  a2  25-28
  h1  f1  d1  b1    29-32


mailbox
    00  01  02  03

04    05  06  07  08
    09  10  11  12
13    14  15  16  17
    18  19  20  21    22
      23  24  25  26
    27  28  29  30
31    32  33  34  35
    36  37  38  39     40

  41  42  43  44


 [FEN "W:WK18,21,22,23,24,25,26,27,29,30,31,32:B1,2,3,5,K6,7,8,9,10,11,12,28"]

  fen                  mailbox
    01  02  03  04       05  06  07  08
  05  06  07  08       09  10  11  12
    09  10  11  12       14  15  16  17
  13  14  15  16       18  19  20  21
    17  18  19  20       23  24  25  26
  21  22  23  24       27  28  29  30
    25  26  27  28       32  33  34  35
  29  30  31  32       36  37  38  39

*/

const u_int x = kOff;
const u_int o = kEmptyCell;
const u_int w = kWhiteChecker;
const u_int b = kBlackChecker;
const u_int W = kWhiteDamka;
const u_int B = kBlackDamka;

TEST_CASE("start position", "fen") {
  FEN fen("[FEN \"W:W21,22,23,24,25,26,27,28,29,30,31,32:B1,2,3,4,5,6,7,8,9,10,11,12\"]");

  std::vector<u_int> etalon_mailbox{
            x, x, x, x,

      x,      b, b, b, b,
            b, b, b, b,
      x,      b, b, b, b,
            o, o, o, o,     x,
              o, o, o, o,
            w, w, w, w,
      x,      w, w, w, w,
            w, w, w, w,     x,

           x, x, x, x,
  };

  REQUIRE(fen.isWhiteMove());
  REQUIRE(etalon_mailbox.size() == fen.getMailBox().size());
  REQUIRE(etalon_mailbox == fen.getMailBox());
}

TEST_CASE("FEN W:W29:B25", "fen") {
  FEN fen("[FEN \"W:W29:B25\"]");

  std::vector<u_int> etalon_mailbox{
            x, x, x, x,

      x,      o, o, o, o,
            o, o, o, o,
      x,      o, o, o, o,
            o, o, o, o,     x,
              o, o, o, o,
            o, o, o, o,
      x,      b, o, o, o,
            w, o, o, o,     x,

           x, x, x, x,
  };

  REQUIRE(fen.isWhiteMove());
  REQUIRE(etalon_mailbox.size() == fen.getMailBox().size());
  REQUIRE(etalon_mailbox == fen.getMailBox());
}

TEST_CASE("FEN W:W32", "fen") {
  FEN fen("[FEN \"W:W32\"]");

  std::vector<u_int> etalon_mailbox{
            x, x, x, x,

      x,      o, o, o, o,
            o, o, o, o,
      x,      o, o, o, o,
            o, o, o, o,     x,
              o, o, o, o,
            o, o, o, o,
      x,      o, o, o, o,
            o, o, o, w,     x,

           x, x, x, x,
  };

  REQUIRE(fen.isWhiteMove());
  REQUIRE(etalon_mailbox.size() == fen.getMailBox().size());
  REQUIRE(etalon_mailbox == fen.getMailBox());
}

TEST_CASE("FEN W:WK29,K32,K28:BK2,3", "fen") {
  FEN fen("[FEN \"W:WK29,K32,K28:BK2,3\"]");

  std::vector<u_int> etalon_mailbox{
             x, x, x, x,

      x,      o, B, b, o,
             o, o, o, o,
      x,      o, o, o, o,
             o, o, o, o,     x,
               o, o, o, o,
             o, o, o, o,
      x,      o, o, o, W,
             W, o, o, W,     x,

            x, x, x, x,
  };

  REQUIRE(fen.isWhiteMove());
  REQUIRE(etalon_mailbox.size() == fen.getMailBox().size());
  REQUIRE(etalon_mailbox == fen.getMailBox());
}
