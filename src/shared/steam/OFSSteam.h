//
// Created by fenteale on 10/16/20.
//

#ifndef OFLAUNCHER_STAINLESS_OFSSTEAM_H
#define OFLAUNCHER_STAINLESS_OFSSTEAM_H

#include <filesystem>
#include <string>
#include "OFSPathDiscover.h"
#include "OFSSteamApp.h"

namespace fs = std::filesystem;

class OFSSteam {
private:
	OFSPathDiscover _steamDefaultDir;
	fs::path _steamPath;
	std::vector<std::string> _libPaths;
	//std::string _sourceMod;
public:
	OFSSteam(std::string steamPath = "");
	fs::path getSteamPath();
	fs::path getSourcemodsPath();
	fs::path getSteamappsPath();
	OFSSteamApp *getApp(int appID);

};

#endif // OFLAUNCHER_STAINLESS_OFSSTEAM_H