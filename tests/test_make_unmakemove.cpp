#include <algorithm>

#include "catch.hpp"

#include "FEN.h"
#include "MoveGenerator.h"

TEST_CASE("make unmake", "make unmake move") {
  std::string pos = "[FEN \"B:W13,17,18,19,22,24,25,26,27,32:B1,6,8,9,10,12,15,16,20\"]";
  FEN fen(pos);

  std::vector<Move> etalon_moves{
      {{25, 33, 32, 23, 24, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 21, 29, true},

      {{25, 24, 23, 32, 33, 0, 0, 0, 0, 0, 0, 0},
       {kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker, kWhiteChecker,
        kOff, kOff, kOff, kOff, kOff, kOff, kOff}, 21, 29, true}
  };
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  MoveGenerator generator(fen);
  auto trueMailbox = generator.getMailbox();
  auto trueIsWhiteMove = generator.getIsWhiteMove();

  auto gen_moves = generator.getMoves();
  std::sort(std::begin(etalon_moves), std::end(etalon_moves));

  REQUIRE(gen_moves.size() == etalon_moves.size());
  REQUIRE(gen_moves == etalon_moves);

  for (auto &move : etalon_moves) {
    generator.makeMove(move);
    generator.unmakeMove(move);

    REQUIRE(trueIsWhiteMove == generator.getIsWhiteMove());
    REQUIRE(trueMailbox == generator.getMailbox());
  }
}
