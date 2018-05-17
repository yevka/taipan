#include "MoveGenerator.h"
#include "FEN.h"

static const int TOP_RIGHT = -4;
static const int TOP_LEFT = -5;
static const int BOTTOM_RIGHT = 5;
static const int BOTTOM_LEFT = 4;

MoveGenerator::MoveGenerator() : mailbox_{}, onlyFirstMoveCheckers{}, moves_{}, isWhiteMove_(true) {
  moves_.reserve(64);
}

MoveGenerator::MoveGenerator(const FEN &pos) : MoveGenerator() {
  setPos(pos);
}

void MoveGenerator::setPos(const FEN &pos) {
  isWhiteMove_ = pos.isWhiteMove();
  auto posMaibox = pos.getMailBox();
  for (int i = 0; i < 45; ++i) { mailbox_[i] = posMaibox[i]; }
}

void MoveGenerator::makeMove(const Move &move) {
  isWhiteMove_ = !isWhiteMove_;

  u_int checker = move.promotion ? mailbox_[move.from] | kKing : mailbox_[move.from];
  mailbox_[move.from] = kEmptyCell;
  mailbox_[move.to] = checker;

  for (size_t i = 0; i < 12 && move.cap_sq[i] != 0; ++i) {
    mailbox_[move.cap_sq[i]] = kEmptyCell;
  }
}

void MoveGenerator::unmakeMove(const Move &move) {
  isWhiteMove_ = !isWhiteMove_;

  u_int checker = move.promotion ? isWhiteMove_ ? kWhiteChecker : kBlackChecker : mailbox_[move.to];
  mailbox_[move.to] = kEmptyCell;
  mailbox_[move.from] = checker;

  for (size_t i = 0; i < 12 && move.cap_sq[i] != 0; ++i) {
    mailbox_[move.cap_sq[i]] = move.cap_type[i];
  }
}

void MoveGenerator::generateSilentMoves() {
  u_int sq = 0;
  for (int j = 0; j < 12 && onlyFirstMoveCheckers[j]; ++j) {
    sq = onlyFirstMoveCheckers[j];
    if (!(mailbox_[sq] & kKing)) {
      if (isWhiteMove_) {
        addManMove(sq, TOP_RIGHT);
        addManMove(sq, TOP_LEFT);
      } else {
        addManMove(sq, BOTTOM_LEFT);
        addManMove(sq, BOTTOM_RIGHT);
      }
    } else {
      addKingMoves(sq, TOP_RIGHT);
      addKingMoves(sq, TOP_LEFT);
      addKingMoves(sq, BOTTOM_LEFT);
      addKingMoves(sq, BOTTOM_RIGHT);
    }
  }
}

Move MoveGenerator::copyMoveFromTemplate(const size_t caps, Move &t) {
  Move newMove
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff},
       t.from, 0, t.promotion};

  for (size_t i = 0; i < caps; ++i) {
    newMove.cap_sq[i] = t.cap_sq[i];
    newMove.cap_type[i] = t.cap_type[i];
  }
  return newMove;
}

void MoveGenerator::kingCapture(const u_int sq,
                                const size_t caps,
                                const int dir,
                                const int bad_dir,
                                Move &t,
                                bool &found) {
  if (dir == bad_dir) { return; }
  u_int m = sq + dir;
  while (!mailbox_[m]) { m += dir; }
  if (!isCapture(m)) { return; }

  u_int to = m + dir;
  if (mailbox_[to]) { return; }

  addCaptured(m, caps, t);
  changeColor(m);
  addKingCaptures(to, caps + 1, dir, t);
  changeColor(m);
  found = true;
}

void MoveGenerator::tryKingCapture(const u_int sq, const int dir) {
  u_int m = sq + dir;
  while (!mailbox_[m]) { m += dir; }
  if (!isCapture(m)) { return; }

  u_int to = m + dir;
  if (mailbox_[to]) { return; }

  u_int saveCell = mailbox_[sq];
  mailbox_[sq] = kEmptyCell;

  Move t{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, sq, 0, false};
  addCaptured(m, 0, t);

  changeColor(m);
  addKingCaptures(to, 1, dir, t);
  changeColor(m);
  mailbox_[sq] = saveCell;
}

void MoveGenerator::addKingCaptures(const u_int sq, const size_t caps, const int dir, Move &t) {
  bool found = false;
  u_int m = sq;
  while (!mailbox_[m]) {
    kingCapture(m, caps, TOP_RIGHT, dir, t, found);
    kingCapture(m, caps, TOP_LEFT, dir, t, found);
    kingCapture(m, caps, BOTTOM_LEFT, dir, t, found);
    kingCapture(m, caps, BOTTOM_RIGHT, dir, t, found);

    m += dir;
  }

  u_int to = m + dir;
  if (isCapture(m) && (!mailbox_[to])) {
    addCaptured(m, caps, t);
    changeColor(m);
    addKingCaptures(to, caps + 1, dir, t);
    changeColor(m);
    return;
  }

  if (!found) {
    for (u_int isq = sq; mailbox_[isq] == 0; isq += dir) {
      auto newMove = copyMoveFromTemplate(caps, t);
      newMove.to = isq;
      moves_.push_back(newMove);
    }
  }
}

void MoveGenerator::addPromoCaptures(const u_int sq, const size_t caps, const int dir, Move &t) {
  int newDir{};
  if (dir == TOP_RIGHT) { newDir = BOTTOM_RIGHT; }
  else if (dir == TOP_LEFT) { newDir = BOTTOM_LEFT; }
  else if (dir == BOTTOM_LEFT) { newDir = TOP_LEFT; }
  else { newDir = TOP_RIGHT; }

  u_int m = sq + newDir;
  while (!mailbox_[m]) { m += newDir; }

  u_int to = m + newDir;
  if (isCapture(m) && !mailbox_[to]) {
    addCaptured(m, caps, t);
    changeColor(m);
    addKingCaptures(to, caps + 1, newDir, t);
    changeColor(m);
    return;
  }

  auto newMove = copyMoveFromTemplate(caps, t);
  newMove.to = sq;
  moves_.push_back(newMove);
}

void MoveGenerator::manCapture(const u_int sq,
                               const size_t caps,
                               const int dir,
                               const int bad_dir,
                               Move &t,
                               bool &found) {
  if (dir == bad_dir) { return; }

  u_int m = sq + dir;
  if (!isCapture(m)) { return; }

  u_int to = m + dir;
  if (mailbox_[to]) { return; }

  addCaptured(m, caps, t);
  changeColor(m);

  const bool promotionCell = ((to < 9 && isWhiteMove_) || (to > 35 && !isWhiteMove_));
  if (!promotionCell) {
    t.promotion = false;
    addManCaptures(to, caps + 1, -dir, t);
  } else {
    t.promotion = true;
    addPromoCaptures(to, caps + 1, dir, t);
  }

  changeColor(m);
  found = true;
}

void MoveGenerator::addManCaptures(const u_int sq, const size_t caps, const int bad_dir, Move &t) {
  bool found = false;
  manCapture(sq, caps, TOP_RIGHT, bad_dir, t, found);
  manCapture(sq, caps, TOP_LEFT, bad_dir, t, found);
  manCapture(sq, caps, BOTTOM_LEFT, bad_dir, t, found);
  manCapture(sq, caps, BOTTOM_RIGHT, bad_dir, t, found);

  if (!found) {
    auto newMove = copyMoveFromTemplate(caps, t);
    newMove.to = sq;
    moves_.push_back(newMove);
  }
}

void MoveGenerator::tryManCapture(const u_int sq, const int dir) {
  u_int m = sq + dir;
  if (!isCapture(m)) { return; }

  u_int to = m + dir;
  if (mailbox_[to]) { return; }

  u_int saveCell = mailbox_[sq];
  mailbox_[sq] = kEmptyCell;

  Move t{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff, kOff}, sq, 0, false};
  addCaptured(m, 0, t);

  changeColor(m);

  const bool promotionCell = (to < 9 && isWhiteMove_) || (to > 35 && !isWhiteMove_);
  if (!promotionCell) {
    t.promotion = false;
    addManCaptures(to, 1, -dir, t);
  } else {
    t.promotion = true;
    addPromoCaptures(to, 1, dir, t);
  }

  changeColor(m);
  mailbox_[sq] = saveCell;
}

void MoveGenerator::generateCaptures() {
  u_int sq = 0;
  for (u_int j = 0; j < 12 && onlyFirstMoveCheckers[j]; ++j) {
    sq = onlyFirstMoveCheckers[j];
    if (!(mailbox_[sq] & kKing)) {
      tryManCapture(sq, TOP_RIGHT);
      tryManCapture(sq, TOP_LEFT);
      tryManCapture(sq, BOTTOM_LEFT);
      tryManCapture(sq, BOTTOM_RIGHT);
    } else {
      tryKingCapture(sq, TOP_RIGHT);
      tryKingCapture(sq, TOP_LEFT);
      tryKingCapture(sq, BOTTOM_LEFT);
      tryKingCapture(sq, BOTTOM_RIGHT);
    }
  }
}

void MoveGenerator::generateAllMoves() {
  auto foundChecker = isWhiteMove_ ? kWhiteChecker : kBlackChecker;
  auto foundDamka = isWhiteMove_ ? kWhiteDamka : kBlackDamka;
  u_int i = 0;
  for (u_int sq = 5; sq < 40; ++sq) {
    if (mailbox_[sq] == foundChecker || mailbox_[sq] == foundDamka) { onlyFirstMoveCheckers[i++] = sq; }
  }
  onlyFirstMoveCheckers[i] = 0;

  this->generateCaptures();
  if (moves_.empty()) { this->generateSilentMoves(); }
}
