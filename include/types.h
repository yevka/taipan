#ifndef TAIPAN_TYPES_H
#define TAIPAN_TYPES_H

#include <stdint.h>

const uint8_t kWhite = 1;
const uint8_t kBlack = 2;
const uint8_t kKing = 4;
const uint8_t kOff = 240;

const uint8_t kWhiteChecker = kWhite;
const uint8_t kWhiteDamka = kWhite | kKing;

const uint8_t kBlackChecker = kBlack;
const uint8_t kBlackDamka = kBlack | kKing;

const uint8_t kEmptyCell = 0;

const uint8_t kChangeColor = 3;

#endif //TAIPAN_TYPES_H
