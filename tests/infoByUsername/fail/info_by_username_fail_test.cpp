#include "cppversus.hpp"
#include "gtest/gtest.h"

#include <memory>

// tests if the api will initialize and fail the player info function properly
// NOTE: must have a valid key in the MULTIVERSUS_TOKEN environment variable
TEST(APIInfoByUsername, Fail) {
    std::unique_ptr<CPPVersus::DokkenAPIEntry> apiEntry;
    EXPECT_NO_THROW(apiEntry = std::unique_ptr<CPPVersus::DokkenAPIEntry>(new CPPVersus::DokkenAPIEntry(API_KEY)));

    EXPECT_EQ(apiEntry->getPlayerInfo("", CPPVersus::PlayerLookupType::USERNAME).has_value(), false);
}