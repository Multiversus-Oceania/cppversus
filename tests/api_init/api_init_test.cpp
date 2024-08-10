#include "cppversus.hpp"
#include "gtest/gtest.h"

// tests if the api will initialize and get data properly
// NOTE: must have a valid key in the MULTIVERSUS_TOKEN environment variable
TEST(APITest, Init) {
    CPPVersus::DokkenAPIEntry* apiEntry;

    EXPECT_NO_THROW(apiEntry = new CPPVersus::DokkenAPIEntry(API_KEY));
    EXPECT_NO_FATAL_FAILURE(delete apiEntry);
}