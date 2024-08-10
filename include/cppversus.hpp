#ifndef __CPP_VERSUS_HPP__
#define __CPP_VERSUS_HPP__

#include <string>
#include <optional>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

namespace CPPVersus {
const std::string API_URL = "https://dokken-api.wbagora.com";

class DokkenAPIEntry {
private:
    std::optional<std::string> _token;

    template<class ...Ts>
    cpr::Response APIGet(Ts&& ...ts);
    template<class ...Ts>
    cpr::Response APIPost(Ts&& ...ts);

public:
   /**
    * @brief Initializes the wrapper for the Dokken endpoints, must have valid token. 
    * @details Entry point for the CPPVersus Dokken wrapper, all requests must go through here.
    * @param token Multiversus API key, info on acquiring can be found here: https://github.com/brianbaldner/multiversus-api-docs/blob/main/auth/README.md
    */
    DokkenAPIEntry(std::string token);

   /**
    * @brief Destructor of the DokkenAPIEntry class
    * @details Cleans up anything the DokkenAPIEntry class may have allocated.
    */
    ~DokkenAPIEntry();
};

};

#endif