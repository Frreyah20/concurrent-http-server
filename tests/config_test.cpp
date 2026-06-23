#include <gtest/gtest.h>
#include <fstream>

#include "../src/config/config.h"

TEST(ConfigTest, LoadConfigFile)
{
    std::ofstream file("test_config.txt");

    file << "port=8080\n";
    file << "workers=4\n";
    file << "root=public\n";

    file.close();

    EXPECT_TRUE(loadConfig("test_config.txt"));

    EXPECT_EQ(config.port, 8080);
    EXPECT_EQ(config.workers, 4);
    EXPECT_EQ(config.root, "public");

    std::remove("test_config.txt");
}