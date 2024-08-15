#include <JSON/JSONUtils.hpp>

using namespace CPPVersus;

std::string JSONUtils::joinKeyPath(std::vector<std::variant<std::string, int>> path) {
    std::string key = "";

    for(size_t i = 0; i < path.size(); i++) {
        const std::variant<std::string, int>& value = path[i];

        if(std::holds_alternative<std::string>(value)) {
            key += std::get<std::string>(value);
        }
        else if(std::holds_alternative<int>(value)) {
            key += fmt::format("[{}]", std::get<int>(value));
        }

        if(i != path.size() - 1) {
            key += ".";
        }
    }

    return key;
}


std::variant<std::string, int> stringToPart(std::string str) {
    if(str[0] == '[' && str[str.size() - 1] == ']') {
        std::string numStr = str.substr(1, str.size() - 1);
        std::stringstream stream(numStr);
        
        int index;
        stream >> index;

        if(stream.fail()) {
            return str;
        }

        return index;
    }

    return str;
}

std::vector<std::variant<std::string, int>> JSONUtils::splitKeyPath(std::string key) {
    std::vector<std::variant<std::string, int>> path;

    std::string current;
    for(char c : key) {
        if(c == '.') {
            if(current.size() > 0) {
                path.push_back(stringToPart(current));
            }

            current = "";
            continue;
        }

        current += c;
    }

    if(current.size() > 0) {
        path.push_back(stringToPart(current));
    }

    return path;
}