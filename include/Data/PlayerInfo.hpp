#ifndef __PLAYER_INFO_HPP__
#define __PLAYER_INFO_HPP__

#include <string>
#include <optional>
#include <vector>
#include <cstdint>

#include <stdlib.h>

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

/**
 * @brief Represents a players data, made from data returned by /accounts/ & /profiles/
 * @param rawDataAccounts Unprocessed JSON from the /accounts/ endpoint
 * @param rawDataProfiles Unprocessed JSON from the /profiles/ endpoint
 * @param id Account ID of the player
 * @param publicID Public ID of the player. (not too sure if it's useful)
 * @param createdAt When the player created their account.
 * @param updatedAt When the players account data last changed.
 * @param lastLogin When the player last logged into their account.
 * @param username Username of the player. (usually found in identity.alternate.wb_network[0].username from /accounts/)
 * @param lastLoginPlatform What platform the player last used.
 * @param openBetaPlayer If the player played during the open beta period.
 * @param profileIconPath The asset path of the players icon, could be used in a viewer if you have the assets dumped.
 * @param stats Player stat tracker info.
 */
struct PlayerInfo {
    nlohmann::json rawDataAccounts;
    nlohmann::json rawDataProfiles;

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

const std::vector<JSONValidation::JSONSchemaValue> statTrackerJSONSchema = {
    { "HighestDamageDealt", JSONValidation::Validators::floatValidator },
    
    { "TotalRingoutLeader", JSONValidation::Validators::integerValidator },
    { "TotalRingouts", JSONValidation::Validators::integerValidator },
    { "TotalWins", JSONValidation::Validators::integerValidator },

    { "TotalAssists", JSONValidation::Validators::integerValidator },
    { "TotalAttacksDodged", JSONValidation::Validators::integerValidator },
    { "TotalDoubleRingouts", JSONValidation::Validators::integerValidator },
    { "TotalMaxedCharacters", JSONValidation::Validators::integerValidator },

    { "character_wins", JSONValidation::Validators::objectValidator },
    { "character_ringouts", JSONValidation::Validators::objectValidator },
    { "character_highest_damage_dealt", JSONValidation::Validators::objectValidator },
    { "character_total_damage_dealt", JSONValidation::Validators::objectValidator }
};

/**
 * @brief Loads a players stats from the server_data.stat_trackers object
 * @details The JSON stat_trackers object can be acquired from the following endpoint: /profiles/{id}. This does do JSON validation, so you could send it through immediately after getting the response, but I would advise checking for errors for more error info.
 * @param statTrackersJSON JSON data returned by the /profile/{id} endpoint
 * @return Optional type of the PlayerStats struct
 */
std::optional<PlayerStats> loadPlayerStats(nlohmann::json statTrackersJSON);


/**
 * @brief Loads a players info from 2 JSON objects
 * @details The JSON objects can be acquired from the following endpoints: /accounts/{id}, /profiles/{id}. This does do JSON validation, so you could send it through immediately after getting the response, but I would advise checking for errors for more error info.
 * @param accountJSON JSON data returned by the /accounts/{id} endpoint
 * @param profileJSON JSON data returned by the /profiles/{id} endpoint
 * @return Optional type of the PlayerInfo struct
 */
std::optional<PlayerInfo> loadPlayerInfo(nlohmann::json accountJSON, nlohmann::json profileJSON);

};

#endif