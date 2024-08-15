#include <JSONValidation.hpp>

using namespace CPPVersus;

std::string JSONValidation::joinKeyName(std::vector<std::variant<std::string, int>> path) {
    std::string keyName = "";

    for(size_t i = 0; i < path.size(); i++) {
        const std::variant<std::string, int>& value = path[i];

        if(std::holds_alternative<std::string>(value)) {
            keyName += std::get<std::string>(value);
        }
        else if(std::holds_alternative<int>(value)) {
            keyName += std::to_string(std::get<int>(value));
        }

        if(i != path.size() - 1) {
            keyName += ".";
        }
    }

    return keyName;
}


bool JSONValidation::validateJSONValue(nlohmann::json value, JSONValidator validator, bool allowsNull) {
    if(allowsNull && Validators::nullValidator.validateFunc(value)) return true;
    if(validator.validateFunc(value)) return true;
    
    return false;
}

std::optional<JSONValidation::JSONSchemaValue> JSONValidation::validateJSONSchema(nlohmann::json json, std::vector<JSONValidation::JSONSchemaValue> schema) {
    for(const JSONValidation::JSONSchemaValue& schemaValue : schema) {
        nlohmann::json currentValue = json;

        // Traverse through the json object to get to the value.
        for(const std::variant<std::string, int>& key : schemaValue.key) {            
            if(std::holds_alternative<std::string>(key)) {
                const std::string str = std::get<std::string>(key);

                if(currentValue.find(str) == currentValue.end()) {
                    return std::optional<JSONValidation::JSONSchemaValue>(schemaValue);
                }

                currentValue = currentValue[str];
            }
            else if(std::holds_alternative<int>(key)) {
                const int index = std::get<int>(key);
                
                if(currentValue.size() <= (unsigned int)index) {
                    return std::optional<JSONValidation::JSONSchemaValue>(schemaValue);
                }

                currentValue = currentValue[index];
            }
        }

        if(!validateJSONValue(currentValue, schemaValue.validator, schemaValue.allowsNull)) {
            return std::optional<JSONValidation::JSONSchemaValue>(schemaValue);
        }
    }

    return std::optional<JSONValidation::JSONSchemaValue>();
}