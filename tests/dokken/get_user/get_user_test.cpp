#include "CPPVersus.hpp"
#include "gtest/gtest.h"

#include <optional>

// tests if the api fails if it gets an invalid token
TEST(DokkenAPITest, GetUserAPI) {
EXPECT_NO_FATAL_FAILURE({
    std::unique_ptr<CPPVersus::DokkenAPI> dokkenAPI;
    EXPECT_NO_THROW(dokkenAPI = std::unique_ptr<CPPVersus::DokkenAPI>(new CPPVersus::DokkenAPI(API_KEY)));

    const std::string usernameToFind = "donkisme";
    std::optional<CPPVersus::PlayerInfo> playerInfo = dokkenAPI->getPlayerInfo(usernameToFind, CPPVersus::PlayerLookupType::USERNAME);

    ASSERT_EQ(playerInfo.has_value(), true);
    EXPECT_EQ(playerInfo.value().username, usernameToFind);
});
}