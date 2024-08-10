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

    nlohmann::json json = nlohmann::json::parse(res.text);
    if(!json.contains("token")) {
        throw std::runtime_error("Invalid response from access endpoint, no token received.");
    }

    _token = json["token"];
    spdlog::debug("Got access token: {}.", _token.value());
}


CPPVersus::DokkenAPIEntry::DokkenAPIEntry(std::string token) : _authToken(token) {
    this->refreshToken();
}

CPPVersus::DokkenAPIEntry::~DokkenAPIEntry() {

}


std::optional<CPPVersus::MinimalPlayerInfo> CPPVersus::DokkenAPIEntry::getPlayerInfoByUsername(std::string username, uint32_t limit) {
    if(!_token.has_value()) {
        return std::optional<MinimalPlayerInfo>();
    }

    cpr::Response res = APIGet(
        cpr::Url(fmt::format("{}/profiles/search_queries/get-by-username/run?username={}&limit={}", API_URL, username, limit))
    );

    if(res.status_code != 200) {
        return std::optional<MinimalPlayerInfo>();
    }

    return std::optional<MinimalPlayerInfo>();
}