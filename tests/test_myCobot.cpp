#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


TEST_CASE("testing will pass") {
    CHECK(42 == 42);
}

// TEST_CASE("testing will fail") {
//     CHECK(0 == 42);
// }

TEST_CASE("subcase example")
{
    SUBCASE("Test will pass") {
        CHECK(true);
    }
    // SUBCASE("Test will fail") {
    //     CHECK(false);
    // }
}
