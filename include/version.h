#ifndef TAIPAN_VERSION_H
#define TAIPAN_VERSION_H

#include <string>

#define TAIPAN_CONFIG_VERSION(version, revision, patch) \
    (((version) << 24) + ((revision) << 16) + (patch))

#define TAIPAN_MAJOR_VERSION 0
#define TAIPAN_MINOR_VERSION 1
#define TAIPAN_PATCH_VERSION 0

#define TAIPAN_VERSION                                              \
    TAIPAN_CONFIG_VERSION(TAIPAN_MAJOR_VERSION,                     \
                          TAIPAN_MINOR_VERSION,                     \
                          TAIPAN_PATCH_VERSION)                     \

std::string engine_info();

#endif //TAIPAN_VERSION_H
