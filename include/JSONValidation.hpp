#ifndef __JSON_VALIDATION_HPP__
#define __JSON_VALIDATION_HPP__

#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <optional>
#include <cstdint>

#include <nlohmann/json.hpp>

namespace CPPVersus {
/**
 * @brief Contains methods and structs for validating a JSON values schema.
 */
namespace JSONValidation {

/**
 * @brief Joins a key path into a string, e.g { "server_data", "identity", "alternate" } = "server_data.identity.alternate"
 * @param path Path of the key.
 * @returns String representation of the path.
 */
std::string joinKeyName(std::vector<std::variant<std::string, int>> path);

/**
 * @brief Validator for checking a JSON value, has a name and validator function.
 * @param name Name of the validator.
 * @param validateFunc Function to check if the json is valid, returns true if so, else false.
 */
struct JSONValidator {
    std::string name;
    std::function<bool(nlohmann::json json)> validateFunc;
};

/**
 * @brief Part of the schema array to be validated.
 * @param key Key path(in order) of the data to validate
 * @param validator Validator for checking correctness of the value.
 * @param allowsNull Should the value allow being null.
 */
struct JSONSchemaValue {
    std::vector<std::variant<std::string, int>> key;

    JSONValidator validator;
    bool allowsNull = false;
};

namespace Validators {

const JSONValidator primitiveValidator  = { "primitive",  [](nlohmann::json json){ return json.is_primitive();       } };
const JSONValidator booleanValidator    = { "boolean",    [](nlohmann::json json){ return json.is_boolean();         } };
const JSONValidator numberValidator     = { "number",     [](nlohmann::json json){ return json.is_number();          } };
const JSONValidator floatValidator      = { "float",      [](nlohmann::json json){ return json.is_number_float();    } };
const JSONValidator integerValidator    = { "integer",    [](nlohmann::json json){ return json.is_number_integer();  } };
const JSONValidator unsignedValidator   = { "unsigned",   [](nlohmann::json json){ return json.is_number_unsigned(); } };
const JSONValidator nullValidator       = { "null",       [](nlohmann::json json){ return json.is_null();            } };
const JSONValidator stringValidator     = { "string",     [](nlohmann::json json){ return json.is_string();          } };

const JSONValidator structuredValidator = { "structured", [](nlohmann::json json){ return json.is_structured();      } };
const JSONValidator objectValidator     = { "object",     [](nlohmann::json json){ return json.is_object();          } };
const JSONValidator arrayValidator      = { "array",      [](nlohmann::json json){ return json.is_array();           } };

};

/**
 * @brief Validates a JSON objects value
 * @param value JSON object
 * @param validator Validator to check the JSON object with
 * @param allowsNull Should the validator allow null.
 * @return true if value was valid else false.
 */
bool validateJSONValue(nlohmann::json value, JSONValidator validator, bool allowsNull = false);

/**
 * @brief Validates a JSON values schema.
 * @param json JSON value to validate.
 * @param schema Array of schema value parts.
 * @return if failed returns the JSONSchemaValue that failed, else null optional value.
 */
std::optional<JSONSchemaValue> validateJSONSchema(nlohmann::json json, std::vector<JSONSchemaValue> schema);

};
};

#endif