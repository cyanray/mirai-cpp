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

	bot.On<FriendMessage>(
		[&](FriendMessage fm)
		{
			try
			{
				fm.Reply(MessageChain().Image(img));
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
				bot.SendMessage(gm.Sender.Group.GID, MessageChain().Image(gImg));
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