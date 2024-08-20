#ifndef __DOKKEN_API_HPP__
#define __DOKKEN_API_HPP__

#include <string>
#include <optional>
#include <execution>
#include <algorithm>
#include <stdexcept>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fmt/core.h>

#include <nlohmann/json.hpp>

#include <JSON/JSONUtils.hpp>
#include <JSON/JSONValidation.hpp>
#include <Data/PlayerInfo.hpp>

namespace CPPVersus {
const std::string API_URL = "https://dokken-api.wbagora.com";

enum PlayerLookupType {
    ID,
    USERNAME
};

/**
 * @brief Handles all of the Dokken API endpoints.
 * @details Must be initialized with valid token
*/
class DokkenAPI {
private:
    enum RequestType {
        GET,
        POST
    };

    const static uint64_t maxRetries = 5;

    const std::string _authToken;
    std::optional<std::string> _token;

    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _loggerSink;
    spdlog::logger _logger;

    bool _shouldRefreshToken(cpr::Response res);

    template<class ...Ts>
    cpr::Response APIRequest(RequestType requestType, Ts&& ...ts) {
        switch(requestType) {
        case POST: 
            return cpr::Post(
                std::forward<Ts>(ts)...,
                cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
                cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
                cpr::Header{{ "x-hydra-access-token", _token.value_or("") }},
                cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }},
                cpr::Header{{ "Content-Type", "application/json" }}
            );
        default:
            return cpr::Get(
                std::forward<Ts>(ts)...,
                cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
                cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
                cpr::Header{{ "x-hydra-access-token", _token.value_or("") }},
                cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }},
                cpr::Header{{ "Content-Type", "application/json" }}
            );
        }
    }

    template<class ...Ts>
    std::optional<cpr::Response> APIRequestWithValidation(RequestType requestType, std::vector<JSONValidation::JSONSchemaValue> schema, long expectedCode, bool shouldRetry, Ts&& ...ts) {
        cpr::Response res;

        for(uint64_t i = 0; i < maxRetries; i++) {
            res = APIRequest(requestType, std::forward<Ts>(ts)...);

            if(res.status_code != expectedCode) {
                _logger.warn("Status code for request was: {}, expected: {}", res.status_code, expectedCode);
                if(shouldRetry && _shouldRefreshToken(res)) {
                    _logger.warn("Will retry the request after refreshing token, as the user session was kicked.");

                    this->refreshToken();
                    continue;
                }

                _logger.warn("Will not retry, failed the request {} times, PLEASE send bug report.", i + 1);
                _logger.warn("{}", res.text);

                return std::optional<cpr::Response>();
            }

            break;
        }

        try {
            nlohmann::json json = nlohmann::json::parse(res.text);
            std::optional<JSONValidation::JSONSchemaValue> failedValue = JSONValidation::validateJSONSchema(json, schema);
            if(failedValue.has_value()) {
                _logger.error("Invalid JSON schema from request, what: {}, PLEASE send a bug report", failedValue.value().key);
                _logger.error("{}", json.dump());

                return std::optional<cpr::Response>();
            }   
        }
        catch(nlohmann::json::parse_error const&) {
            return std::optional<cpr::Response>();
        }

        return std::optional<cpr::Response>(res);
    }

    void refreshToken();
public:
   /**
    * @brief Initializes the wrapper for the Dokken endpoints, must have valid token. 
    * @details Entry point for the CPPVersus Dokken wrapper, all requests must go through here.
    * @param token Multiversus API key, info on acquiring can be found [here](https://github.com/brianbaldner/multiversus-api-docs/blob/main/auth/README.md).
    * @param logLevel Logging level for debug/info/error output.
    */
    DokkenAPI(std::string token, spdlog::level::level_enum logLevel = spdlog::level::err);

   /**
    * @brief Destructor of the DokkenAPI class
    * @details Cleans up anything the DokkenAPI class may have allocated.
    */
    ~DokkenAPI();

    /**
     * @brief Gets a players info from id
     * @param lookupValue ID or username of the player you want to find.
     * @param lookupType Type of value to use for finding the player. 
     * @returns Optional type with the players info, only if the player is found.
     */
    std::optional<PlayerInfo> getPlayerInfo(std::string lookupValue, PlayerLookupType lookupType = PlayerLookupType::ID);
};

};

#endif