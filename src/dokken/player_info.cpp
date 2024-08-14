#include <DokkenAPI.hpp>
#include <iostream>

using namespace CPPVersus;

std::string joinKeyName(std::vector<std::variant<std::string, int>> path) {
    std::string keyName = "";

    for(size_t i = 0; i < path.size(); i++) {
        const std::variant<std::string, int>& value = path[i];

        if(std::holds_alternative<std::string>(value)) {
            keyName += std::get<std::string>(value);
        }
        else if(std::holds_alternative<int>(value)) {
            keyName += std::to_string(std::get<int>(value));
        }

        if(i != path.size() - 1) {
            keyName += ".";
        }
    }

    return keyName;
}

std::optional<nlohmann::json> DokkenAPI::_getAccountInfo(std::string id, uint64_t _retryNumber) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/accounts/{}", API_URL, id))
    );

    if(res.status_code != 200) {
        _logger.warn("Status code for player account id lookup was: {}, expected: 200", res.status_code);
        if(shouldRetry(res)) {
            _logger.warn("Will retry the request after refreshing token, as the user session was kicked.");

            this->refreshToken();
            return this->_getAccountInfo(id, _retryNumber + 1);
        }

        _logger.warn("Will not retry, failed the request {} times, PLEASE send bug report.", _retryNumber);
        _logger.warn("{}", res.text);

        return std::optional<nlohmann::json>();
    }

    nlohmann::json json = nlohmann::json::parse(res.text);
    std::optional<JSONValidation::JSONSchemaValue> failedValue = JSONValidation::validateJSONSchema(json, accountJSONSchema);
    if(failedValue.has_value()) {
        _logger.error("Invalid JSON schema from player account id lookup, what: {}, PLEASE send a bug report", joinKeyName(failedValue.value().key));
        _logger.error("{}", json.dump());

        return std::optional<nlohmann::json>();
    }

    return std::optional<nlohmann::json>(json);
}

std::optional<nlohmann::json> DokkenAPI::_getProfileInfo(std::string id, uint64_t _retryNumber) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/profiles/{}", API_URL, id))
    );

    if(res.status_code != 200) {
        _logger.warn("Status code for player profile id lookup was: {}, expected: 200", res.status_code);
        if(shouldRetry(res)) {
            _logger.warn("Will retry the request after refreshing token, as the user session was kicked.");

            this->refreshToken();
            return this->_getProfileInfo(id, _retryNumber + 1);
        }

        _logger.warn("Will not retry, failed the request {} times, PLEASE send bug report.", _retryNumber);
        _logger.warn("{}", res.text);

        return std::optional<nlohmann::json>();
    }

    nlohmann::json json = nlohmann::json::parse(res.text);
    std::optional<JSONValidation::JSONSchemaValue> failedValue = JSONValidation::validateJSONSchema(json, profileJSONSchema);
    if(failedValue.has_value()) {
        _logger.error("Invalid JSON schema from player profile id lookup, what: {}, PLEASE send a bug report", joinKeyName(failedValue.value().key));
        _logger.error("{}", json.dump());

        return std::optional<nlohmann::json>();
    }

    return std::optional<nlohmann::json>(json);
}


std::optional<PlayerInfo> DokkenAPI::_getPlayerInfoFromUsername(std::string username, uint64_t _retryNumber) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}&account_fields=identity", API_URL, username, 5))
    );

    return std::optional<PlayerInfo>();
}

std::optional<PlayerInfo> DokkenAPI::_getPlayerInfoFromID(std::string id, uint64_t _retryNumber) {
    // These functions validate the JSON is correct so if no value, return null opt.
    std::optional<nlohmann::json> accountJSONOpt = _getAccountInfo(id, _retryNumber);
    if(!accountJSONOpt.has_value()) return std::optional<PlayerInfo>();

    std::optional<nlohmann::json> profileJSONOpt = _getProfileInfo(id, _retryNumber);
    if(!profileJSONOpt.has_value()) return std::optional<PlayerInfo>();

    // JSON should be correct here.
    nlohmann::json accountJSON = accountJSONOpt.value();
    nlohmann::json profileJSON = profileJSONOpt.value();

    PlayerInfo playerInfo = {
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

    return std::optional<PlayerInfo>(playerInfo);
}


std::optional<PlayerInfo> DokkenAPI::_getPlayerInfo(std::string lookupValue, PlayerLookupType lookupType, uint64_t _retryNumber) {
    if(_retryNumber == maxRetries || !_token.has_value()) return std::optional<PlayerInfo>();

    switch(lookupType) {
    case PlayerLookupType::ID: return _getPlayerInfoFromID(lookupValue, _retryNumber);
    case PlayerLookupType::USERNAME: return _getPlayerInfoFromUsername(lookupValue, _retryNumber);
    default: return std::optional<PlayerInfo>();
    }
}

std::optional<PlayerInfo> DokkenAPI::getPlayerInfo(std::string lookupValue, PlayerLookupType lookupType) {
    return _getPlayerInfo(lookupValue, lookupType, 0);
}