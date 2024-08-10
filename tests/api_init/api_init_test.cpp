#include "cppversus.hpp"
#include "gtest/gtest.h"

#include <memory>

// tests if the api will initialize and get data properly
// NOTE: must have a valid key in the MULTIVERSUS_TOKEN environment variable
TEST(APITest, Init) {
    std::unique_ptr<CPPVersus::DokkenAPIEntry> apiEntry;
    EXPECT_NO_THROW(apiEntry = std::unique_ptr<CPPVersus::DokkenAPIEntry>(new CPPVersus::DokkenAPIEntry(API_KEY)));
}