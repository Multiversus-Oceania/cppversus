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
    UNKNOWN
};

struct CharacterInfo {
    uint64_t level;
    uint64_t xp;
};

struct BoostInfo {
    uint64_t gamesRemaining;
    std::string expirationDate;
};

struct PlayerInfo {
    std::string id;
    std::string publicID;

    std::string createdAt;
    std::string updatedAt;

    std::string lastLogout;
    std::string lastLogin;

    LoginPlatform lastLoginPlatform;

    uint64_t level;
    uint64_t xp;

    std::string profileIconPath;

    std::map<uint64_t, CharacterInfo> characters;
    std::map<std::string, BoostInfo> activeBoosts;
};

};

#endif