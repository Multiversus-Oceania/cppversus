#include <Dokken/DokkenAPI.hpp>
#include <iostream>

using namespace CPPVersus;

std::optional<nlohmann::json> DokkenAPI::_getAccountInfo(std::string id, uint64_t _retryNumber) {
    if(_retryNumber == maxRetries || !_token.has_value()) return std::optional<nlohmann::json>();

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
        _logger.error("Invalid JSON schema from player account id lookup, what: {}, PLEASE send a bug report", failedValue.value().key);
        _logger.error("{}", json.dump());

        return std::optional<nlohmann::json>();
    }

    return std::optional<nlohmann::json>(json);
}

std::optional<nlohmann::json> DokkenAPI::_getProfileInfo(std::string id, uint64_t _retryNumber) {
    if(_retryNumber == maxRetries || !_token.has_value()) return std::optional<nlohmann::json>();

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
        _logger.error("Invalid JSON schema from player profile id lookup, what: {}, PLEASE send a bug report", failedValue.value().key);
        _logger.error("{}", json.dump());

        return std::optional<nlohmann::json>();
    }

    return std::optional<nlohmann::json>(json);
}


std::optional<PlayerInfo> DokkenAPI::_getPlayerInfoFromUsername(std::string username) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}&account_fields=identity", API_URL, username, 5))
    );

    return std::optional<PlayerInfo>();
}

std::optional<PlayerInfo> DokkenAPI::_getPlayerInfoFromID(std::string id) {
    // These functions validate the JSON is correct so if no value, return null opt.
    std::optional<nlohmann::json> accountJSON = _getAccountInfo(id);
    if(!accountJSON.has_value()) return std::optional<PlayerInfo>();

    std::optional<nlohmann::json> profileJSON = _getProfileInfo(id);
    if(!profileJSON.has_value()) return std::optional<PlayerInfo>();

    // JSON should be correct here.
    return loadPlayerInfo(accountJSON.value(), profileJSON.value());
}


std::optional<PlayerInfo> DokkenAPI::getPlayerInfo(std::string lookupValue, PlayerLookupType lookupType) {
    switch(lookupType) {
    case PlayerLookupType::ID: return _getPlayerInfoFromID(lookupValue);
    case PlayerLookupType::USERNAME: return _getPlayerInfoFromUsername(lookupValue);
    default: return std::optional<PlayerInfo>();
    }
}