#include <Dokken/DokkenAPI.hpp>
#include <iostream>

using namespace CPPVersus;

bool DokkenAPI::shouldRetry(cpr::Response res) {
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


void DokkenAPI::refreshToken() {
    _logger.info("Refreshing access token, url: {}/access.", API_URL);
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
        _logger.critical("Error getting access token, likely invalid token.");
        _logger.debug("Status Code: {}, Server Text: {}", res.status_code, res.text);
        
        throw std::runtime_error(fmt::format("Error getting access token."));
    }

    nlohmann::json json = nlohmann::json::parse(res.text);
    if(
        !json.is_object() ||
        !json["token"].is_string()
    ) {
        _logger.critical("Invalid repsonse from access endpoint, no token received, PLEASE send a bug report.");
        _logger.debug("{}", res.text);

        throw std::runtime_error("Invalid response from access endpoint.");
    }

    _token = json["token"];
    _logger.debug("Got access token: {}.", _token.value());
}


DokkenAPI::DokkenAPI(std::string token, spdlog::level::level_enum logLevel) :
    _authToken(token),
    _loggerSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()),
    _logger(spdlog::logger("DokkenAPI", _loggerSink)) {
    _logger.set_level(logLevel);

    this->refreshToken();
}

DokkenAPI::~DokkenAPI() {

}