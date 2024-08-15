#include <Data/PlayerInfo.hpp>

std::optional<CPPVersus::PlayerInfo> CPPVersus::loadPlayerInfo(nlohmann::json accountJSON, nlohmann::json profileJSON) {
    if(CPPVersus::JSONValidation::validateJSONSchema(accountJSON, CPPVersus::accountJSONSchema).has_value()) return std::optional<PlayerInfo>();
    if(CPPVersus::JSONValidation::validateJSONSchema(profileJSON, CPPVersus::profileJSONSchema).has_value()) return std::optional<PlayerInfo>();

    CPPVersus::PlayerInfo playerInfo = {
        .id = accountJSON["id"],
        .publicID = accountJSON["public_id"],
        
        .createdAt = profileJSON["created_at"],
        .updatedAt = profileJSON["updated_at"],
        .lastLogin = profileJSON["last_login"],

        .username = accountJSON["identity"]["alternate"]["wb_network"][0]["username"],

        .openBetaPlayer = accountJSON["server_data"]["OpenBeta"],

        .profileIconPath = accountJSON["server_data"]["ProfileIcon"]["AssetPath"]
    };

    // Load the players last platform.
    {
        std::string lastPlatform = accountJSON["server_data"]["LastLoginPlatform"];
        if(lastPlatform == "PC") playerInfo.lastLoginPlatform = LoginPlatform::PC;
    }

    return std::optional<CPPVersus::PlayerInfo>(playerInfo);
}