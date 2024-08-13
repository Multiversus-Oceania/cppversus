#include <DokkenAPI.hpp>
#include <iostream>

bool validatePlayerJSON(nlohmann::json json) {
    // TODO: Want to have it more explicit but I hate doing this many ifs. Figure out a more dynamic validation method.
    if(!json["id"].is_string()) {
        spdlog::error("Invalid JSON returned by server, WHAT: id, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["public_id"].is_string()) {
        spdlog::error("Invalid JSON returned by server, WHAT: public_id, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["identity"].is_object()) {
        spdlog::error("Invalid JSON returned by server, WHAT: identity, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["identity"]["username"].is_string()) {
        spdlog::error("Invalid JSON returned by server, WHAT: identity.username, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["identity"]["alternate"].is_object()) {
        spdlog::error("Invalid JSON returned by server, WHAT: identity.alternate, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["identity"]["alternate"]["wb_network"].is_array()) {
        spdlog::error("Invalid JSON returned by server, WHAT: identity.alternate.wb_network, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["created_at"].is_string()) {
        spdlog::error("Invalid JSON returned by server, WHAT: created_at, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["updated_at"].is_string()) {
        spdlog::error("Invalid JSON returned by server, WHAT: updated_at, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"].is_object()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["LastLogoutTime"].is_string() && !json["server_data"]["LastLogoutTime"].is_null()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.LastLogoutTime, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["LastLoginTime"].is_string() && !json["server_data"]["LastLoginTime"].is_null()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.LastLoginTime, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["LastLoginPlatform"].is_string() && !json["server_data"]["LastLoginPlatform"].is_null()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.LastLoginPlatform, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["Level"].is_number_unsigned() && !json["server_data"]["Level"].is_null()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.Level, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["CurrentXP"].is_number_unsigned() && !json["server_data"]["CurrentXP"].is_null()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.CurrentXP, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["ProfileIcon"].is_object()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.ProfileIcon, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["ProfileIcon"]["AssetPath"].is_string()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.ProfileIcon.AssetPath, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }
    else if(!json["server_data"]["Characters"].is_object() && !json["server_data"]["Characters"].is_null()) {
        spdlog::error("Invalid JSON returned by server, WHAT: server_data.Characters, PLEASE send a bug report.\n{}", json.dump());
        return false;
    }

    return true;
}


std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPI::_getPlayerInfoFromUsername(std::string username, uint64_t _retryNumber) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}&account_fields=identity", API_URL, username, 5))
    );

    if(res.status_code != 200) {
        spdlog::warn("Status code for username lookup != 200 was: {}\n{}", res.status_code, res.text);
        if(shouldRetry(res)) {
            spdlog::warn("Will retry the request after refreshing token, as the user session was kicked.");

            this->refreshToken();
            return this->_getPlayerInfo(username, PlayerLookupType::USERNAME, _retryNumber + 1);
        }

        return std::optional<PlayerInfo>();
    }

    std::string cmp_username = username;
    for(char& c : username) c = tolower(c);

    nlohmann::json json = nlohmann::json::parse(res.text);
    nlohmann::json::array_t results = json["results"];
    
    for(nlohmann::json current : results) {
        if(
            !current.is_object() ||
            !current["result"].is_object() ||
            !current["result"]["account_id"].is_string() ||
            !current["result"]["account"]["identity"].is_object() ||
            !current["result"]["account"]["identity"]["alternate"].is_object() ||
            !current["result"]["account"]["identity"]["alternate"]["wb_network"].is_array() ||
            !current["result"]["account"]["identity"]["alternate"]["wb_network"][0].is_object() ||
            !current["result"]["account"]["identity"]["alternate"]["wb_network"][0]["username"].is_string()
        ) {
            spdlog::warn("Data from username lookup had invalid schema. PLEASE file a bug report\n{}", current.dump());
            continue;
        }

        std::string id = current["result"]["account_id"];
        std::string user = current["result"]["account"]["identity"]["alternate"]["wb_network"][0]["username"];
        for(char& c : user) c = tolower(c);

        if(user == cmp_username) {
            std::optional<CPPVersus::PlayerInfo> info = this->getPlayerInfo(id, CPPVersus::PlayerLookupType::ID);

            if(info.has_value()) {
                return info.value();
            }
        }
    }

    return std::optional<PlayerInfo>();
}

std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPI::_getPlayerInfoFromID(std::string id, uint64_t _retryNumber) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/accounts/{}", API_URL, id))
    );

    if(res.status_code != 200) {
        spdlog::warn("Status code for ID lookup != 200 was: {}\n{}", res.status_code, res.text);
        if(shouldRetry(res)) {
            spdlog::warn("Will retry the request after refreshing token, as the user session was kicked.");

            this->refreshToken();
            return this->_getPlayerInfo(id, PlayerLookupType::ID, _retryNumber + 1);
        }
        
        return std::optional<PlayerInfo>();
    }

    nlohmann::json json = nlohmann::json::parse(res.text);
    nlohmann::json serverData = json["server_data"];
    
    // Validate JSON schema here
    if(!validatePlayerJSON(json)) {
        return std::optional<CPPVersus::PlayerInfo>();
    }
    
    // TODO: There may be some edge cases with lastLogout, etc, not too sure though. 
    CPPVersus::PlayerInfo playerInfo = {
        .id = json["id"],
        .publicID = json["public_id"],

        .username = json["identity"]["username"],

        .createdAt = json["created_at"],
        .updatedAt = json["updated_at"],
        
        .lastLogout = serverData["LastLogoutTime"],
        .lastLogin = serverData["LastLoginTime"],

        .profileIconPath = serverData["ProfileIcon"]["AssetPath"],

        .characters = {},
        .activeBoosts = {}
    };

    if(serverData["Level"].is_number_unsigned()) playerInfo.level = serverData["Level"];
    if(serverData["CurrentXP"].is_number_unsigned()) playerInfo.xp = serverData["CurrentXP"];

    // try assign a better username here
    nlohmann::json::array_t wb_network = json["identity"]["alternate"]["wb_network"];
    if(wb_network.size() > 0 && wb_network[0]["username"].is_string()) {
        std::string username = wb_network[0]["username"];

        if(username.size() > 0) {
            playerInfo.username = username;
        }
    }

    if(serverData["LastLoginPlatform"].is_string()) {
        std::string loginPlatform = serverData["LastLoginPlatform"];

        if(loginPlatform == "PC") {
            playerInfo.lastLoginPlatform = CPPVersus::LoginPlatform::PC;
        }
    }

    nlohmann::json::object_t characters = serverData["Characters"];
    for(auto pair : characters) {
        std::optional<Character> character = characterFromSlug(pair.first);
        if(!character.has_value()) {
            spdlog::error("Error couldn't find character {}, PLEASE send a bug report.\n{}", pair.first, pair.second.dump());
            return std::optional<PlayerInfo>();
        }

        if(
            !pair.second.is_object() ||
            !pair.second["Mastery"].is_object() ||
            !pair.second["Mastery"]["Level"].is_number_unsigned() ||
            !pair.second["Mastery"]["CurrentXP"].is_number_unsigned()
        ) {
            spdlog::error("Error while validating JSON data for {}, PLEASE send a bug report.\n{}", pair.first, pair.second.dump());
            return std::optional<PlayerInfo>();
        }

        playerInfo.characters[character.value().id] = {
            .level = pair.second["Mastery"]["Level"],
            .xp = pair.second["Mastery"]["CurrentXP"]
        };
    }

    return playerInfo;
}


std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPI::_getPlayerInfo(std::string lookupValue, CPPVersus::PlayerLookupType lookupType, uint64_t _retryNumber) {
    if(_retryNumber == maxRetries || !_token.has_value()) return std::optional<CPPVersus::PlayerInfo>();

    switch(lookupType) {
    case CPPVersus::PlayerLookupType::ID: return _getPlayerInfoFromID(lookupValue, _retryNumber);
    case CPPVersus::PlayerLookupType::USERNAME: return _getPlayerInfoFromUsername(lookupValue, _retryNumber);
    default: return std::optional<PlayerInfo>();
    }
}

std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPI::getPlayerInfo(std::string lookupValue, CPPVersus::PlayerLookupType lookupType) {
    return _getPlayerInfo(lookupValue, lookupType, 0);
}