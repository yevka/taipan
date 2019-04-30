#ifndef TAIPAN_MOVEGENERATOR_H
#define TAIPAN_MOVEGENERATOR_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

#include "types.h"

struct Move {
  int64_t cap_sq[12];
  int64_t cap_type[12];
  int64_t from;
  int64_t to;
  bool promotion;
};

inline bool operator==(const Move &left, const Move &right) {
  if (left.promotion == right.promotion && left.from == right.from && left.to == right.to) {
    if (left.cap_sq[0] == 0 || right.cap_sq[0] == 0) { return false; }
    std::array<int64_t, 12> tmpLeftCapSq;
    std::partial_sort_copy(left.cap_sq, left.cap_sq + 12, tmpLeftCapSq.begin(), tmpLeftCapSq.end());

    std::array<int64_t, 12> tmpRightCapSq;
    std::partial_sort_copy(right.cap_sq, right.cap_sq + 12, tmpRightCapSq.begin(), tmpRightCapSq.end());

    std::array<int64_t, 12> tmpLeftCapType;
    std::partial_sort_copy(left.cap_type, left.cap_type + 12, tmpLeftCapType.begin(), tmpLeftCapType.end());

    std::array<int64_t, 12> tmpRightCapType;
    std::partial_sort_copy(right.cap_type, right.cap_type + 12, tmpRightCapType.begin(), tmpRightCapType.end());

    return std::equal(tmpLeftCapSq.begin(), tmpLeftCapSq.end(), tmpRightCapSq.begin())
        && std::equal(tmpLeftCapType.begin(), tmpLeftCapType.end(), tmpRightCapType.begin());
  }
  return false;
}

inline bool operator<(const Move &left, const Move &right) {
  return left.to < right.to;
}

inline std::ostream &operator<<(std::ostream &out, const Move &move) {
  out << move.from << " : " << move.to << " : " << move.promotion << "\n";
  for (size_t i = 0; i < 12 && move.cap_sq[i]; ++i) { out << move.cap_sq[i] << ":" << move.cap_type[i] << "   "; }
  out << "\n\n";
  return out;
}

class FEN;

class MoveGenerator {
 public:
  MoveGenerator();
  explicit MoveGenerator(const FEN &pos);

  void setPos(const FEN &pos);

  void makeMove(const Move &move);

  void unmakeMove(const Move &move);

  void setMailbox(const std::vector<int64_t> &mailbox) {
    for (int64_t i = 0; i < 45; ++i) { mailbox_[i] = mailbox[i]; }
  }

  std::vector<int64_t> getMailbox() const {
    return std::vector<int64_t>(mailbox_, mailbox_ + 45);
  }

  void setIsWhiteMove(const bool isWhiteMove) {
    isWhiteMove_ = isWhiteMove;
  }

  bool getIsWhiteMove() const {
    return isWhiteMove_;
  }

  std::vector<Move> getMoves() {
    moves_ = {};
    this->generateAllMoves();
    return moves_;
  }

  std::vector<std::string> moves2str(const std::vector<Move> &moves) {
    /*
        board                      mailbox
           b8  d8  f8  h8  5-8        05  06  07  08
         a7  c7  e7  g7    9-12     09  10  11  12
           b6  d6  f6  h6  14-17      14  15  16  17
         a5  c5  e5  g5    18-21    18  19  20  21
           b4  d4  f4  h4  23-26      23  24  25  26
         a3  c3  e3  g3    27-30    27  28  29  30
           b2  d2  f2  h2  32-35      32  33  34  35
         a1  c1  e1  g1    36-39    36  37  38  39

          a b c d e f g h
    */

    const char *cells[32] = {
        "b8", "d8", "f8", "h8",
        "a7", "c7", "e7", "g7",
        "b6", "d6", "f6", "h6",
        "a5", "c5", "e5", "g5",
        "b4", "d4", "f4", "h4",
        "a3", "c3", "e3", "g3",
        "b2", "d2", "f2", "h2",
        "a1", "c1", "e1", "g1"
    };

    int64_t mailbox[32] = {
        5, 6, 7, 8,
        9, 10, 11, 12,
        14, 15, 16, 17,
        18, 19, 20, 21,
        23, 24, 25, 26,
        27, 28, 29, 30,
        32, 33, 34, 35,
        36, 37, 38, 39
    };

    std::vector<std::string> str_moves;
    for (const Move& move : moves) {
      std::string from = cells[mailbox[move.from]];
      std::string to = cells[mailbox[move.to]];
      std::string str_move = from;
      str_move += "-";
      str_move += to;
      str_moves.push_back(str_move);
    }

    return str_moves;
  }

 private:

  bool isCapture(const int64_t sq) const {
    int64_t stm = isWhiteMove_ ? 1 : 2;
    return bool(mailbox_[sq] & (stm ^ kChangeColor));
  }

  void changeColor(const int64_t sq) {
    mailbox_[sq] ^= kChangeColor;
  }

  void addCaptured(int64_t sq, size_t caps, Move &m) {
    m.cap_sq[caps] = sq;
    m.cap_type[caps] = mailbox_[sq];
  }

  void addKingMoves(int64_t sq, int64_t dir) {
    for (int64_t to = sq + dir; !mailbox_[to]; to += dir) {
      moves_.emplace_back(Move {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff},
                                sq, to, false});
    }
  }

  void addManMove(int64_t sq, int64_t dir) {
    int64_t to = sq + dir;
    if (!mailbox_[to]) {
      moves_.emplace_back(Move {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, sq, to,
                                (to < 9 || to > 35)});
    }
  }
  void generateSilentMoves();

  Move copyMoveFromTemplate(size_t caps, Move &t);

  void kingCapture(int64_t sq, size_t caps, int64_t dir, int64_t bad_dir, Move &t, bool &found);

  void tryKingCapture(int64_t sq, int64_t dir);

  void addKingCaptures(int64_t sq, size_t caps, int64_t dir, Move &t);

  void addPromoCaptures(int64_t sq, size_t caps, int64_t dir, Move &t);

  void manCapture(int64_t sq, size_t caps, int64_t dir, int64_t bad_dir, Move &t,
                  bool &found);

  void addManCaptures(int64_t sq, size_t caps, int64_t bad_dir, Move &t);

  void tryManCapture(int64_t sq, int64_t dir);

  void generateCaptures();

  void generateAllMoves();

 private:
  int64_t mailbox_[45];              // 180 bytes
  int64_t onlyFirstMoveCheckers[13]; // 52 bytes
  std::vector<Move> moves_;        // 24 bytes
  bool isWhiteMove_;               // 1 bytes
};

#endif //TAIPAN_MOVEGENERATOR_H
