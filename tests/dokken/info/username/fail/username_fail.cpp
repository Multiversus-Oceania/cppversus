#include "CPPVersus.hpp"
#include "gtest/gtest.h"

#include <memory>

// tests if the api will initialize and fail the player info function properly
// NOTE: must have a valid key in the MULTIVERSUS_TOKEN environment variable
TEST(DokkenAPITest, InfoFromUsernameFail) {
    std::unique_ptr<CPPVersus::DokkenAPI> dokkenAPI;
    EXPECT_NO_THROW(dokkenAPI = std::unique_ptr<CPPVersus::DokkenAPI>(new CPPVersus::DokkenAPI(API_KEY)));

    EXPECT_EQ(dokkenAPI->getPlayerInfo("", CPPVersus::PlayerLookupType::USERNAME).has_value(), false);
}