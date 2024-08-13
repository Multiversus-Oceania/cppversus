#include <DokkenAPI.hpp>
#include <iostream>

using namespace CPPVersus;

// doesnt seem to work with numbers
const static std::vector<JSONValidation::JSONSchemaValue> accountJSONSchema = {
    { { "id" }, JSONValidation::Validators::stringValidator },
    { { "identity" }, JSONValidation::Validators::objectValidator },
    { { "identity", "alternate" }, JSONValidation::Validators::objectValidator },
    { { "identity", "alternate", "wb_network" }, JSONValidation::Validators::arrayValidator },
    { { "identity", "alternate", "wb_network", (std::size_t)0 }, JSONValidation::Validators::objectValidator },
    { { "identity", "alternate", "wb_network", (std::size_t)0, "username" }, JSONValidation::Validators::stringValidator }
};

const static std::vector<JSONValidation::JSONSchemaValue> profileJSONSchema = {
    { { "id" }, JSONValidation::Validators::stringValidator }
};


std::string joinKeyName(std::vector<std::variant<std::string, std::size_t>> path) {
    std::string keyName = "";

    for(std::size_t i = 0; i < path.size(); i++) {
        const std::variant<std::string, std::size_t>& value = path[i];

        if(std::holds_alternative<std::string>(value)) {
            keyName += std::get<std::string>(value);
        }
        else if(std::holds_alternative<std::size_t>(value)) {
            keyName += std::to_string(std::get<std::size_t>(value));
        }

        if(i != path.size() - 1) {
            keyName += ".";
        }
    }

    return keyName;
}


std::optional<PlayerInfo> DokkenAPI::_getPlayerInfoFromUsername(std::string username, uint64_t _retryNumber) {
    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}&account_fields=identity", API_URL, username, 5))
    );

    return std::optional<PlayerInfo>();
}

std::optional<PlayerInfo> DokkenAPI::_getPlayerInfoFromID(std::string id, uint64_t _retryNumber) {
    cpr::Response accountResponse, profileResponse;
    nlohmann::json accountJSON, profileJSON;

    // get account data
    {
        accountResponse = APIGet(
            cpr::Url(fmt::format("{}/accounts/{}", API_URL, id))
        );

        if(accountResponse.status_code != 200) {
            _logger.warn("Status code for player account id lookup was: {}, expected: 200", accountResponse.status_code);
            if(shouldRetry(accountResponse)) {
                _logger.warn("Will retry the request after refreshing token, as the user session was kicked.");

                this->refreshToken();
                return this->_getPlayerInfo(id, PlayerLookupType::ID, _retryNumber + 1);
            }

            _logger.warn("Will not retry, failed the request {} times, PLEASE send bug report.", _retryNumber);
            _logger.warn("{}", accountResponse.text);

            return std::optional<PlayerInfo>();
        }

        accountJSON = nlohmann::json::parse(accountResponse.text);
        std::optional<JSONValidation::JSONSchemaValue> failedValue = JSONValidation::validateJSONSchema(accountJSON, accountJSONSchema);
        if(failedValue.has_value()) {
            _logger.error("Invalid JSON schema from player account id lookup, what: {}, PLEASE send a bug report", joinKeyName(failedValue.value().key));
            _logger.error("{}", accountJSON.dump());

            return std::optional<PlayerInfo>();
        }
    }

    {
        profileResponse = APIGet(
            cpr::Url(fmt::format("{}/profiles/{}", API_URL, id))
        );

        if(profileResponse.status_code != 200) {
            _logger.warn("Status code for player profile id lookup was: {}, expected: 200", profileResponse.status_code);
            if(shouldRetry(profileResponse)) {
                _logger.warn("Will retry the request after refreshing token, as the user session was kicked.");

                this->refreshToken();
                return this->_getPlayerInfo(id, PlayerLookupType::ID, _retryNumber + 1);
            }

            _logger.warn("Will not retry, failed the request {} times, PLEASE send bug report.", _retryNumber);
            _logger.warn("{}", profileResponse.text);

            return std::optional<PlayerInfo>();
        }

        profileJSON = nlohmann::json::parse(profileResponse.text);
        std::optional<JSONValidation::JSONSchemaValue> failedValue = JSONValidation::validateJSONSchema(profileJSON, profileJSONSchema);
        if(failedValue.has_value()) {
            _logger.error("Invalid JSON schema from player profile id lookup, what: {}, PLEASE send a bug report", joinKeyName(failedValue.value().key));
            _logger.error("{}", profileJSON.dump());

            return std::optional<PlayerInfo>();
        }
    }

    return std::optional<PlayerInfo>();
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