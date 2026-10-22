// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "library/install_plan.hpp"
#include "library/game_install.hpp"
namespace store::library {
// Register the existing validated R4 installation without copying or modifying
// game data, registry, saves, gta_sa.set or the original R4 profile.
bool adopt_gta(const InstallPlan &plan,const InstallPaths &paths,std::string &error);
}
