#include "CPPVersus.hpp"
#include "gtest/gtest.h"

// tests if the api fails if it gets an invalid token
TEST(DokkenAPITest, APIInitFail) {
EXPECT_NO_FATAL_FAILURE({
    EXPECT_ANY_THROW(CPPVersus::DokkenAPI("invalid_key"));
});
}