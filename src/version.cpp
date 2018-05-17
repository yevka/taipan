#include <iostream>
#include <sstream>

#include "version.h"

std::string engine_info() {
  const std::string months("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
  std::string month, day, year;
  std::stringstream ss, date(__DATE__); // From compiler, format is "Nov 22 2016"

  ss << "taipan version "
     << TAIPAN_MAJOR_VERSION << "."
     << TAIPAN_MINOR_VERSION << "."
     << TAIPAN_PATCH_VERSION;

  date >> month >> day >> year;
  ss << ", date " << day << "." << (1 + months.find(month) / 4) << "." << year.substr(2);
  ss << ", author by Vlad Evka";

  return ss.str();
}

