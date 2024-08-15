#ifndef __PLAYER_INFO_HPP__
#define __PLAYER_INFO_HPP__

#include <string>
#include <optional>
#include <vector>
#include <cstdint>

#include <map>

#include <Data/Characters.hpp>

#include <JSON/JSONValidation.hpp>

namespace CPPVersus {

enum LoginPlatform {
    PC,
    XBoxSeriesX,
    UNKNOWN
};

struct CharacterInfo {
    size_t wins = 0;
    size_t ringouts = 0;

    float highestDamageDealt = 0.0f;
    float totalDamageDealt = 0.0f;
};

struct PlayerStats {
    float highestDamageDealt = 0.0f;
    float totalDamage = 0.0f;
    
    size_t totalRingoutLeader = 0;
    size_t totalRingouts = 0;
    size_t totalWins = 0;

    size_t totalAssists = 0;
    size_t totalAttacksDodged = 0;
    size_t totalDoubleRingouts = 0;
    size_t totalMaxedCharacters = 0;
    
    std::map<uint64_t, CharacterInfo> characters = {};
};

struct PlayerInfo {
    std::string id = "";
    std::string publicID = "";

    std::string createdAt = "";
    std::string updatedAt = "";
    std::string lastLogin = "";

    std::string username = "";

    LoginPlatform lastLoginPlatform = LoginPlatform::UNKNOWN;

    bool openBetaPlayer = false;
    std::string profileIconPath = "";
    
    PlayerStats stats = {};
};

// For JSON validation of the /accounts/{account_id} endpoint
const std::vector<JSONValidation::JSONSchemaValue> accountJSONSchema = {
    { "id", JSONValidation::Validators::stringValidator },
    { "public_id", JSONValidation::Validators::stringValidator },
    
    { "identity", JSONValidation::Validators::objectValidator },
    { "identity.alternate", JSONValidation::Validators::objectValidator },
    { "identity.alternate.wb_network", JSONValidation::Validators::arrayValidator },
    { "identity.alternate.wb_network[0]", JSONValidation::Validators::objectValidator },
    { "identity.alternate.wb_network[0].username", JSONValidation::Validators::stringValidator },

    { "server_data", JSONValidation::Validators::objectValidator },
    { "server_data.LastLoginPlatform", JSONValidation::Validators::stringValidator },
    { "server_data.OpenBeta", JSONValidation::Validators::booleanValidator },
    { "server_data.ProfileIcon", JSONValidation::Validators::objectValidator },
    { "server_data.ProfileIcon.AssetPath", JSONValidation::Validators::stringValidator }
};

// For JSON validation of the /profiles/{account_id} endpoint
const std::vector<JSONValidation::JSONSchemaValue> profileJSONSchema = {
    { "id", JSONValidation::Validators::stringValidator },
    
    { "created_at", JSONValidation::Validators::stringValidator },
    { "updated_at", JSONValidation::Validators::stringValidator },
    { "last_login", JSONValidation::Validators::stringValidator },

    { "server_data", JSONValidation::Validators::objectValidator },
    { "server_data.stat_trackers", JSONValidation::Validators::objectValidator },
};

/**
 * @brief Loads a players info from 2 JSON objects
 * @param accountJSON JSON data returned by the /accounts/{id} endpoint
 * @param profileJSON JSON data returned by the /profiles/{id} endpoint
 * @return Optional type of the PlayerInfo struct
 */
std::optional<PlayerInfo> loadPlayerInfo(nlohmann::json accountJSON, nlohmann::json profileJSON);

};

#endif