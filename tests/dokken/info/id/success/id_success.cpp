#include "CPPVersus.hpp"
#include "gtest/gtest.h"

#include <memory>

// tests if the api works and can get data on a user properly
// NOTE: must have a valid key in the MULTIVERSUS_TOKEN environment variable
TEST(DokkenAPITest, InfoFromIDSuccess) {
    std::unique_ptr<CPPVersus::DokkenAPI> dokkenAPI;
    EXPECT_NO_THROW(dokkenAPI = std::unique_ptr<CPPVersus::DokkenAPI>(new CPPVersus::DokkenAPI(API_KEY)));

    EXPECT_EQ(dokkenAPI->getPlayerInfo("6289bfe8213b6d574354d9fc", CPPVersus::PlayerLookupType::ID).has_value(), true);
}