#include <Data/PlayerInfo.hpp>

std::optional<CPPVersus::PlayerStats> CPPVersus::loadPlayerStats(nlohmann::json statTrackerJSON) {
    if(CPPVersus::JSONValidation::validateJSONSchema(statTrackerJSON, CPPVersus::statTrackerJSONSchema).has_value()) return std::optional<CPPVersus::PlayerStats>();

    CPPVersus::PlayerStats playerStats = {
        .highestDamageDealt = statTrackerJSON["HighestDamageDealt"],

        .totalRingoutLeader = statTrackerJSON["TotalRingoutLeader"],
        .totalRingouts = statTrackerJSON["TotalRingouts"],
        .totalWins = statTrackerJSON["TotalWins"],

        .totalAssists = statTrackerJSON["TotalAssists"],
        .totalAttacksDodged = statTrackerJSON["TotalAttacksDodged"],
        .totalDoubleRingouts = statTrackerJSON["TotalDoubleRingouts"],
        .totalMaxedCharacters = statTrackerJSON["TotalMaxedCharacters"]
    };

    return std::optional<CPPVersus::PlayerStats>(playerStats);
}

std::optional<CPPVersus::PlayerInfo> CPPVersus::loadPlayerInfo(nlohmann::json accountJSON, nlohmann::json profileJSON) {
    if(CPPVersus::JSONValidation::validateJSONSchema(accountJSON, CPPVersus::accountJSONSchema).has_value()) return std::optional<CPPVersus::PlayerInfo>();
    if(CPPVersus::JSONValidation::validateJSONSchema(profileJSON, CPPVersus::profileJSONSchema).has_value()) return std::optional<CPPVersus::PlayerInfo>();

    CPPVersus::PlayerInfo playerInfo = {
        .rawDataAccounts = accountJSON,
        .rawDataProfiles = profileJSON,

        .id = accountJSON["id"],
        .publicID = accountJSON["public_id"],
        
        .createdAt = profileJSON["created_at"],
        .updatedAt = profileJSON["updated_at"],
        .lastLogin = profileJSON["last_login"],

        .username = accountJSON["identity"]["alternate"]["wb_network"][0]["username"],

        .openBetaPlayer = accountJSON["server_data"]["OpenBeta"],

        .profileIconPath = accountJSON["server_data"]["ProfileIcon"]["AssetPath"]
    };

    std::optional<CPPVersus::PlayerStats> playerStats = loadPlayerStats(profileJSON["server_data"]["stat_trackers"]);
    if(playerStats.has_value()) {
        playerInfo.stats = playerStats.value();
    }

    // Load the players last platform.
    {
        std::string lastPlatform = accountJSON["server_data"]["LastLoginPlatform"];
        if(lastPlatform == "PC") playerInfo.lastLoginPlatform = LoginPlatform::PC;
    }

    return std::optional<CPPVersus::PlayerInfo>(playerInfo);
}