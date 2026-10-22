// Prospero BlackBox - The built-in catalog: Blur.
// SPDX-License-Identifier: GPL-3.0-or-later
#include "catalog/blackbox.hpp"

namespace store::catalog
{
namespace
{
constexpr Game kGames[] = {
    {"PPSA99405", "GTASA", "GTA San Andreas", "2005-06-07", "gtasa",
     "Votre copie PC de GTA San Andreas sur PS5. Commandes PS2, pause Options et activation automatique R4.",
     true, {0x14251b, 0x356d46, 0xb8e083, 0x0c140f}},
};
} // namespace

std::span<const Game> blackbox_games()
{
    return kGames;
}

const Palette &blackbox_palette(std::string_view id)
{
    for (const auto &game : kGames)
        if (id == game.id)
            return game.palette;
    return kManagerPalette;
}

Snapshot blackbox_snapshot(const std::string &app_folder)
{
    Snapshot snapshot;
    for (const auto &game : kGames)
    {
        Entry entry;
        entry.id = game.id;
        entry.name = game.name;
        entry.author = "GTA SA";
        entry.kind = "game";
        entry.status = game.available ? "available" : "coming_soon";
        entry.released = game.released;
        entry.description = game.description;
        entry.icon = app_folder + "/assets/games/" + game.art + ".png";
        snapshot.entries.push_back(std::move(entry));
    }
    // Built into the app: nothing to verify or download.
    snapshot.verified = true;
    snapshot.accepted = true;
    snapshot.online = false;
    return snapshot;
}
} // namespace store::catalog
