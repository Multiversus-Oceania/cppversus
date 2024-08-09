/**
@file
@brief Main entry point of the CPPVersus wrapper.
*/

#include <cppversus.hpp>

//! Initializes the APIEntry class
/*!
    Main entry point for the CPPVersus wrapper, all requests should go through here.
        @param token Multiversus API token, info on acquiring can be found here: https://github.com/brianbaldner/multiversus-api-docs/blob/main/auth/README.md
*/
CPPVersus::APIEntry::APIEntry(std::string token) {
    spdlog::debug("API created with token: {}", token);
}


//! Destructor of the APIEntry class
/*!
    Cleans up anything the APIEntry class may have allocated.
*/
CPPVersus::APIEntry::~APIEntry() {

}