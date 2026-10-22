// SPDX-License-Identifier: GPL-3.0-or-later
#include "library/adopt_gta.hpp"
#include "library/install_plan.hpp"
#include "library/game_profile.hpp"
#include "library/game_install.hpp"
#include "core/save_file.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
namespace fs=std::filesystem;
using namespace store::library;
static void put(const fs::path &path,const std::string &body) {
    fs::create_directories(path.parent_path()); std::ofstream(path,std::ios::binary)<<body;
}
static std::string read(const fs::path &path) {
    std::string text; assert(hui::save::read_file(path.string(),&text)); return text;
}
int main(int argc,char **argv) {
    assert(argc==3); InstallPlan plan; std::string error,profile;
    assert(load_plan(argv[1],plan,error));
    assert(plan.game=="GTASA" && plan.profile.id=="gta-sa-homebrew");
    assert(plan.profile.ps2_controls && plan.profile.debug_env.size()==4);
    InstallOptions options; options.cursor=Cursor::touchpad;
    auto effective=effective_plan(plan,options);
    assert(!effective.profile.touchpad_mouse);
    assert(profile_text(effective.profile,profile,error));
    assert(profile.find("cross = enter")!=std::string::npos);
    assert(profile.find("options = escape")!=std::string::npos);
    assert(profile.find("mouse = right_stick")!=std::string::npos);
    assert(profile.find("mouse = touchpad")==std::string::npos);
    assert(profile.find("PW_DINPUT_GTA_PS2=1")!=std::string::npos);
    assert(profile.find("wmvcore=")!=std::string::npos);
    put(argv[2],profile);
    char pattern[]="/tmp/gtasa-homebrew-test-XXXXXX"; const auto temp=mkdtemp(pattern); assert(temp);
    fs::path root=temp; InstallPaths paths{(root/"data").string(),(root/"win").string()};
    assert(!adopt_gta(plan,paths,error));
    const auto prefix=root/"win/prefixes/gta-sa-ps2-r4";
    fs::create_directories(prefix/"drive_c/windows");
    put(prefix/"system.reg","SYSTEM ORIGINAL"); put(prefix/"user.reg","USER ORIGINAL");
    const auto game=prefix/"drive_c/Games/GTASA";
    put(game/"gta_sa.exe","GAME ORIGINAL"); put(game/"data/gta.dat","DATA ORIGINAL");
    put(game/"models/gta3.img","MODELS ORIGINAL");
    auto save=prefix/"drive_c/users/prospero/Documents/GTA San Andreas User Files/GTASAsf1.b";
    auto settings=save.parent_path()/"gta_sa.set";
    put(save,"SAVE ORIGINAL"); put(settings,"CAMERA AND JOYPAD ORIGINAL");
    put(root/"win/profiles/gta-sa-ps2-r4.profile","NATIVE PROFILE ORIGINAL");
    put(root/"win/profiles/profiles.lst","gta-sa-ps2-r4.profile\nother-game.profile\n");
    assert(adopt_gta(plan,paths,error));
    assert(read(game/"gta_sa.exe")=="GAME ORIGINAL");
    assert(read(save)=="SAVE ORIGINAL" && read(settings)=="CAMERA AND JOYPAD ORIGINAL");
    assert(read(prefix/"system.reg")=="SYSTEM ORIGINAL" && read(prefix/"user.reg")=="USER ORIGINAL");
    assert(read(root/"win/profiles/gta-sa-ps2-r4.profile")=="NATIVE PROFILE ORIGINAL");
    assert(read(root/"win/profiles/profiles.lst").find("other-game.profile")!=std::string::npos);
    auto newprofile=root/"win/profiles/gta-sa-homebrew.profile";
    put(newprofile,"PLAYER CHANGED SETTINGS");
    assert(adopt_gta(plan,paths,error));
    assert(read(newprofile)=="PLAYER CHANGED SETTINGS");
    assert(!uninstall_game(plan,paths,options,error));
    assert(read(save)=="SAVE ORIGINAL" && fs::exists(game/"gta_sa.exe"));
    // Fresh-install path uses the same real Sahin installer, with synthetic
    // game data. The base copy is read only and optional saves never overwrite.
    const auto fresh=root/"fresh";
    InstallPaths fresh_paths{(fresh/"data").string(),(fresh/"win").string()};
    const auto fp=fresh/"win/prefixes/gta-sa-ps2-r4";
    fs::create_directories(fp/"drive_c/windows");
    fs::create_directories(fp/"drive_c/users/prospero/Documents");
    put(fp/"system.reg","WINE REGISTRY Version 2\n\n#arch=win64\n\n");
    put(fp/"user.reg","WINE REGISTRY Version 2\n\n#arch=win64\n\n");
    const auto base=fresh/"data/base-games/GTASA";
    for (const auto *dir : {"data","models","audio","anim","text"}) fs::create_directories(base/dir);
    put(base/"gta_sa.exe","SYNTHETIC GAME"); put(base/"data/gta.dat","SYNTHETIC DATA");
    put(base/"models/gta3.img","SYNTHETIC MODELS"); put(base/"anim/ped.ifp","SYNTHETIC ANIM");
    put(base/"dinput8.dll","OLD MOD TO OMIT");
    put(fresh/"data/saves/GTASA/GTASAsf1.b","PLAYER SEED");
    const auto *manifest=manifest_for("GTASA"); assert(manifest);
    std::atomic<bool> cancel{false};
    error.clear();
    assert(install_game(plan,*manifest,fresh_paths,options,[](const InstallStatus &){},cancel,error));
    assert(read(fp/"drive_c/Games/GTASA/gta_sa.exe")=="SYNTHETIC GAME");
    assert(read(base/"gta_sa.exe")=="SYNTHETIC GAME" && read(base/"dinput8.dll")=="OLD MOD TO OMIT");
    assert(!fs::exists(fp/"drive_c/Games/GTASA/dinput8.dll"));
    const auto imported=fp/"drive_c/users/prospero/Documents/GTA San Andreas User Files/GTASAsf1.b";
    assert(read(imported)=="PLAYER SEED");
    put(imported,"NEW IN-GAME SAVE");
    assert(install_game(plan,*manifest,fresh_paths,options,[](const InstallStatus &){},cancel,error));
    assert(read(imported)=="NEW IN-GAME SAVE");
    fs::remove_all(root);
    std::cout<<"PASS GTA plan, R4 inputs, adoption, fresh install/repair, mod exclusion and save/settings preservation\n";
}
