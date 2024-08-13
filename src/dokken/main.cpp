#include <DokkenAPI.hpp>
#include <iostream>

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
        catch(nlohmann::json::parse_error const&) {
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