#include "version.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <time.h>

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
static const std::string currentDateTime() {
  time_t     now = time(nullptr);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  return buf;
}

std::string engine_info() {
  const std::string months("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
  std::string month, day, year;
  std::stringstream ss;

  ss << "taipan version "
     << TAIPAN_MAJOR_VERSION << "."
     << TAIPAN_MINOR_VERSION << "."
     << TAIPAN_PATCH_VERSION;

  ss << ", date " << currentDateTime();
  ss << ", author by Vlad Yevka";

  return ss.str();
}

