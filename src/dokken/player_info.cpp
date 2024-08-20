#include <Dokken/DokkenAPI.hpp>
#include <iostream>

using namespace CPPVersus;

std::optional<PlayerInfo> DokkenAPI::getPlayerInfo(std::string lookupValue, PlayerLookupType lookupType) {
    switch(lookupType) {
    case PlayerLookupType::ID: {
        std::optional<cpr::Response> accountResponse = APIRequestWithValidation(RequestType::GET, accountJSONSchema, 200, true, cpr::Url(fmt::format("{}/accounts/{}", API_URL, lookupValue)));
        if(!accountResponse.has_value()) return std::optional<PlayerInfo>();

        std::optional<cpr::Response> profileResponse = APIRequestWithValidation(RequestType::GET, profileJSONSchema, 200, true, cpr::Url(fmt::format("{}/profiles/{}", API_URL, lookupValue)));
        if(!profileResponse.has_value()) return std::optional<PlayerInfo>();

        // JSON should be correct here.
        return loadPlayerInfo(nlohmann::json::parse(accountResponse.value().text), nlohmann::json::parse(profileResponse.value().text));
    }
    case PlayerLookupType::USERNAME: {
        cpr::Response res = APIRequest(
            RequestType::GET,
            cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}&account_fields=identity", API_URL, lookupValue, 1))
        );

        return std::optional<PlayerInfo>();
    }
    default: return std::optional<PlayerInfo>();
    }
}