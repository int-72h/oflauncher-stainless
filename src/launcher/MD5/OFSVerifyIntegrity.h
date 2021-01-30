//
// Created by fenteale on 11/15/20.
//

#ifndef OFLAUNCHER_STAINLESS_OFSVERIFYINTEGRITY_H
#define OFLAUNCHER_STAINLESS_OFSVERIFYINTEGRITY_H

#include <string>
#include "OFFilesystem.h"
#include <iostream>
#include "../hashlib2plus/hashlibpp.h"

bool verifyIntegrity(fs::path fToCheck, std::string md5db);

#endif // OFLAUNCHER_STAINLESS_OFSVERIFYINTEGRITY_H
