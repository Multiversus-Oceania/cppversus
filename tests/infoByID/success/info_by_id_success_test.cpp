#include "cppversus.hpp"
#include "gtest/gtest.h"

#include <memory>

// tests if the api works and can get data on a user properly
// NOTE: must have a valid key in the MULTIVERSUS_TOKEN environment variable
TEST(APIInfoByID, Success) {
    std::unique_ptr<CPPVersus::DokkenAPIEntry> apiEntry;
    EXPECT_NO_THROW(apiEntry = std::unique_ptr<CPPVersus::DokkenAPIEntry>(new CPPVersus::DokkenAPIEntry(API_KEY)));

    EXPECT_EQ(apiEntry->getPlayerInfo("6289bfe8213b6d574354d9fc", CPPVersus::PlayerLookupType::ID).has_value(), true);
}