#include "cppversus.hpp"
#include "gtest/gtest.h"

// tests if the api fails if it gets an invalid token
TEST(APITest, InitFail) {
EXPECT_NO_FATAL_FAILURE({
    EXPECT_ANY_THROW(CPPVersus::DokkenAPIEntry("invalid_key"));
});
}