#include <gtest/gtest.h>
#include "../src/http/response.h"

TEST(ResponseTest, HtmlMimeType)
{
    EXPECT_EQ(
        getMimeType("index.html"),
        "text/html"
    );
}

TEST(ResponseTest, CssMimeType)
{
    EXPECT_EQ(
        getMimeType("style.css"),
        "text/css"
    );
}

TEST(ResponseTest, JsMimeType)
{
    EXPECT_EQ(
        getMimeType("app.js"),
        "application/javascript"
    );
}

TEST(ResponseTest, DefaultMimeType)
{
    EXPECT_EQ(
        getMimeType("file.xyz"),
        "text/plain"
    );
}