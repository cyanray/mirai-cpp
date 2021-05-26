#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;
	system("chcp 65001");
	MiraiBot bot("127.0.0.1", 539);
	while (true)
	{
		try
		{
			bot.Verify("INITKEY7A3O1a9v", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;

	FriendImage img = bot.UploadFriendImage("D:\\test.png");
	GroupImage gImg = bot.UploadGroupImage("D:\\test.png");
	TempImage tImg = bot.UploadTempImage("D:\\test.png");

	bot.On<Message>([&](Message m)
		{
			try
			{
				if(m.MessageChain.GetPlainTextFirst() == "点歌")
				{
					m.Reply(MessageChain().App(R"( {"app":"com.tencent.structmsg","config":{"autosize":true,"ctime":1592909927,"forward":true,"token":"8e01d9ea9297158cef76512886cfe8bc","type":"normal"},"desc":"新闻","extra":{"app_type":1,"appid":100497308,"msg_seq":6841496030403626000},"meta":{"news":{"action":"","android_pkg_name":"","app_type":1,"appid":100497308,"desc":"周杰伦","jumpUrl":"https://i.y.qq.com/v8/playsong.html?platform=11&appshare=android_qq&appversion=9160007&songmid=004Z8Ihr0JIu5s&type=0&appsongtype=1&_wv=1&source=qq&ADTAG=qfshare","preview":"https://y.gtimg.cn/music/photo_new/T002R300x300M000003DFRzD192KKD.jpg","source_icon":"","source_url":"","tag":"QQ音乐","title":"七里香"}},"prompt":"[分享]七里香","ver":"0.0.0.1","view":"news"} )"));
				}
				cout << m.ToString() << endl;
			}
			catch (const exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	
	bot.On<FriendMessage>(
		[&](FriendMessage fm)
		{
			try
			{
				fm.Reply(MessageChain().FlashImage(img));
				fm.Reply(MessageChain().Poke(PokeType::FangDaZhao));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.On<GroupMessage>(
		[&](GroupMessage gm)
		{
			try
			{
				gm.Reply(MessageChain().FlashImage(gImg));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.On<TempMessage>(
		[&](TempMessage gm)
		{
			try
			{
				gm.Reply(MessageChain().Image(tImg));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});


	bot.EventLoop();


	return 0;
}