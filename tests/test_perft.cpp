#include <set>
#include <vector>

#include "catch.hpp"

#include "FEN.h"
#include "MoveGenerator.h"

/*
http://pdn.fmjd.org/index.html
http://kvetka.org/UCI_format.shtml

Perft for russian draughts:
1 -  7
2 -  49
3 -  302
4 -  1469
5 -  7482
6 -  37986
7 -  190146
8 -  929905
9 -  4570667
10 - 22450628
11 - 110961169
12 - 545059387
13 - 2675994747
14 - 13138899366

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

start pos
[FEN "W:W21,22,23,24,25,26,27,28,29,30,31,32:B1,2,3,4,5,6,7,8,9,10,11,12"]

 */

static void removeDuplicate(std::vector<Move> &moves, MoveGenerator &generator) {
  if (moves.size() < 2) { return; }
  if (moves[0].cap_sq[0] == 0) { return; }

  // удаление дубликатов только при множественных взятиях, больше одной шашки
  size_t i = 0;
  for (auto &move : moves) { if (move.cap_sq[1] == 0) { ++i; }}
  if (i == moves.size()) { return; }

  std::sort(moves.begin(), moves.end());
  moves.erase(std::unique(moves.begin(), moves.end()), moves.end());

  MoveGenerator g;
  g.setIsWhiteMove(generator.getIsWhiteMove());
  g.setMailbox(generator.getMailbox());

  std::set<std::vector<uint64_t>> uniqueMaibox;
  std::vector<Move> uniqueMoves;
  for (auto &move : moves) {
    g.makeMove(move);
    if (uniqueMaibox.insert(g.getMailbox()).second) { uniqueMoves.push_back(move); }
    g.unmakeMove(move);
  }

  moves = uniqueMoves;
}

static size_t perftKallisto(size_t depth, MoveGenerator &generator) {
  auto moves = generator.getMoves();
  --depth;
  size_t countMoves = 0;
  for (auto &move : moves) {
    generator.makeMove(move);
    countMoves += (depth ? perftKallisto(depth, generator) : 1);
    generator.unmakeMove(move);
  }
  return countMoves;
}

static size_t perftAvrora(size_t depth, MoveGenerator &generator) {
  auto moves = generator.getMoves();
  removeDuplicate(moves, generator);
  --depth;
  size_t countMoves = 0;
  for (auto &move : moves) {
    generator.makeMove(move);
    countMoves += (depth ? perftAvrora(depth, generator) : 1);
    generator.unmakeMove(move);
  }
  return countMoves;
}

const char *const startPos = "[FEN \"W:W21,22,23,24,25,26,27,28,29,30,31,32:B1,2,3,4,5,6,7,8,9,10,11,12\"]";
FEN fen(startPos);

TEST_CASE("Perft Kallisto", "perft") {
  const size_t perftNodesKallisto = 14;
  size_t depthsKallisto[perftNodesKallisto] =
      {7, 49, 302, 1469, 7482, 37986, 190146, 929905, 4570667,
       22450628, 110961169, 545059387, 2675994747, 13138899366};

/*  for (size_t i = 0; i < 14; ++i) {
    MoveGenerator generator(fen);
    size_t countMoves = perftKallisto(i + 1, generator);
    REQUIRE(countMoves == depthsKallisto[i]);
  }*/

  const size_t depth = 11;
  MoveGenerator generator(fen);
  size_t countMoves = perftKallisto(depth, generator);
  REQUIRE(countMoves == depthsKallisto[depth - 1]);
}

TEST_CASE("Perft Avrora", "perft") {
  const size_t perftNodesAvrora = 16;
  size_t depthsAvrora[perftNodesAvrora] =
      {7, 49, 302, 1469, 7482, 37986, 190146, 929899,
       4570586, 22444032, 110917189, 544770444, 2673979569,
       13124294848, 64710633296, 321172593712};

/*
  for (size_t i = 0; i < 16; ++i) {
    MoveGenerator generator(fen);
    size_t countMoves = perftAvrora(i + 1, generator);
    REQUIRE(countMoves == depthsAvrora[i]);
  }
*/

  const size_t depth = 1;
  MoveGenerator generator(fen);
  size_t countMoves = perftAvrora(depth, generator);
  REQUIRE(countMoves == depthsAvrora[depth - 1]);
}
