#ifndef __CPP_VERSUS_HPP__
#define __CPP_VERSUS_HPP__

#include <string>
#include <optional>
#include <stdexcept>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

#include <nlohmann/json.hpp>

#include <MinimalPlayerInfo.hpp>
#include <PlayerInfo.hpp>

namespace CPPVersus {
const std::string API_URL = "https://dokken-api.wbagora.com";

class DokkenAPIEntry {
private:
    const std::string _authToken;
    std::optional<std::string> _token;

    template<class ...Ts>
    cpr::Response APIGet(Ts&& ...ts);
    template<class ...Ts>
    cpr::Response APIPost(Ts&& ...ts);


    void refreshToken();

public:
   /**
    * @brief Initializes the wrapper for the Dokken endpoints, must have valid token. 
    * @details Entry point for the CPPVersus Dokken wrapper, all requests must go through here.
    * @param token Multiversus API key, info on acquiring can be found [here](https://github.com/brianbaldner/multiversus-api-docs/blob/main/auth/README.md).
    */
    DokkenAPIEntry(std::string token);

   /**
    * @brief Destructor of the DokkenAPIEntry class
    * @details Cleans up anything the DokkenAPIEntry class may have allocated.
    */
    ~DokkenAPIEntry();


    /**
     * @brief Gets a players info from username
     * @param username Username of the player you want to find.
     * @param limit Maximum number of players for the API to return.
     * @returns Optional type with the players info, only if the player is found.
     */
    std::optional<MinimalPlayerInfo> getPlayerInfoByUsername(std::string username, uint32_t limit = 5);
};

};

#endif