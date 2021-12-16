#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>
using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{
	system("chcp 65001");
	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
	bot.Connect(opts);
	cout << "Bot working..." << endl;

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

	string command;
	while (cin >> command)
	{
		if (command == "exit")
		{
			bot.Disconnect();
			break;
		}
	}

	return 0;
}