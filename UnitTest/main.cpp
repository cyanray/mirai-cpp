#include <mirai.h>
#include <vector>
#include <gtest/gtest.h>

TEST(MessageChain_Test, Set_Plain) {
    using namespace Cyan;
    MessageChain mc;
    json j =
    {
        {{"type","Source"},{"time",1234},{"id",123456}},
        {{"type","Plain"},{"text","textsssss"}}
    };
    mc.Set(j);
    string text = mc.ToJson()[0]["text"];
    ASSERT_STREQ("textsssss", text.c_str());
}

TEST(MessageChain_Test, Add_and_Get) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    auto m = mc.GetFirst<PlainMessage>();
    ASSERT_STREQ("Hello", m.Text().c_str());
}

TEST(MessageChain_Test, Add_and_Get2) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add<PlainMessage>("Hello");
    auto m = mc.GetFirst<PlainMessage>();
    ASSERT_STREQ("Hello", m.Text().c_str());
}

TEST(MessageChain_Test, Add_and_Get3) {
    using namespace Cyan;
    MessageChain mc;
    AppMessage app("<app></app>");
    mc.Add(app);
    AtAllMessage atall;
    mc.Add(atall);
    AtMessage at(123456_qq);
    mc.Add(at);
    FaceMessage face("wx");
    mc.Add(face);
    FlashImageMessage flash;
    mc.Add(flash);
    ImageMessage img;
    mc.Add(img);
    JsonMessage json;
    mc.Add(json);
    PlainMessage plain("Hello");
    mc.Add(plain);
    PokeMessage poke;
    mc.Add(poke);
    QuoteMessage quote;
    mc.Add(quote);
    VoiceMessage voice;
    mc.Add(voice);
    XmlMessage xml;
    mc.Add(xml);
    auto m = mc.GetFirst<PlainMessage>();
    ASSERT_STREQ("Hello", m.Text().c_str());
}

TEST(MessageChain_Test, Insert_and_Get) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add<PlainMessage>("OOOOOO");
    mc.Insert(mc.begin(), PlainMessage("Hello"));
    ASSERT_TRUE(mc[0]->GetType() == "Plain");
    auto mptr = std::dynamic_pointer_cast<PlainMessage>(mc[0]);
    ASSERT_STREQ("Hello", mptr->Text().c_str());
}

TEST(MessageChain_Test, GetAll) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hi"));
    auto m = mc.GetAll<PlainMessage>();
    ASSERT_TRUE(m.size() == 2);
    ASSERT_STREQ("Hello", m[0].Text().c_str());
    ASSERT_STREQ("Hi", m[1].Text().c_str());
}

TEST(MessageChain_Test, Count) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hi"));
    ASSERT_TRUE(mc.Count() == 2);
}

TEST(MessageChain_Test, get_at) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hi"));
    ASSERT_TRUE(mc[0]->GetType() == "Plain");
    auto mptr = std::dynamic_pointer_cast<PlainMessage>(mc[1]);
    ASSERT_STREQ("Hi", mptr->Text().c_str());
}

TEST(MessageChain_Test, Clear) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hi"));
    mc.Clear();
    ASSERT_TRUE(mc.Count() == 0);
}

TEST(MessageChain_Test, Remove) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hi"));
    mc.Remove(PlainMessage("Hello"));
    ASSERT_TRUE(mc.Count() == 1);
    auto mptr = std::dynamic_pointer_cast<PlainMessage>(mc[0]);
    ASSERT_STREQ("Hi", mptr->Text().c_str());
}

TEST(MessageChain_Test, GetPlainText) {
    using namespace Cyan;
    MessageChain mc;
    mc.Plain("Number:").Plain(12345);
    string text = mc.GetPlainText();
    ASSERT_STREQ("Number:12345", text.c_str());
}

TEST(MessageChain_Test, ImageMessage) {
    using namespace Cyan;
    MessageChain mc;
    MiraiImage img;
    img.Id = "{qweasd}.png";
    mc.Plain("Image:").Image(img);
    auto img_msg = mc.GetFirst<ImageMessage>();
    ASSERT_TRUE(img_msg.ToMiraiImage().Id == "{qweasd}.png");
}

TEST(MessageChain_Test, FlashImageMessage) {
    using namespace Cyan;
    MessageChain mc;
    MiraiImage img;
    img.Id = "{qweasd}.png";
    mc.Plain("Image:").FlashImage(img);
    auto img_msg = mc.GetFirst<FlashImageMessage>();
    ASSERT_TRUE(img_msg.ToMiraiImage().Id == "{qweasd}.png");
}

TEST(MessageChain_Test, AtMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(AtMessage(1234_qq));
    auto at = mc.GetFirst<AtMessage>();
    ASSERT_TRUE(at.Target().ToInt64() == 1234);
}

TEST(MessageChain_Test, AtAllMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(AtAllMessage());
    auto atall = mc.GetFirst<AtAllMessage>();
    ASSERT_TRUE(true);
}

TEST(MessageChain_Test, FaceMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(FaceMessage(1));
    auto f = mc.GetFirst<FaceMessage>();
    ASSERT_TRUE(f.FaceId() == 1);
}

TEST(MessageChain_Test, XmlMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(XmlMessage("<xml></xml>"));
    auto x = mc.GetFirst<XmlMessage>();
    ASSERT_STREQ("<xml></xml>", x.Xml().c_str());
}

TEST(MessageChain_Test, AppMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(AppMessage("<app></app>"));
    auto x = mc.GetFirst<AppMessage>();
    ASSERT_TRUE(x.Content() == "<app></app>");
}

TEST(MessageChain_Test, JsonMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(JsonMessage(R"({"type":"json"})"));
    auto x = mc.GetFirst<JsonMessage>();
    ASSERT_TRUE(x.Json() == R"({"type":"json"})");
}

TEST(MessageChain_Test, PokeMessage) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PokeMessage(PokeType::Poke));
    auto x = mc.GetFirst<PokeMessage>();
    ASSERT_TRUE(x.Poke() == PokeType::Poke);
    x.Poke(PokeType::SixSixSix);
    ASSERT_TRUE(x.Name() == "SixSixSix");
}

TEST(MessageChain_Test, RemoveAt) {
    using namespace Cyan;
    MessageChain mc;
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hello"));
    mc.Add(PlainMessage("Hi"));
    mc.RemoveAt(0);
    mc.RemoveAt(1);
    ASSERT_TRUE(mc.Count() == 1);
    auto mptr = std::dynamic_pointer_cast<PlainMessage>(mc[0]);
    ASSERT_STREQ("Hello", mptr->Text().c_str());
}

TEST(MessageChain_Test, Iterator) {
    using namespace Cyan;
    const vector<string> test_data = { "Well", "Hello" , "Hi" };
    MessageChain mc;
    for (const string& str : test_data)
    {
        mc.Add<PlainMessage>(str);
    }
    int i = 0;
    for (const auto& m : mc)
    {
        auto mptr = std::dynamic_pointer_cast<PlainMessage>(m);
        ASSERT_STREQ(test_data[i++].c_str(), mptr->Text().c_str());
    }
}



TEST(ABC, TEST2) {
    ASSERT_TRUE(2 == 2);
}

TEST(ABC, TEST3) {
    ASSERT_TRUE(2 == 2);
}