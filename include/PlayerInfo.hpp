#ifndef __PLAYER_INFO_HPP__
#define __PLAYER_INFO_HPP__

#include <string>
#include <optional>
#include <vector>
#include <cstdint>

#include <map>

#include <Characters.hpp>

namespace CPPVersus {

enum LoginPlatform {
    PC,
    XBoxSeriesX,
    UNKNOWN
};

struct CharacterInfo {
    uint64_t level;
    uint64_t xp;
};

struct PlayerInfo {
    std::string id = "";
    std::string publicID = "";

    std::string username = "";

    std::string createdAt = "";
    std::string updatedAt = "";

    std::optional<std::string> lastLogout = std::optional<std::string>();
    std::optional<std::string> lastLogin = std::optional<std::string>();

    LoginPlatform lastLoginPlatform = LoginPlatform::UNKNOWN;

    uint64_t level = 0;
    uint64_t xp = 0;

    std::string profileIconPath = "";
    std::map<uint64_t, CharacterInfo> characters = {};
};

};

#endif