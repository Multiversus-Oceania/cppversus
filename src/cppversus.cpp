/**
@file
@brief Main entry point of the CPPVersus wrapper.
*/

#include <cppversus.hpp>
#include <iostream>

template<class ...Ts>
cpr::Response CPPVersus::DokkenAPIEntry::APIGet(Ts&& ...ts) {
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
cpr::Response CPPVersus::DokkenAPIEntry::APIPost(Ts&& ...ts) {
    return cpr::Post(
        std::forward<Ts>(ts)...,
        cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
        cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
        cpr::Header{{ "x-hydra-access-token", _token.value_or("") }},
        cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }},
        cpr::Header{{ "Content-Type", "application/json" }}
    );
}


void CPPVersus::DokkenAPIEntry::refreshToken() {
    spdlog::debug("Refreshing Access Token");
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
        throw std::runtime_error("Error getting access token, likely invalid token.");
    }

    const nlohmann::json json = nlohmann::json::parse(res.text);
    if(!json.contains("token")) {
        throw std::runtime_error("Invalid response from access endpoint, no token received.");
    }

    _token = json["token"];
    spdlog::info("Got access token: {}.", _token.value());
}


CPPVersus::DokkenAPIEntry::DokkenAPIEntry(std::string token) : _authToken(token) {
    this->refreshToken();
}

CPPVersus::DokkenAPIEntry::~DokkenAPIEntry() {

}


// std::optional<CPPVersus::MinimalPlayerInfo> CPPVersus::DokkenAPIEntry::getMinimalPlayerInfoByUsername(std::string username, uint32_t limit) {
//     if(!_token.has_value()) {
//         return std::optional<MinimalPlayerInfo>();
//     }

//     cpr::Response res = APIGet(
//         cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}", API_URL, username, limit))
//     );

//     if(res.status_code != 200) {
//         return std::optional<MinimalPlayerInfo>();
//     }

//     nlohmann::json json = nlohmann::json::parse(res.text);
//     nlohmann::json::array_t results = json["results"];

//     // nlohmann::json account = std::find_if(results.begin(), results.end(), [](nlohmann::json current) {
//     //     if(!current.is_object()) return false;

//     //     nlohmann::json id = current["id"];
//     //     if(!id.is_string()) return false;

        

//     //     return false;
//     // });

//     return std::optional<MinimalPlayerInfo>();
// }


std::optional<CPPVersus::PlayerInfo> CPPVersus::DokkenAPIEntry::getPlayerInfo(std::string lookupValue, CPPVersus::PlayerLookupType lookupType) {
    if(!_token.has_value()) {
        return std::optional<PlayerInfo>();
    }

    switch(lookupType) {
    case CPPVersus::PlayerLookupType::ID: {
        cpr::Response res = APIGet(
            cpr::Url(fmt::format("{}/accounts/{}", API_URL, lookupValue))
        );

        if(res.status_code != 200) {
            spdlog::error("status code for id lookup != 200: {}\n{}", res.status_code, res.text);
            return std::optional<PlayerInfo>();
        }

        nlohmann::json json = nlohmann::json::parse(res.text);
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

        if(!serverData["Characters"].is_object()) {
            spdlog::error("Characters data was not object");
            return std::optional<PlayerInfo>();
        }

        nlohmann::json::object_t characters = serverData["Characters"];

        for(auto pair : characters) {
            std::optional<Character> character = characterFromName(pair.first);
            if(!character.has_value()) continue;

            playerInfo.characters[character.value()] = {
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