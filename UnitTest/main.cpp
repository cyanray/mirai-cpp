#include <mirai.h>
#include <gtest/gtest.h>

TEST(ABC, TEST1) {
    using namespace Cyan;
    MessageChain mc;
    json j =
    {
        {{"type","Source"},{"time",1234},{"id",123456}},
        {{"type","Plain"},{"text","textsssss"}}
    };
    mc.Set(j);
    string text = mc.ToJson()[0]["text"];
    ASSERT_TRUE(text == "textsssss");
}

TEST(ABC, TEST2) {
    ASSERT_TRUE(2 == 2);
}

TEST(ABC, TEST3) {
    ASSERT_TRUE(2 == 2);
}