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

  std::vector<int64_t> getMailBox() const;

 private:
  std::string fenStr_;
};

std::string mailbox2fenStr(const std::vector<int64_t> &mailbox, bool isWhiteMove);

#endif //TAIPAN_FEN_H
