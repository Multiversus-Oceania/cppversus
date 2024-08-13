#ifndef __DOKKEN_API_HPP__
#define __DOKKEN_API_HPP__

#include <string>
#include <optional>
#include <execution>
#include <algorithm>
#include <stdexcept>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

#include <nlohmann/json.hpp>

#include <PlayerInfo.hpp>

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
    const static uint64_t maxRetries = 5;

    const std::string _authToken;
    std::optional<std::string> _token;

    template<class ...Ts>
    cpr::Response APIGet(Ts&& ...ts) {
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
    cpr::Response APIPost(Ts&& ...ts) {
        return cpr::Post(
            std::forward<Ts>(ts)...,
            cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
            cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
            cpr::Header{{ "x-hydra-access-token", _token.value_or("") }},
            cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }},
            cpr::Header{{ "Content-Type", "application/json" }}
        );
    }


    void refreshToken();
    bool shouldRetry(cpr::Response res);


    std::optional<PlayerInfo> _getPlayerInfoFromUsername(std::string username, uint64_t _retryNumber = 0);
    std::optional<PlayerInfo> _getPlayerInfoFromID(std::string id, uint64_t _retryNumber = 0);

    std::optional<PlayerInfo> _getPlayerInfo(std::string lookupValue, PlayerLookupType lookupType, uint64_t _retryNumber = 0);

public:
   /**
    * @brief Initializes the wrapper for the Dokken endpoints, must have valid token. 
    * @details Entry point for the CPPVersus Dokken wrapper, all requests must go through here.
    * @param token Multiversus API key, info on acquiring can be found [here](https://github.com/brianbaldner/multiversus-api-docs/blob/main/auth/README.md).
    */
    DokkenAPI(std::string token);

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