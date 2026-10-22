// SPDX-License-Identifier: GPL-3.0-or-later
#include "library/adopt_gta.hpp"
#include "library/prefix.hpp"
#include "core/save_file.hpp"
#include <sys/stat.h>
#include <unistd.h>
namespace store::library {
bool adopt_gta(const InstallPlan &plan,const InstallPaths &paths,std::string &error)
{
    if (plan.game!="GTASA" || plan.prefix!="gta-sa-ps2-r4" || plan.profile.id!="gta-sa-homebrew")
        return false;
    if (access(record_path(plan,paths).c_str(),F_OK)==0) return true;
    if (!prefix_ready(paths.win_root,plan.prefix)) return false;
    const auto folder=paths.win_root+"/prefixes/"+plan.prefix+"/drive_c/Games/GTASA";
    struct stat info{};
    for (const auto *name : {"/gta_sa.exe","/data/gta.dat","/models/gta3.img"}) {
        if (lstat((folder+name).c_str(),&info) || !S_ISREG(info.st_mode) || info.st_size<=0)
            return false;
    }
    if (!install_profile(paths.win_root+"/profiles",plan.profile,plan.profile_after,error)) return false;
    const auto directory=paths.data_root+"/installs/"+plan.prefix;
    if (!hui::save::ensure_directory(paths.data_root) ||
        !hui::save::ensure_directory(paths.data_root+"/installs") ||
        !hui::save::ensure_directory(directory)) { error="Can't create GTA installation record"; return false; }
    const std::string record="{\"schema\":1,\"game\":\"GTASA\",\"release\":\"classic\",\"base_release\":\"classic\",\"prefix\":\"gta-sa-ps2-r4\",\"profile\":\"gta-sa-homebrew\",\"adopted\":true,\"options\":[],\"sources\":{}}\n";
    error=hui::save::write_atomic(record_path(plan,paths),record);
    return error.empty();
}
}
