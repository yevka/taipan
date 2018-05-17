#ifndef TAIPAN_TYPES_H
#define TAIPAN_TYPES_H

typedef unsigned int u_int;

const u_int kWhite = 1;
const u_int kBlack = 2;
const u_int kKing = 4;
const u_int kOff = 240;

const u_int kWhiteChecker = kWhite;
const u_int kWhiteDamka = kWhite | kKing;

const u_int kBlackChecker = kBlack;
const u_int kBlackDamka = kBlack | kKing;

const u_int kEmptyCell = 0;

const u_int kChangeColor = 3;

#endif //TAIPAN_TYPES_H
