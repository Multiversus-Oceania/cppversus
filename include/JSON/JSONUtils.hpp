#ifndef __JSON_UTILS_HPP__
#define __JSON_UTILS_HPP__

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <optional>
#include <cstdint>
#include <sstream>

#include <fmt/format.h>

namespace CPPVersus {
/**
 * @brief Contains various methods for JSON handling.
 */
namespace JSONUtils {

/**
 * @brief Joins a key path into a string, e.g { "server_data", "identity", "alternate", "wb_network", 0 } = "server_data.identity.alternate.wb_network[0]"
 * @param path Path of the key.
 * @returns String representation of the path.
 */
std::string joinKeyPath(std::vector<std::variant<std::string, int>> path);

/**
 * @brief Splits a key path into an array, e.g "server_data.identity.alternate.wb_network[0]" = { "server_data", "identity", "alternate", "wb_network", 0 }
 * @param key Path of the key.
 * @returns Array representation of the key.
 */
std::vector<std::variant<std::string, int>> splitKeyPath(std::string key);

};
};

#endif