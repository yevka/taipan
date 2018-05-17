#ifndef TAIPAN_FEN_H
#define TAIPAN_FEN_H

#include <vector>
#include <string>

#include "types.h"

class FEN {
 public:
  FEN() = default;
  explicit FEN(const std::string &fenStr);

  bool isWhiteMove() const;

  std::vector<u_int> getMailBox() const;

 private:
  std::string fenStr_;
};

std::string mailbox2fenStr(const std::vector<u_int> &mailbox, const bool isWhiteMove);

#endif //TAIPAN_FEN_H
