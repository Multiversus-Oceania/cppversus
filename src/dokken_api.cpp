#include <DokkenAPI.hpp>
#include <iostream>

template<class ...Ts>
cpr::Response CPPVersus::DokkenAPI::APIGet(Ts&& ...ts) {
    return cpr::Get(
        std::forward<Ts>(ts)...,
        cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
        cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
        cpr::Header{{ "x-hydra-access-token", _token.value_or("") }},
        cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }},
        cpr::Header{{ "Content-Type", "application/json" }}
    );
}

template<class ...Ts>
cpr::Response CPPVersus::DokkenAPI::APIPost(Ts&& ...ts) {
    return cpr::Post(
        std::forward<Ts>(ts)...,
        cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
        cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
        cpr::Header{{ "x-hydra-access-token", _token.value_or("") }},
        cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }},
        cpr::Header{{ "Content-Type", "application/json" }}
    );
}

bool CPPVersus::DokkenAPI::shouldRetry(cpr::Response res) {
    if(res.status_code == 401) {
        try {
            nlohmann::json json = nlohmann::json::parse(res.text);
            if(!json["msg"].is_string()) {
                return false;
            }

            std::string msg = json["msg"];
            if(msg == "User session kicked") {
                return true;
            }
        }
        catch(nlohmann::json::parse_error err) {
            return false;
        }
    }

    return false;
}


void CPPVersus::DokkenAPI::refreshToken() {
    spdlog::info("Refreshing access token, url: {}/access.", API_URL);
    _token = std::nullopt;

    cpr::Response res = APIPost(
        cpr::Url(fmt::format("{}/access", API_URL)),
        cpr::Body(
            nlohmann::json::object({
                {
                    "auth", nlohmann::json::object({
                        { "fail_on_missing", true },
                        { "steam", _authToken }
                    })
                },
                {
                    "options",
                    nlohmann::json::array({
                        "wb_network"
                    })
                }
            }).dump()
        )
    );

    if(res.status_code != 200) {
        spdlog::critical("Error getting access token, likely invalid token.\nStatus Code: {}, Server Text: {}", res.status_code, res.text);
        throw std::runtime_error(fmt::format("Error getting access token."));
    }

    nlohmann::json json = nlohmann::json::parse(res.text);
    if(
        !json.is_object() ||
        !json["token"].is_string()
    ) {
        spdlog::critical("Invalid repsonse from access endpoint, no token received, PLEASE send a bug report.\n{}", res.text);
        throw std::runtime_error("Invalid response from access endpoint.");
    }

    _token = json["token"];
    spdlog::debug("Got access token: {}.", _token.value());
}


CPPVersus::DokkenAPI::DokkenAPI(std::string token) : _authToken(token) {
    this->refreshToken();
}

CPPVersus::DokkenAPI::~DokkenAPI() {

}


std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPI::_getPlayerInfo(std::string lookupValue, CPPVersus::PlayerLookupType lookupType, uint64_t _retryNumber) {
    if(_retryNumber == maxRetries) return std::optional<CPPVersus::PlayerInfo>();
    if(!_token.has_value()) {
        return std::optional<PlayerInfo>();
    }

    switch(lookupType) {
    case CPPVersus::PlayerLookupType::ID: {
        cpr::Response res = APIGet(
            cpr::Url(fmt::format("{}/accounts/{}", API_URL, lookupValue))
        );

        if(res.status_code != 200) {
            spdlog::warn("Status code for ID lookup != 200 was: {}\n{}", res.status_code, res.text);
            if(shouldRetry(res)) {
                spdlog::warn("Will retry the request after refreshing token, as the user session was kicked.");

                this->refreshToken();
                return this->_getPlayerInfo(lookupValue, lookupType, _retryNumber + 1);
            }

            
            return std::optional<PlayerInfo>();
        }

        nlohmann::json json = nlohmann::json::parse(res.text);
        
        // Validate JSON schema here
        if(
            !json["id"].is_string() ||
            !json["public_id"].is_string() ||
            !json["created_at"].is_string() ||
            !json["updated_at"].is_string() ||
            !json["server_data"].is_object() ||
            !json["server_data"]["LastLogoutTime"].is_string() ||
            !json["server_data"]["LastLoginTime"].is_string() ||
            !json["server_data"]["LastLoginPlatform"].is_string() ||
            !json["server_data"]["Level"].is_number_unsigned() ||
            !json["server_data"]["CurrentXP"].is_number_unsigned() ||
            !json["server_data"]["ProfileIcon"].is_object() ||
            !json["server_data"]["ProfileIcon"]["AssetPath"].is_string() ||
            !json["server_data"]["Characters"].is_object()
        ) {
            spdlog::error("Innvalid JSON returned by server, PLEASE send a bug report.\n{}", res.text);
            return std::optional<PlayerInfo>();
        }

        nlohmann::json serverData = json["server_data"];
        
        // TODO: There may be some edge cases with lastLogout, etc, not too sure though. 
        CPPVersus::PlayerInfo playerInfo = {
            .id = json["id"],
            .publicID = json["public_id"],

            .createdAt = json["created_at"],
            .updatedAt = json["updated_at"],
            
            .lastLogout = serverData["LastLogoutTime"],
            .lastLogin = serverData["LastLoginTime"],

            .level = serverData["Level"],
            .xp = serverData["CurrentXP"],

            .profileIconPath = serverData["ProfileIcon"]["AssetPath"]
        };

        if(serverData["LastLoginPlatform"] == "PC") {
            playerInfo.lastLoginPlatform = CPPVersus::LoginPlatform::PC;
        }
        else {
            playerInfo.lastLoginPlatform = CPPVersus::LoginPlatform::UNKNOWN;
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
    default: break;
    }

    return std::optional<PlayerInfo>();

    // cpr::Response res = APIGet(
    //     cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}", API_URL, username, limit))
    // );

    // if(res.status_code != 200) {
    //     return std::optional<MinimalPlayerInfo>();
    // }

    // nlohmann::json json = nlohmann::json::parse(res.text);
    // nlohmann::json::array_t results = json["results"];

    // // nlohmann::json account = std::find_if(results.begin(), results.end(), [](nlohmann::json current) {
    // //     if(!current.is_object()) return false;

    // //     nlohmann::json id = current["id"];
    // //     if(!id.is_string()) return false;

        

    // //     return false;
    // // });

    // return std::optional<MinimalPlayerInfo>();
}

std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPI::getPlayerInfo(std::string lookupValue, CPPVersus::PlayerLookupType lookupType) {
    return _getPlayerInfo(lookupValue, lookupType, 0);
}