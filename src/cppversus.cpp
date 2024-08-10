/**
@file
@brief Main entry point of the CPPVersus wrapper.
*/

#include <cppversus.hpp>
#include <iostream>



template<class ...Ts>
cpr::Response CPPVersus::DokkenAPIEntry::APIGet(Ts&& ...ts) {
    cpr::Header clientHeader = cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }};
    if(_token.has_value()) {
        clientHeader = cpr::Header{{ "x-hydra-access-token", _token.value() }};
    }

    return cpr::Get(
        std::forward<Ts>(ts)...,
        clientHeader,
        cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
        cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
        cpr::Header{{ "Content-Type", "application/json" }}
    );
}

template<class ...Ts>
cpr::Response CPPVersus::DokkenAPIEntry::APIPost(Ts&& ...ts) {
    cpr::Header clientHeader = cpr::Header{{ "x-hydra-client-id", "47201f31-a35f-498a-ae5b-e9915ecb411e" }};
    if(_token.has_value()) {
        clientHeader = cpr::Header{{ "x-hydra-access-token", _token.value() }};
    }

    return cpr::Post(
        std::forward<Ts>(ts)...,
        clientHeader,
        cpr::Header{{ "x-hydra-api-key", "51586fdcbd214feb84b0e475b130fce0" }},
        cpr::Header{{ "x-hydra-user-agent", "Hydra-Cpp/1.132.0" }},
        cpr::Header{{ "Content-Type", "application/json" }}
    );
}


CPPVersus::DokkenAPIEntry::DokkenAPIEntry(std::string token) {
    cpr::Response res = APIPost(
        cpr::Url(fmt::format("{}/access", API_URL)),
        cpr::Body(
            fmt::format(
            R"(     {{                                      )"
            R"(        "auth": {{                           )"
            R"(            "fail_on_missing": true,         )"
            R"(            "steam": "{}"                    )"
            R"(        }},                                  )"
            R"(        "options": [                         )"
            R"(            "wb_network"                     )"
            R"(        ]                                    )"
            R"(     }}                                      )",
                token
            )
        )
    );

    spdlog::info("API called {}/access with token: {}. returned status code: {}", API_URL, token, res.status_code);
    spdlog::info("Response text: {}", res.text);
}

CPPVersus::DokkenAPIEntry::~DokkenAPIEntry() {

}