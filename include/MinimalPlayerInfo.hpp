#ifndef __MINIMAL_PLAYER_INFO_HPP__
#define __MINIMAL_PLAYER_INFO_HPP__

#include <string>
#include <optional>
#include <vector>
#include <cstdint>

#include <map>

#include <Characters.hpp>

namespace CPPVersus {

// TODO: Unfinished.
struct MinimalPlayerInfo {
    std::string id;
    std::optional<std::string> updated_at;
    std::string account_id;
    std::string created_at;
    std::optional<std::string> last_login;


};

};

#endif