#ifndef __CPP_VERSUS_HPP__
#define __CPP_VERSUS_HPP__

#include <string>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

namespace CPPVersus {

class APIEntry {
private:
    std::string _token;

public:
    APIEntry(std::string token);
    ~APIEntry();
};

};

#endif