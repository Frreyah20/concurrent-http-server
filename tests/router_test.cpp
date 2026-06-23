#include <gtest/gtest.h>
#include "../src/router/router.h"

TEST(RouterTest, GetRouteRegistration)
{
    Router router;

    router.get("/hello", []() {
        return "world";
    });

    std::string path = "/hello";

    EXPECT_TRUE(router.hasRoute("GET", path));
    EXPECT_EQ(router.route("GET", "/hello"), "world");
}

TEST(RouterTest, MissingRoute)
{
    Router router;

    EXPECT_EQ(
        router.route("GET", "/missing"),
        "404 Not Found"
    );
}