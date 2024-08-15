#include <JSON/JSONUtils.hpp>

#include <spdlog/spdlog.h>

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

std::vector<std::variant<std::string, int>> JSONUtils::splitKeyPath(std::string key) {
    std::vector<std::variant<std::string, int>> path;

    std::string current;
    for(size_t i = 0; i < key.size(); i++) {
        const char& c = key[i];

        switch(c) {
        case '[': {
            size_t bracketEndPos = key.find_first_of(']', i);
            size_t dotEndPos = key.find_first_of('.', i);
            
            if(bracketEndPos == std::string::npos || (dotEndPos != std::string::npos && dotEndPos < bracketEndPos)) {
                current += c;
                break;
            }

            std::string subStr = key.substr(i + 1, bracketEndPos - (i - 2));
            std::stringstream stream(subStr);
        
            int index;
            stream >> index;

            if(stream.fail()) {
                current += c;
                break;
            }

            if(current.size() > 0) {
                path.push_back(current);
                current = "";
            }

            path.push_back(index);

            i = bracketEndPos;
            break;
        }
        case '.': {
            if(current.size() <= 0) break;
            
            path.push_back(current);
            current = "";

            break;
        }
        default: {
            current += c;
            break;
        }
        }
    }

    if(current.size() > 0) {
        path.push_back(current);
    }

    return path;
}