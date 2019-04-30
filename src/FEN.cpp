#include "FEN.h"

#include <sstream>


namespace {
    void sideFenToMailbox(const std::string &side, std::vector<int64_t> &mailbox) {
        if (side.empty()) { return; }

        const int64_t fen_to_mailbox[32]{
                5, 6, 7, 8,
                9, 10, 11, 12,
                14, 15, 16, 17,
                18, 19, 20, 21,
                23, 24, 25, 26,
                27, 28, 29, 30,
                32, 33, 34, 35,
                36, 37, 38, 39
        };

        std::string line;
        std::stringstream fenStream(side.substr(1));
        while (fenStream >> line) {
            const bool isDamka = line.at(0) == 'K';
            const int64_t fenIndex = isDamka ? std::stoi(line.substr(1)) : std::stoi(line);
            const int64_t mailBoxIndex = fen_to_mailbox[fenIndex - 1];
            const bool isWhite = side.at(0) == 'W';
            if (isDamka) {
                mailbox[mailBoxIndex] = isWhite ? kWhiteDamka : kBlackDamka;
            } else {
                mailbox[mailBoxIndex] = isWhite ? kWhiteChecker : kBlackChecker;
            }
        }
    }
} // zero namespace

FEN::FEN(const std::string &fenStr) : fenStr_(fenStr) {

}

bool FEN::isWhiteMove() const {
  // [FEN "W:WK18,21,22,23,24,25,26,27,29,30,31,32:B1,2,3,5,K6,7,8,9,10,11,12,28"]
  return fenStr_.at(6) == 'W';
}

std::vector<int64_t> FEN::getMailBox() const {
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

      41  42  43   44


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

  std::string tmp(fenStr_.begin() + 8, fenStr_.end() - 2);
  for (auto &symbol : tmp) { if (symbol == ',') { symbol = ' '; }}

  size_t posColon = tmp.find(':');

  // [FEN "W:WK18,21,22,23,24,25,26,27,29,30,31,32:B1,2,3,5,K6,7,8,9,10,11,12,28"]
  std::string leftSideColon; // WK18,21,22,23,24,25,26,27,29,30,31,32
  std::string rightSideColon; // B1,2,3,5,K6,7,8,9,10,11,12,28

  if (posColon != std::string::npos) {
    leftSideColon = std::string(tmp.begin(), tmp.begin() + posColon);
    rightSideColon = std::string(tmp.begin() + posColon + 1u, tmp.end());
  } else {
    leftSideColon = tmp;
  }

  std::vector<int64_t> mailbox(45, kEmptyCell);
  mailbox[0] = mailbox[1] = mailbox[2] = mailbox[3] = mailbox[4] = kOff;
  mailbox[13] = kOff;
  mailbox[22] = kOff;
  mailbox[31] = kOff;
  mailbox[40] = mailbox[41] = mailbox[42] = mailbox[43] = mailbox[44] = kOff;

  sideFenToMailbox(leftSideColon, mailbox);
  sideFenToMailbox(rightSideColon, mailbox);

  return mailbox;
}

std::string mailbox2fenStr(const std::vector<int64_t> &mailbox, bool isWhiteMove) {
  std::string leftSideColon = isWhiteMove ? "[FEN \"W:W" : "[FEN \"B:W";
  std::string rightSideColon = ":B";

  int64_t j = 1;
  for (int64_t i = 5; i < 40; ++i) {
    if (mailbox[i] == kOff) { continue; }
    if (mailbox[i] == kBlackChecker) { rightSideColon += std::to_string(j) + ","; }
    else if (mailbox[i] == kBlackDamka) { rightSideColon += "K" + std::to_string(j) + ","; }
    else if (mailbox[i] == kWhiteChecker) { leftSideColon += std::to_string(j) + ","; }
    else if (mailbox[i] == kWhiteDamka) { leftSideColon += "K" + std::to_string(j) + ","; }
    else {}
    ++j;
  }

  if (leftSideColon.back() == ',') { leftSideColon.pop_back(); }
  if (rightSideColon.back() == ',') { rightSideColon.pop_back(); }
  return leftSideColon + rightSideColon + "\"]";
}
